
#include <RayTracingStructs.hpp>
# include <RayTracing.hpp>

bool hit(const Object3D& obj, const Ray& r, Interval rayT, HitRecord* rec) {
	Vec3 oc = obj.center - r.origin;
	float a = lengthSquared(r.direction);
	float b = dot(r.direction, oc);
	float c = lengthSquared(oc) - obj.radius * obj.radius;
	float descriminant = b * b - a * c;

	if (descriminant < 0)
		return false;

	float sqrtD = std::sqrt(descriminant);
	float root = (b - sqrtD) / a;
	if (!(rayT.min < root && root < rayT.max)) {
		root = (b + sqrtD) / a;
		if (!(rayT.min < root && root < rayT.max))
			return false;
	}

	rec->t = root;
	rec->p = at(r, rec->t);
	rec->normal = (rec->p - obj.center) / obj.radius;

	Vec3 outwardNormal = (rec->p - obj.center) / obj.radius;
	setFaceNormal(rec, r, outwardNormal);

	rec->mat = obj.mat;
	
	return true;
}

