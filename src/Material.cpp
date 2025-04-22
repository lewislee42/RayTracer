
# include <Material.hpp>
# include <Object3D.hpp>


void setFaceNormal(HitRecord* hitRecord, const Ray&r, const Vec3& outwardNormal) {
	hitRecord->frontFace = dot(r.direction, outwardNormal) < 0;
	if (hitRecord->frontFace)
		hitRecord->normal = outwardNormal;
	else
		hitRecord->normal = outwardNormal * -1;
}

// Lambertian
bool scatterLambertian(const Material& mat, const Ray& rIn, const HitRecord& rec, Vec3* attenuation, Ray* scattered) {
	/*std::cout << "dount: " << rec.p << std::endl;*/
    Vec3 scatterDirection = rec.normal + randomUnitVector(rIn.direction);

    if (nearZero(scatterDirection))
        scatterDirection = rec.normal;

    *scattered = (Ray){rec.p, scatterDirection};
    *attenuation = mat.albedo;
    return true;
}

// Metal
bool scatterMetal(const Material& mat, const Ray& rIn, const HitRecord& rec, Vec3* attenuation, Ray* scattered) {
    Vec3 reflected = reflect(rIn.direction, rec.normal);
    reflected = unitVector(reflected) + (mat.fuzz * randomUnitVector(rIn.direction));
    *scattered = (Ray){rec.p, reflected};
    *attenuation = mat.albedo;

    return (dot(scattered->direction, rec.normal) > 0);
}

// Dielectric
bool scatterDielectric(const Material& mat, const Ray& rIn, const HitRecord& rec, Vec3* attenuation, Ray* scattered) {
    *attenuation = (Vec3){1.0, 1.0, 1.0};
    float ri;
    if (rec.frontFace)
        ri = 1.0 / mat.refractionIndex;
    else
        ri = mat.refractionIndex;

    Vec3 unitDirection = unitVector(rIn.direction);
    float cosTheta = std::fmin(dot(unitDirection * -1, rec.normal), 1.0);
    float sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

    bool cannotRefract = ri * sinTheta > 1.0;
    Vec3 direction;

    if (cannotRefract)
        direction = reflect(unitDirection, rec.normal);
    else
        direction = refract(unitDirection, rec.normal, ri);
        

    *scattered = (Ray){rec.p, direction};
    return true;
}

bool scatter(const Material& mat, const Ray& rIn, const HitRecord& rec, Vec3* attenuation, Ray* scattered) {
	switch (mat.type) {
		case MaterialType::LAMBERTIAN:
			return scatterLambertian(mat, rIn, rec, attenuation, scattered);
		case MaterialType::METAL:
			return scatterMetal(mat, rIn, rec, attenuation, scattered);
		case MaterialType::DIELECTRIC:
			return scatterDielectric(mat, rIn, rec, attenuation, scattered);
		default:
			return true;
	}
}

