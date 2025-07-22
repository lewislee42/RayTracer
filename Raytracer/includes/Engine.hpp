//
//  Engine.hpp
//  Raytracer
//
//  Created by Lewis lee on 22/07/2025.
//

#ifndef Engine_hpp
#define Engine_hpp

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>

#include <SDL3/SDL.h>

#include <stdio.h>
#include <vector>

#include "RayTracingSharedStructs.h"
#include "BVH.h"
#include "Camera.hpp"
#include "MetalAdder.hpp"

class Engine {
private:
	unsigned int	screenWidth		= 800;
	unsigned int	screenHeight	= 400;
	double			aspectRatio		= 16.0 / 9.0;
	
	SDL_Window*		window			= NULL;
	SDL_Renderer*	renderer		= NULL;
	SDL_Surface*	surface			= NULL;
	CA::MetalLayer* swapchain		= NULL;
	
	MetalAdder*		metalAdder		= NULL;
	
	SDL_Event		event = {0};
	
	float			deltaTime		= 0.0f;
	
	std::vector<Object3D>	objBuffer;
	std::vector<BVHNode>	bvhNodeBuffer;
	ObjectsData 			objData;
	
	Camera					camera;
	
	void	render();

public:
	Engine(unsigned int screenWidth, unsigned int screenHeight, double aspectRatio);
	~Engine();
	
	void	run();

	class SDLInitializationError: public std::exception {
	public:
		const char* what() const throw();
	};
};

#endif /* Engine_hpp */
