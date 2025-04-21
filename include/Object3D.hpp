
#ifndef OBJECT3D_ABSTRACT_CLASS_HPP
# define OBJECT3D_ABSTRACT_CLASS_HPP 

# include <Ray.hpp>
# include <Vec3.hpp>
# include <Material.hpp>
# include <Interval.hpp>


// only supports sphere for now
typedef struct Object3D {
	Vec3			center;
	float			radius;
	struct Material	mat;
}	Object3D;

bool hit(const Object3D& obj, const Ray& r, Interval rayT, HitRecord* rec);

#endif /* OBJECT3D_ABSTRACT_CLASS_HPP */
