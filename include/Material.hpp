
#ifndef MATERIAL_ABSTRACT_CLASS_HPP
# define MATERIAL_ABSTRACT_CLASS_HPP

#include <Object3D.hpp>

class Material {
public:
	virtual ~Material() = default;

	virtual bool scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const;
};


class Lambertian: public Material {
public:
	Lambertian(const Vec3& albedo);

	bool scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override;

private:
	Vec3 _albedo;
};


class Metal: public Material {
public:
	Metal(const Vec3& albedo, float fuzz);

	bool scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override;

private:
	Vec3	_albedo;
	float	_fuzz;
};


class Dielectric: public Material {
public:
	Dielectric(float refractionIndex);

	bool scatter(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override;

private:
	float	_refractionIndex;
};

#endif /* MATERIAL_ABSTRACT_CLASS_HPP */
