
#include <RayTracingStructs.hpp>
#include <Vec3.hpp>

Vec3 at(const Ray& r, const double& t) {
	return r.origin + t * r.direction;
}
