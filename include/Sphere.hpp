
#ifndef SPHERE_CLASS_HPP
# define SPHERE_CLASS_HPP 

# include <Object3D.hpp>
# include <Interval.hpp>
# include <Vec3.hpp>
# include <cmath>

class Sphere: public Object3D {
public:
	Sphere(const Vec3& center, const float& radius, shared_ptr<Material> mat);
	bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override;

private:
	Vec3						_center;
	float						_radius;
	std::shared_ptr<Material>	_mat;
};

#endif /* SPHERE_CLASS_HPP */
