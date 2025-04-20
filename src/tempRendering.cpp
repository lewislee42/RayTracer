

#ifndef SHADERTYPE_H
#define SHADERTYPE_H

#include <simd/simd.h>
#include <limits>

using Vec3 = simd::double3;
using Vec2 = simd::double2;

typedef enum {
	LAMBERTIAN,
	METAL,
	DIELECTRIC
}	MaterialType;

typedef struct {
	// material stuff
	MaterialType	matType;

	// for metal & lambertian
	Vec3 albedo;
	// for metal
	Vec3 fuzz;

	// for dielectric
	double refractionIndex;
}	Material;

typedef struct {
	double min;
	double max;
}	Interval;

typedef struct {
	Vec3		p;
	Vec3		normal;
	Material	mat;
	double		t;
	bool		frontFace;
}	HitRecord;

typedef struct {
	// sphere stuff
	simd::double3	center;
	double			radius;

	Material		mat;
}	Object3D;

typedef struct {
	Vec3 origin;
	Vec3 direction;
}	Ray;


#endif /* SHADERTYPE_H */


// Vec/Math helper functions
inline double dot(const Vec3& u, const Vec3& v) {
	return u.x * v.x
         + u.y * v.y
         + u.z * v.z;	
}

inline double dot(const Vec2& u, const Vec2& v) {
	return u.x * v.x
         + u.y * v.y;	
}

inline double randomDouble(double min, double max) {
    return min + (min - max) * simd::fract(sin(dot((Vec2){min, max}, (Vec2){12.9898, 78.233})) * 43758.5453);
}

inline Vec3 at(Vec3 o, Vec3 direction, double t) {
	return o + t * direction;
}

inline double lengthSquared(const Vec3& v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline double length(const Vec3& v) {
	return std::sqrt(lengthSquared(v));
}

inline Vec3 unitVector(const Vec3& v) {
    return v / length(v);
}

inline Vec3 randomVector(double min, double max) {
	return (Vec3){randomDouble(min, max), randomDouble(min, max), randomDouble(min, max)};
}

inline Vec3 randomUnitVector() {
	while (true) {
		Vec3 p = randomVector(-1, 1);
		double lensq = lengthSquared(p);
		if (1e-160 < lensq && lensq <= 1)
			return p / sqrt(lensq);
	}
}



bool scatterLambertian(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) {
	Vec3 scatterDirection = rec.normal + randomUnitVector();

	if (scatterDirection.nearZero())
		scatterDirection = rec.normal;

	scattered = Ray(rec.p, scatterDirection);
	attenuation = this->_albedo;
	return true;
}

// Metal
bool scatterMetal(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) {
	Vec3 reflected = reflect(rIn.getDirection(), rec.normal);
	reflected = unitVector(reflected) + (this->_fuzz * randomUnitVector());
	scattered = Ray(rec.p, reflected);
	attenuation = this->_albedo;

	return (dot(scattered.getDirection(), rec.normal) > 0);
}

// Dielectric
bool scatterDielectric(const Ray& rIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) {
	attenuation = (Vec3){1.0, 1.0, 1.0};
	double ri;
	if (rec.frontFace)
		ri = 1.0 / this->_refractionIndex;
	else
		ri = this->_refractionIndex;

	Vec3 unitDirection = unitVector(rIn.direction);
	double cosTheta = std::fmin(dot(-unitDirection, rec.normal), 1.0);
	double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

	bool cannotRefract = ri * sinTheta > 1.0;
	Vec3 direction;

	if (cannotRefract)
		direction = reflect(unitDirection, rec.normal);
	else
		direction = refract(unitDirection, rec.normal, ri);
		

	scattered = Ray(rec.p, direction);
	return true;
}


// Raytracing part
bool hit(const Object3D object, const Ray& r, Interval rayT, HitRecord& rec) {
	Vec3 oc = object.center - r.origin;
	double a = lengthSquared(r.direction);
	double b = dot(r.direction, oc);
	double c = lengthSquared(oc) - object.radius * object.radius;
	double descriminant = b * b - a * c;

	if (descriminant < 0)
		return false;

	double sqrtD = std::sqrt(descriminant);
	double root = (b - sqrtD) / a;
	if (!(rayT.min < root & root < rayT.max)) {
		root = (b + sqrtD) / a;
		if (!(rayT.min < root & root < rayT.max))
			return false;
	}

	rec.t = root;
	rec.p = at(r.origin, r.direction, rec.t);
	rec.normal = (rec.p - object.center) / object.radius;

	Vec3 outwardNormal = (rec.p - object.center) / object.radius;
	
	rec.frontFace = dot(r.direction, outwardNormal) < 0;
	if (rec.frontFace)
		rec.normal = outwardNormal;
	else
		rec.normal = -outwardNormal;

	rec.mat = object.mat;
	
	return true;
}


bool gotHit (const Ray& r, Interval rayT, HitRecord& rec, Object3D* objects, int objectAmount) {
	HitRecord	tempRec;
	bool		hitAnything		= false;
	double		closestSoFar	= rayT.max;


	for (int i = 0; i < objectAmount; i++) {
		if (hit(objects[i], r, (Interval){rayT.min, closestSoFar}, tempRec)) {
			hitAnything = true;
			closestSoFar = tempRec.t;
			rec = tempRec;
		}
	}

	return hitAnything;
}



Vec3 rayColorTest(const Ray& r, int depth, Object3D* objects, int objectAmount) {
	HitRecord rec;
	Vec3 color = {0, 0, 0};
	Vec3 attenuationList = {1, 1, 1};

	Ray currentRay = r;

	while (1) {
		if (depth <= 0) {
			return (Vec3){0, 0, 0};
		}

		if (gotHit(currentRay, (Interval){0.001, INFINITY}, rec, objects, objectAmount)) {
			Ray scattered;
			Vec3 attenuation;
			if (rec.mat->scatter(currentRay, rec, attenuation, scattered)) {
				depth -= 1;
				/*attenuationList.push_back(attenuation);*/
				attenuationList = attenuationList * attenuation;
				currentRay = scattered;
				continue;
			}
			return (Vec3){0, 0, 0};
		}
		Vec3 unitDirection = unitVector(r.direction);
		double a = 0.5 * (unitDirection.y + 1.0);
		color = (1.0 - a) * (Vec3){1, 1, 1} + a * (Vec3){0.5, 0.7, 1.0};

		break;
	}
	/*while (attenuationList.size() != 0) {*/
	/*	color = attenuationList.back() * color;*/
	/*	attenuationList.pop_back();*/
	/*}*/
	return color * attenuationList;
}

