
#ifndef MATERIAL_ABSTRACT_CLASS_HPP
# define MATERIAL_ABSTRACT_CLASS_HPP

# include <Vec3.hpp>
# include <Ray.hpp>

bool scatterLambertian(const Material& mat, const Ray& rIn, const HitRecord& rec, Vec3* attenuation, Ray* scattered);
bool scatterMetal(const Material& mat, const Ray& rIn, const HitRecord& rec, Vec3* attenuation, Ray* scattered);
bool scatterDielectric(const Material& mat, const Ray& rIn, const HitRecord& rec, Vec3* attenuation, Ray* scattered);
bool scatter(const Material& obj, const Ray& rIn, const HitRecord& rec, Vec3* attenuation, Ray* scattered);

#endif /* MATERIAL_ABSTRACT_CLASS_HPP */
