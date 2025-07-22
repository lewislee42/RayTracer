
#ifndef CAMERA_CLASS_HPP
# define CAMERA_CLASS_HPP 

#include <SDL3/SDL.h>

#include "Vec3.hpp"

#include "RayTracingSharedStructs.h"
#include "RayTracingProgramStructs.h"

#include <chrono>
#include <ctime>
#include <vector>


#define TURN_SPEED 1
#define MOVE_SPEED 11

class MetalAdder;

typedef struct CameraMovementBools {
	bool keyW			= false;
    bool keyS			= false;
    bool keyA			= false;
    bool keyD			= false;
    bool keyMoveUp		= false;
    bool keyMoveDown	= false;

    bool keyTurnLeft	= false;
    bool keyTurnRight	= false;
    bool keyTurnUp		= false;
    bool keyTurnDown	= false;
}	CameraMovementBools;


class Camera {
public:
    uint	screenWidth				= 800;
	double	aspectRatio				= 16.0 / 9.0;
	uint	samplePerPixel			= 10;
	uint	lightSampleAmount		= 1;
	uint	maxBounces				= 10;
	double	vfov			        = 90;
	Vec3	center	                = (Vec3){0.0f, 0.0f, 0.0f};
	Vec3	direction		        = (Vec3){0.0f, 0.0f, -1.0f};
	Vec3	vup				        = (Vec3){0.0f, 1.0f, 0.0f}; // Camera relative "up" direction

	Camera(
        uint            screenWidth,
        double          aspectRatio,
        uint            samplePerPixel,
        uint            maxBounces,
		uint			lightSampleAmount,
        uint			vfov,
        Vec3            lookFrom,
        Vec3            lookAt,
        Vec3            vup
    );

	void	updateMovementBools(const SDL_Event event);
	void	updateCameraPosDir(float deltaTime);
	void	updateCameraData();
	RaytracingMetaData	getMetaData() const;

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



};


#endif /* CAMERA_CLASS_HPP */
