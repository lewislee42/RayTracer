
# include <Material.hpp>

bool Material::scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
	return false;
}




// Lambertian
Lambertian::Lambertian(const Vec3& albedo): _albedo(albedo) {}

bool Lambertian::scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
	Vec3 scatterDirection = rec.normal + randomUnitVector();

	if (scatterDirection.nearZero())
		scatterDirection = rec.normal;

	scattered = Ray(rec.p, scatterDirection);
	attenuation = this->_albedo;
	return true;
}



// Metal
Metal::Metal(const Vec3& albedo, float fuzz): _albedo(albedo), _fuzz(fuzz < 1 ? fuzz : 1) {}

bool Metal::scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
	Vec3 reflected = reflect(rIn.getDirection(), rec.normal);
	reflected = unitVector(reflected) + (this->_fuzz * randomUnitVector());
	scattered = Ray(rec.p, reflected);
	attenuation = this->_albedo;

	return (dot(scattered.getDirection(), rec.normal) > 0);
}



// Dielectric
Dielectric::Dielectric(float refractionIndex): _refractionIndex(refractionIndex) {}

bool Dielectric::scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
	attenuation = Vec3(1.0, 1.0, 1.0);
	float ri;
	if (rec.frontFace)
		ri = 1.0 / this->_refractionIndex;
	else
		ri = this->_refractionIndex;

	Vec3 unitDirection = unitVector(rIn.getDirection());
	float cosTheta = std::fmin(dot(-unitDirection, rec.normal), 1.0);
	float sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

	bool cannotRefract = ri * sinTheta > 1.0;
	Vec3 direction;

	if (cannotRefract)
		direction = reflect(unitDirection, rec.normal);
	else
		direction = refract(unitDirection, rec.normal, ri);
		

	scattered = Ray(rec.p, direction);
	return true;
}
