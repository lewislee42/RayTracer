
#include "MetalAdder.hpp"


// initializes the metal shader function
MetalAdder::MetalAdder(CA::MetalLayer* metalLayer, const int& imageW, const int& imageH) {
    this->swapchain = metalLayer;
	this->_mDevice = this->swapchain->device();

	MTL::Library* defaultLibrary = this->_mDevice->newDefaultLibrary();
	if (defaultLibrary == nullptr) {
		std::cerr << "Error: could not find default library\n";
		return; // maybe throw error here instead
	}

	NS::String* str = NS::String::string("computeMain", NS::ASCIIStringEncoding); 
	MTL::Function* rayColorTest = defaultLibrary->newFunction(str);

	defaultLibrary->release();
	if (rayColorTest == nullptr) {
		std::cerr << "Error: could not find rayColorTest function\n";
		return; // maybe throw error here instead
	}

	NS::Error* error = nullptr;
	this->_mAddFunctionPso = this->_mDevice->newComputePipelineState(rayColorTest, &error);
	rayColorTest->release();

	if (this->_mAddFunctionPso == nullptr || error) {
		std::cerr << "Error: failed to create pipeline state object: " << error << std::endl;
		return; // maybe throw error here instead
	}

	this->_mCommandQueue = this->_mDevice->newCommandQueue();
    
	if (this->_mCommandQueue == nullptr) {
		std::cerr << "Error: failed to find command queue\n";
		return; // maybe throw error here instead
	}

	this->_metaData = nullptr;
	this->_objects = nullptr;
	this->_bvhNodes = nullptr;
	this->_objectsData = nullptr;
    this->_outputText = nullptr;
}

void MetalAdder::sendComputeCommand(
	const RaytracingMetaData& metaData,
	const Object3D* objects,
	const BVHNode* bvhNodes,
	const ObjectsData& objectsData
) {
	this->updateVariables(metaData, objects, bvhNodes, objectsData);

	if (!this->_metaData || !this->_objects || !this->_bvhNodes || !this->_objectsData) {
        std::cerr << "Error: nothing initailized in buffers\n";
        return;
    }
    
    
	MTL::CommandBuffer* commandBuffer = _mCommandQueue->commandBuffer();

	if (commandBuffer == nullptr) {
		std::cerr << "Error: failed to create command buffer\n";
		return; // maybe throw error here instead
	}

	MTL::ComputeCommandEncoder* computeCommandEncoder = commandBuffer->computeCommandEncoder();
	if (computeCommandEncoder == nullptr) {
		std::cerr << "Error: failed to start compute command encoder\n";
		return; // maybe throw error here instead
	}


	computeCommandEncoder->setComputePipelineState(this->_mAddFunctionPso);
	computeCommandEncoder->setBuffer(this->_metaData, 0, 0);
	computeCommandEncoder->setBuffer(this->_objects, 0, 1);
	computeCommandEncoder->setBuffer(this->_bvhNodes, 0, 2);
	computeCommandEncoder->setBuffer(this->_objectsData, 0, 3);
    computeCommandEncoder->setTexture(this->_outputText, 0);


    MTL::Size gridSize = MTL::Size::Make(metaData.imageW, metaData.imageH, 1);

	MTL::Size threadgroupSize = MTL::Size::Make(16, 16, 1);
    
	computeCommandEncoder->dispatchThreads(gridSize, threadgroupSize);
	computeCommandEncoder->endEncoding();

	commandBuffer->commit();
	commandBuffer->waitUntilCompleted();

    
    // for blitting process
    MTL::CommandBuffer* commandBuffer1 = _mCommandQueue->commandBuffer();

    if (commandBuffer1 == nullptr) {
        std::cerr << "Error: failed to create command buffer\n";
        return; // maybe throw error here instead
    }
    
	CA::MetalDrawable* drawable = this->swapchain->nextDrawable();
	MTL::Texture* drawableTexture = drawable->texture();

    MTL::BlitCommandEncoder* blit = commandBuffer1->blitCommandEncoder();
    blit->copyFromTexture(
		this->_outputText,
		0,
		0,
		{0, 0, 0},
		{(unsigned int)metaData.imageW, (unsigned int)metaData.imageH, 1},
		drawableTexture,
		0,
		0,
		{0, 0, 0}
	);
    blit->endEncoding();
    
    commandBuffer1->presentDrawable(drawable);
    commandBuffer1->commit();
    commandBuffer1->waitUntilCompleted();
}

// NOTE:: DOES NOT HANDLE FOOKING UPDATING OBJECTS YET
void MetalAdder::updateVariables(
	const RaytracingMetaData& metaData,
	const Object3D* objects,
	const BVHNode* bvhNodes,
	const ObjectsData& objectsData
) {
    
    if (this->_mDevice == nullptr) {
        std::cerr << "Error: device not created before running updateVariables\n";
        return;
    }
	if (this->_metaData == nullptr) {
		this->_metaData = this->_mDevice->newBuffer(sizeof(metaData), MTL::ResourceStorageModeShared);
	}
	memcpy(this->_metaData->contents(), &metaData, sizeof(RaytracingMetaData));
	
	if (this->_objectsData == nullptr) {
		this->_objectsData = this->_mDevice->newBuffer(sizeof(objectsData), MTL::ResourceStorageModeShared);
	}
	memcpy(this->_objectsData->contents(), &objectsData, sizeof(ObjectsData));
	
    if (this->_objects == nullptr) {
		this->_objects = this->_mDevice->newBuffer(sizeof(Object3D) * objectsData.objectAmount, MTL::ResourceStorageModeShared);
	memcpy(this->_objects->contents(), objects, sizeof(Object3D) * objectsData.objectAmount);}
	
	if (this->_bvhNodes == nullptr) {
		this->_bvhNodes = this->_mDevice->newBuffer(sizeof(BVHNode) * objectsData.nodesCount, MTL::ResourceStorageModeShared);
	memcpy(this->_bvhNodes->contents(), bvhNodes, sizeof(BVHNode) * objectsData.nodesCount);}
    
    // temp
    if (this->_outputText == nullptr) {
        MTL::TextureDescriptor* desc = MTL::TextureDescriptor::texture2DDescriptor(MTL::PixelFormatBGRA8Unorm, metaData.imageW, metaData.imageH, false);
        desc->setUsage(MTL::TextureUsageShaderRead | MTL::TextureUsageShaderWrite);
        
        this->_outputText = this->_mDevice->newTexture(desc);
        desc->release();
    }
}
