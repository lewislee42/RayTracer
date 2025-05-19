
#ifndef CAMERA_CLASS_HPP
# define CAMERA_CLASS_HPP 

# include <SDL3/SDL.h>

# include <Vec3.hpp>
# include <vector>


# include <Foundation/Foundation.hpp>
# include <Metal/Metal.hpp>
# include <QuartzCore/CAMetalLayer.hpp>
# include <RayTracingStructs.hpp>

#include <chrono>
#include <ctime>
#include <iomanip> // For formatting

class MetalAdder;

class Camera {
public:
    int     screenWidth             = 800;
	double	aspectRatio		        = 16.0 / 9;
	uint		samplePerPixel      = 10;
	uint     lightSampleAmount      = 1;
	uint		maxBounces          = 10;
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
        int             vfov,
        Vec3            lookFrom,
        Vec3            lookAt,
        Vec3            vup,
        SDL_Renderer*   renderer
    );

	void	render(const std::vector<Object3D> world);

private:
	int			_screenHeight;
	int			_frameCount;
	double		_pixelSampleWeight;
	Vec3		_pixel00Loc;
	Vec3		_pixelDeltaU;
	Vec3		_pixelDeltaV;
	Vec3		_u;
	Vec3		_v;
	Vec3		_w;

	MetalAdder*		_metalAdder;

    void	initialize();
    public:
	CameraData	getCameraData() const;
};


#endif /* CAMERA_CLASS_HPP */
