
#ifndef CAMERA_CLASS_HPP
# define CAMERA_CLASS_HPP 

# include <SDL3/SDL.h>

# include "Vec3.hpp"
# include <vector>


# include <Foundation/Foundation.hpp>
# include <Metal/Metal.hpp>
# include <QuartzCore/CAMetalLayer.hpp>

# include "RayTracingSharedStructs.h"
# include "RayTracingProgramStructs.h"

#include <chrono>
#include <ctime>
#include <iomanip> // For formatting

class MetalAdder;

typedef struct CameraMovementBools {
	bool keyW = false;
    bool keyS = false;
    bool keyA = false;
    bool keyD = false;
    bool keyMoveUp = false;
    bool keyMoveDown = false;

    bool keyTurnLeft = false;
    bool keyTurnRight = false;
    bool keyTurnUp = false;
    bool keyTurnDown = false;
}	CameraMovementBools;

class Camera {
public:
    uint	screenWidth				= 800;
	double	aspectRatio				= 16.0 / 9;
	uint	samplePerPixel			= 10;
	uint	lightSampleAmount		= 1;
	uint	maxBounces				= 10;
	double	vfov			        = 90;
	Vec3	center	                = (Vec3){0, 0, 0};
	Vec3	direction		        = (Vec3){0, 0, -1};
	Vec3	vup				        = (Vec3){0, 1, 0}; // Camera relative "up" direction
    
    CA::MetalLayer* swapchain;

	Camera(
        uint            screenWidth,
        double          aspectRatio,
        uint            samplePerPixel,
        uint            maxBounces,
		uint			lightSampleAmount,
        uint			vfov,
        Vec3            lookFrom,
        Vec3            lookAt,
        Vec3            vup,
        SDL_Renderer*   renderer
    );

	void	render(const ObjectsData& objData, const Object3D* objects, const BVHNode* bvhNodes);
	void	updateMovementBools(const SDL_Event event);
	void	updateCamera();

private:

	uint		_screenHeight;
	double		_pixelSampleWeight;
	Vec3		_pixel00Loc;
	Vec3		_pixelDeltaU;
	Vec3		_pixelDeltaV;
	Vec3		_u;
	Vec3		_v;
	Vec3		_w;
	
	
	CameraMovementBools movementBools;

	MetalAdder*		_metalAdder;

    void		initialize();
    public:
	RaytracingMetaData	getMetaData() const;
};


#endif /* CAMERA_CLASS_HPP */
