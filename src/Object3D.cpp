
# include <Object3D.hpp>


void HitRecord::setFaceNormal(const Ray&r, const Vec3& outwardNormal) {
	this->frontFace = dot(r.getDirection(), outwardNormal) < 0;
	if (frontFace)
		this->normal = outwardNormal;
	else
		this->normal = -outwardNormal;
}
