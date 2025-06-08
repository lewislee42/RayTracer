
#ifndef METALADDER_CLASS_HPP
# define METALADDER_CLASS_HPP

# include <iostream>
# include <Camera.hpp>

# include <Foundation/Foundation.hpp>
# include <Metal/Metal.hpp>
# include <QuartzCore/CAMetalLayer.hpp>


class MetalAdder {
private:
    CA::MetalLayer*             swapchain;
	MTL::Device*                _mDevice;
	MTL::ComputePipelineState*  _mAddFunctionPso;
	MTL::CommandQueue*	        _mCommandQueue;

	MTL::Buffer*	_camData;
	MTL::Buffer*	_objects;
	MTL::Buffer*	_objectAmount;
	MTL::Buffer*	_bounces;
    MTL::Texture*   _outputText; // need to figure out how does

public:
	MetalAdder(CA::MetalLayer* metalLayer, const int& imageW, const int& imageH);
	
	void sendComputeCommand(
		struct CameraData& camData,
		const Object3D* objects,
		const uint& objectAmount,
		const uint& bounces
	);
	void updateVariables(
		struct CameraData& camData,
		const Object3D* objects,
		const uint& objectAmount,
		const uint& bounces
	);

};

#endif
