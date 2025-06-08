
#ifndef METALADDER_CLASS_HPP
# define METALADDER_CLASS_HPP

# include <iostream>
# include <Camera.hpp>

# include <Foundation/Foundation.hpp>
# include <Metal/Metal.hpp>


class MetalAdder {
private:
	MTL::Device* _mDevice;
	MTL::ComputePipelineState* _mAddFunctionPso;
	MTL::CommandQueue*	_mCommandQueue;

	MTL::Buffer*	_camData;
	MTL::Buffer*	_objects;
	MTL::Buffer*	_objectAmount;
	MTL::Buffer*	_bounces;
	MTL::Buffer*	_pixels;

public:
	MetalAdder(MTL::Device* d, const int& imageW, const int& imageH);
	
	void sendComputeCommand(
		struct CameraData& camData,
		const Object3D* objects,
		const uint& objectAmount,
		const uint& bounces,
		uint8_t* pixels
	);
	void updateVariables(
		struct CameraData& camData,
		const Object3D* objects,
		const uint& objectAmount,
		const uint& bounces,
		uint8_t* pixels
	);

};

#endif
