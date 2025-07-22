
#ifndef METALADDER_CLASS_HPP
# define METALADDER_CLASS_HPP

# include <iostream>
# include "Camera.hpp"
# include "RayTracingProgramStructs.h"

# include <Foundation/Foundation.hpp>
# include <Metal/Metal.hpp>
# include <QuartzCore/CAMetalLayer.hpp>



class MetalAdder {
private:
    CA::MetalLayer*             swapchain;
	MTL::Device*                _mDevice;
	MTL::ComputePipelineState*  _mAddFunctionPso;
	MTL::CommandQueue*	        _mCommandQueue;

	MTL::Buffer*	_metaData;
	MTL::Buffer*	_objects;
	MTL::Buffer*	_bvhNodes;
	MTL::Buffer*	_objectsData;
    MTL::Texture*   _outputText; // need to figure out how does

public:
	MetalAdder(CA::MetalLayer* metalLayer, const int& imageW, const int& imageH);
	
	void sendComputeCommand(
		const RaytracingMetaData& metaData,
		const Object3D* objects,
		const BVHNode* bvhNodes,
		const ObjectsData& objectsData
	);
	void updateVariables(
		const RaytracingMetaData& metaData,
		const Object3D* objects,
		const BVHNode* bvhNodes,
		const ObjectsData& objectsData
	);

};

#endif
