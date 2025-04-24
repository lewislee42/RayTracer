
#ifndef RAYTRACING_CLASS_HPP 
# define RAYTRACING_CLASS_HPP 

# include <SDLObject.hpp>
# include <SDLTextureObject.hpp>

# include <Camera.hpp>


# include <Utils.hpp>

# include <RayTracingStructs.hpp>

// Object3D
bool hit(const Object3D& obj, const Ray& r, Interval rayT, HitRecord* rec);

// Materials
void setFaceNormal(HitRecord* hitRecord, const Ray&r, const Vec3& outwardNormal);
bool scatterLambertian(const Material& mat, const Ray& rIn, const HitRecord& rec, Vec3* attenuation, Ray* scattered);
bool scatterMetal(const Material& mat, const Ray& rIn, const HitRecord& rec, Vec3* attenuation, Ray* scattered);
bool scatterDielectric(const Material& mat, const Ray& rIn, const HitRecord& rec, Vec3* attenuation, Ray* scattered);
bool scatter(const Material& obj, const Ray& rIn, const HitRecord& rec, Vec3* attenuation, Ray* scattered);

// Interval
float	clamp(float x, float min, float max);


Vec3 at(const Ray& r, const double& t);




#endif /* RAYTRACING_CLASS_HPP */
