
#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include <MetalAdder.hpp>

// initializes the metal shader function
MetalAdder::MetalAdder(MTL::Device *d, const int& imageW, const int& imageH) {
	this->_mDevice = d;

	MTL::Library* defaultLibrary = this->_mDevice->newDefaultLibrary();
	if (defaultLibrary == nullptr) {
		std::cerr << "Error: could not find default library\n";
		return; // maybe throw error here instead
	}

	NS::String* str = NS::String::string("rayColorTest", NS::ASCIIStringEncoding); 
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

	this->_camData = nullptr;
	this->_objects = nullptr;
	this->_objectAmount = nullptr;
	this->_bounces = nullptr;
	this->_pixels = nullptr;
}

void MetalAdder::sendComputeCommand(
	struct CameraData& camData,
	const Object3D* objects,
	const uint& objectAmount,
	const uint& bounces,
	uint8_t* pixels
) {
	this->updateVariables(camData, objects, objectAmount, bounces, pixels);

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
	computeCommandEncoder->setBuffer(this->_camData, 0, 0);
	computeCommandEncoder->setBuffer(this->_objects, 0, 1);
	computeCommandEncoder->setBuffer(this->_objectAmount, 0, 2);
	computeCommandEncoder->setBuffer(this->_bounces, 0, 3);
	computeCommandEncoder->setBuffer(this->_pixels, 0, 4);

	MTL::Size gridSize = MTL::Size::Make(camData.imageW, camData.imageH, 1);
	NS::UInteger threadGroupSize = this->_mAddFunctionPso->maxTotalThreadsPerThreadgroup();

	if (threadGroupSize > camData.imageW)
		threadGroupSize = camData.imageW;


	MTL::Size threadgroupSize = MTL::Size::Make(8, 8, 1);
	computeCommandEncoder->dispatchThreads(gridSize, threadgroupSize);
	computeCommandEncoder->endEncoding();

	commandBuffer->commit();
	std::cout << "waiting\n";
	commandBuffer->waitUntilCompleted();

	uint pixelAmount = (sizeof(uint8_t) * camData.imageW * camData.imageH) * 3;
	uint8_t* pixelData = static_cast<uint8_t*>(this->_pixels->contents());
	/*for (size_t i = 0; i < pixelAmount; ++i) {*/
	/*	std::cout << "Pixel" << i << ": " << static_cast<int>(pixelData[i]);*/
	/*	if (i % 3 == 0)*/
	/*		std::cout << "\n";*/
	/*}*/
}

// NOTE:: DOES NOT HANDLE FOOKINNG UPDATING OBJECTS YET
void MetalAdder::updateVariables(
	struct CameraData& camData,
	const Object3D* objects,
	const uint& objectAmount,
	const uint& bounces,
	uint8_t* pixels
) {
	if (this->_camData == nullptr) {
		this->_camData = this->_mDevice->newBuffer(sizeof(camData), MTL::ResourceStorageModeShared);
		memcpy(this->_camData->contents(), &camData, sizeof(CameraData)); // maybe breaking point
		std::cout << "updated camdata\n";
	}
	if (this->_objects == nullptr) {
		this->_objects = this->_mDevice->newBuffer(sizeof(Object3D) * objectAmount, MTL::ResourceStorageModeShared);
		memcpy(this->_objects->contents(), objects, objectAmount); // maybe breaking point
		std::cout << "updated objects\n";
	}
	if (this->_objectAmount == nullptr) {
		this->_objectAmount = this->_mDevice->newBuffer(sizeof(uint), MTL::ResourceStorageModeShared);
		memcpy(this->_objectAmount->contents(), &objectAmount, sizeof(uint)); // maybe breaking point
		std::cout << "updated objectamount\n";
	}
	if (this->_bounces == nullptr) {
		this->_bounces = this->_mDevice->newBuffer(sizeof(uint), MTL::ResourceStorageModeShared);
		memcpy(this->_bounces->contents(), &bounces, sizeof(uint)); // maybe breaking point
		std::cout << "updated bounces\n";
	}
	if (this->_pixels == nullptr) {
		this->_pixels = this->_mDevice->newBuffer(sizeof(uint8_t) * 3 * camData.imageW * camData.imageH, MTL::ResourceStorageModeShared);
		memcpy(this->_pixels->contents(), pixels, (sizeof(uint8_t) * camData.imageW * camData.imageH) * 3); // maybe breaking point
		std::cout << "updated pixels\n";
	}
}
