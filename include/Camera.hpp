
#ifndef CAMERA_CLASS_HPP
# define CAMERA_CLASS_HPP 

# include <SDLTextureObject.hpp>
# include <Object3D.hpp>
# include <Vec3.hpp>
# include <Utils.hpp>
# include <Material.hpp>
# include <vector>

class Camera {
public:
	double	aspectRatio		= 16.0 / 9;
	int		imageW			= 800;
	int		samplePerPixel	= 10;
	int		maxDepth		= 10;
	double	fov				= 90;
	Vec3	lookFrom		= Vec3(0, 0, 0);
	Vec3	lookAt			= Vec3(0, 0, -1);
	Vec3	vup				= Vec3(0, 1, 0); // Camera relative "up" direction

	void	render(const Object3D& world, SDLTextureObject& texture);
	int		getHeight() const;

private:
	int		_imageH;
	double	_pixelSampleWeight;
	Vec3	_center;
	Vec3	_pixel00Loc;
	Vec3	_pixelDeltaU;
	Vec3	_pixelDeltaV;
	Vec3	_u;
	Vec3	_v;
	Vec3	_w;

	void	initialize();
	Vec3	rayColor(const Ray& r, int depth, const Object3D& world) const;
	Vec3	sampleSquare() const;
	Ray		getRay(int i, int j) const;
};


#endif /* CAMERA_CLASS_HPP */
