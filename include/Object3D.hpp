
#ifndef OBJECT3D_ABSTRACT_CLASS_HPP
# define OBJECT3D_ABSTRACT_CLASS_HPP 

# include <Ray.hpp>
# include <Vec3.hpp>
# include <Interval.hpp>

class HitRecord {
public:
	Vec3	p;
	Vec3	normal;
	double		t;
	bool		frontFace;

	void setFaceNormal(const Ray& r, const Vec3& outwardNormal);
};

class Object3D {
public:
	virtual ~Object3D() = default;

	virtual bool hit(const Ray& r, Interval rayT, HitRecord& rec) const = 0;
};

#endif /* OBJECT3D_ABSTRACT_CLASS_HPP */
