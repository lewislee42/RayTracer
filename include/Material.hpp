
#ifndef MATERIAL_ABSTRACT_CLASS_HPP
# define MATERIAL_ABSTRACT_CLASS_HPP

# include <Vec3.hpp>
# include <Ray.hpp>

typedef enum MaterialType {
	LAMBERTIAN	= 0,
	METAL		= 1,
	DIELECTRIC	= 2
}	MaterialType;

typedef struct Material {
	MaterialType	type;
	Vec3			albedo;
	float			fuzz;
	float			refractionIndex;
}	Material;

typedef struct HitRecord {
	Vec3			p;
	Vec3			normal;
	struct Material	mat;
	float			t;
	bool			frontFace;
}	HitRecord;

void setFaceNormal(HitRecord* hitRecord, const Ray&r, const Vec3& outwardNormal);
bool scatterLambertian(const Material& mat, const Ray& rIn, const HitRecord& rec, Vec3* attenuation, Ray* scattered);
bool scatterMetal(const Material& mat, const Ray& rIn, const HitRecord& rec, Vec3* attenuation, Ray* scattered);
bool scatterDielectric(const Material& mat, const Ray& rIn, const HitRecord& rec, Vec3* attenuation, Ray* scattered);
bool scatter(const Material& obj, const Ray& rIn, const HitRecord& rec, Vec3* attenuation, Ray* scattered);

#endif /* MATERIAL_ABSTRACT_CLASS_HPP */
