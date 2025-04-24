
#ifndef CAMERA_CLASS_HPP
# define CAMERA_CLASS_HPP 

# include <SDLTextureObject.hpp>
# include <Vec3.hpp>
# include <Utils.hpp>
# include <vector>


# include <Foundation/Foundation.hpp>
# include <Metal/Metal.hpp>
# include <RayTracingStructs.hpp>

class MetalAdder;

class Camera {
public:
	double	aspectRatio		= 16.0 / 9;
	int		imageW			= 800;
	int		samplePerPixel	= 10;
	int		maxDepth		= 10;
	double	fov				= 90;
	Vec3	lookFrom		= (Vec3){0, 0, 0};
	Vec3	lookAt			= (Vec3){0, 0, -1};
	Vec3	vup				= (Vec3){0, 1, 0}; // Camera relative "up" direction

	Camera();

	void	render(const std::vector<Object3D> world, SDLTextureObject& texture);
	int		getHeight() const;

private:
	int			_imageH;
	double		_pixelSampleWeight;
	Vec3		_center;
	Vec3		_pixel00Loc;
	Vec3		_pixelDeltaU;
	Vec3		_pixelDeltaV;
	uint8_t*	_pixels;
	Vec3		_u;
	Vec3		_v;
	Vec3		_w;

	MTL::Device*	_device;
	MetalAdder*		_metalAdder;

	void	initialize();
	Vec3	rayColor(const Ray& r, int depth, const std::vector<Object3D>& world, const int& objectAmount) const;
	Vec3	sampleSquare() const;
	Ray		getRay(int i, int j) const;

	CameraData	getCameraData() const;
};


#endif /* CAMERA_CLASS_HPP */
