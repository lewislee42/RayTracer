
#include <MetalAdder.hpp>


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
    std::cout << "Command Queue address: " << &this->_mCommandQueue << std::endl;
    std::cout << "Command Queue address: " << this->_mCommandQueue << std::endl;
    
	if (this->_mCommandQueue == nullptr) {
		std::cerr << "Error: failed to find command queue\n";
		return; // maybe throw error here instead
	}

	this->_camData = nullptr;
	this->_objects = nullptr;
	this->_objectAmount = nullptr;
	this->_bounces = nullptr;

    // TEMP
    this->_outputText = nullptr;
}

void MetalAdder::sendComputeCommand(
	struct CameraData& camData,
	const Object3D* objects,
	const uint& objectAmount,
	const uint& bounces
) {
	this->updateVariables(camData, objects, objectAmount, bounces);

    if (!this->_camData || !this->_objects || !this->_objectAmount || !this->_bounces) {
        std::cerr << "Error: nothing initailized in buffers\n";
        return;
    }
    
    
	MTL::CommandBuffer* commandBuffer = _mCommandQueue->commandBuffer();
    // std::cout << "Command buffers address: " << commandBuffer << std::endl;

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
	computeCommandEncoder->setBuffer(this->_camData, 0, 0);
	computeCommandEncoder->setBuffer(this->_objects, 0, 1);
	computeCommandEncoder->setBuffer(this->_objectAmount, 0, 2);
	computeCommandEncoder->setBuffer(this->_bounces, 0, 3);
    computeCommandEncoder->setTexture(this->_outputText, 0);


    MTL::Size gridSize = MTL::Size::Make(camData.imageW, camData.imageH, 1);
    // std::cout << "gridSize " << gridSize.width << " " << gridSize.height << " " << gridSize.depth << std::endl;
    
	// NS::UInteger threadGroupSize = this->_mAddFunctionPso->maxTotalThreadsPerThreadgroup();
    // std::cout << "Max totalthreads per thread group: " << threadGroupSize << std::endl;


	MTL::Size threadgroupSize = MTL::Size::Make(8, 8, 1);
	// std::cout << "threadGroupSize " << threadgroupSize.width << " " << threadgroupSize.height << " " << threadgroupSize.depth << std::endl;
    
    
	computeCommandEncoder->dispatchThreads(gridSize, threadgroupSize);
	computeCommandEncoder->endEncoding();

	commandBuffer->commit();
	// std::cout << "waiting\n";
	commandBuffer->waitUntilCompleted();

    
    // for blitting process
    MTL::CommandBuffer* commandBuffer1 = _mCommandQueue->commandBuffer();
    // std::cout << "Command buffers address: " << commandBuffer << std::endl;

    if (commandBuffer1 == nullptr) {
        std::cerr << "Error: failed to create command buffer\n";
        return; // maybe throw error here instead
    }
    
        CA::MetalDrawable* drawable = this->swapchain->nextDrawable();
        MTL::Texture* drawableTexture = drawable->texture();
    // std::cout << "drawable texture pixel format: " << static_cast<int>(drawableTexture->pixelFormat()) << std::endl;
    MTL::BlitCommandEncoder* blit = commandBuffer1->blitCommandEncoder();
    blit->copyFromTexture(this->_outputText,
                          0, 0, {0, 0, 0},
                          {(unsigned int)camData.imageW, (unsigned int)camData.imageH, 1},
                          drawableTexture,
                          0, 0, {0, 0, 0});
    blit->endEncoding();
    
    commandBuffer1->presentDrawable(drawable);
    commandBuffer1->commit();
    commandBuffer1->waitUntilCompleted();
}

// NOTE:: DOES NOT HANDLE FOOKINNG UPDATING OBJECTS YET
void MetalAdder::updateVariables(
	struct CameraData& camData,
	const Object3D* objects,
	const uint& objectAmount,
	const uint& bounces
) {
    
    if (this->_mDevice == nullptr) {
        std::cerr << "Error: device not created before running updateVariables\n";
        return;
    }
	if (this->_camData == nullptr) {
		this->_camData = this->_mDevice->newBuffer(sizeof(camData), MTL::ResourceStorageModeShared);
    }
    memcpy(this->_camData->contents(), &camData, sizeof(CameraData)); // maybe breaking point
    // std::cout << "updated camdata\n";
	if (this->_objects == nullptr) {
		this->_objects = this->_mDevice->newBuffer(sizeof(Object3D) * objectAmount, MTL::ResourceStorageModeShared);
		memcpy(this->_objects->contents(), objects, sizeof(Object3D) * objectAmount); // maybe breaking point
		// std::cout << "updated objects\n";
	}
	if (this->_objectAmount == nullptr) {
		this->_objectAmount = this->_mDevice->newBuffer(sizeof(uint), MTL::ResourceStorageModeShared);
		memcpy(this->_objectAmount->contents(), &objectAmount, sizeof(uint)); // maybe breaking point
		// std::cout << "updated objectamount\n";
	}
	if (this->_bounces == nullptr) {
		this->_bounces = this->_mDevice->newBuffer(sizeof(uint), MTL::ResourceStorageModeShared);
		memcpy(this->_bounces->contents(), &bounces, sizeof(uint)); // maybe breaking point
		// std::cout << "updated bounces\n";
	}
    
    
    // temp
    if (this->_outputText == nullptr) {
        MTL::TextureDescriptor* desc = MTL::TextureDescriptor::texture2DDescriptor(MTL::PixelFormatBGRA8Unorm, camData.imageW, camData.imageH, false);
        desc->setUsage(MTL::TextureUsageShaderRead | MTL::TextureUsageShaderWrite);
        
        this->_outputText = this->_mDevice->newTexture(desc);
        desc->release();
    }
}
