
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

	void	render(const Object3D& world, SDLTextureObject& texture);
	int		getHeight() const;

private:
	int		_imageH;
	double	_pixelSampleWeight;
	Vec3	_center;
	Vec3	_pixel00Loc;
	Vec3	_pixelDeltaU;
	Vec3	_pixelDeltaV;

	void	initialize();
	Vec3	rayColor(const Ray& r, int depth, const Object3D& world) const;
	Vec3	sampleSquare() const;
	Ray		getRay(int i, int j) const;
};


#endif /* CAMERA_CLASS_HPP */
