
# include <Sphere.hpp>

Sphere::Sphere(const Vec3& center, const double& radius, shared_ptr<Material> mat): _center(center), _radius(std::fmax(0.0, radius)), _mat(mat) {
}

bool Sphere::hit(const Ray& r, Interval rayT, HitRecord& rec) const {
	Vec3 oc = this->_center - r.getOrigin();
	double a = r.getDirection().lengthSquared();
	double b = dot(r.getDirection(), oc);
	double c = oc.lengthSquared() - this->_radius * this->_radius;
	double descriminant = b * b - a * c;

	if (descriminant < 0)
		return false;

	double sqrtD = std::sqrt(descriminant);
	double root = (b - sqrtD) / a;
	if (!rayT.surrounds(root)) {
		root = (b + sqrtD) / a;
		if (!rayT.surrounds(root))
			return false;
	}

	rec.t = root;
	rec.p = r.at(rec.t);
	rec.normal = (rec.p - this->_center) / this->_radius;

	Vec3 outwardNormal = (rec.p - this->_center) / this->_radius;
	rec.setFaceNormal(r, outwardNormal);

	rec.mat = this->_mat;
	
	return true;
}
