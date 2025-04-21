
#ifndef SHADERTYPE_H
#define SHADERTYPE_H

#include <simd/simd.h>

using Vec3 = vector_float3;
using Vec2 = vector_float2;

typedef enum {
    LAMBERTIAN,
    METAL,
    DIELECTRIC
}    MaterialType;

typedef struct {
    // material stuff
    MaterialType    matType;

    // for metal & lambertian
    Vec3 albedo;
    // for metal
    Vec3 fuzz;

    // for dielectric
    float refractionIndex;
}    Material;

typedef struct {
    float min;
    float max;
}    Interval;

typedef struct {
    Vec3        p;
    Vec3        normal;
    Material    mat;
    float       t;
    bool        frontFace;
}    HitRecord;

typedef struct {
    // sphere stuff
    Vec3    center;
    float   radius;

    Material        mat;
}    Object3D;

typedef struct {
    Vec3 origin;
    Vec3 direction;
}    Ray;


#endif /* SHADERTYPE_H */


// Vec/Math helper functions
inline float dot(const Vec3 u, const Vec3 v) {
    return u.x * v.x
         + u.y * v.y
         + u.z * v.z;
}

inline float dot(const Vec2 u, const Vec2 v) {
    return u.x * v.x
         + u.y * v.y;
}

inline float randomDouble(float min, float max) {
    return min + (min - max) * simd::fract(metal::sin(dot((Vec2){min, max}, (Vec2){12.9898, 78.233})) * 43758.5453);
}

inline Vec3 at(Vec3 o, Vec3 direction, float t) {
    return o + t * direction;
}

inline float lengthSquared(const Vec3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline float length(const Vec3 v) {
    return metal::sqrt(lengthSquared(v));
}

inline Vec3 unitVector(const Vec3 v) {
    return v / length(v);
}

inline Vec3 randomVector(float min, float max) {
    return (Vec3){randomDouble(min, max), randomDouble(min, max), randomDouble(min, max)};
}

inline Vec3 randomUnitVector() {
    while (true) {
        Vec3 p = randomVector(-1, 1);
        float lensq = lengthSquared(p);
        if (0 < lensq && lensq <= 1)
            return p / metal::sqrt(lensq);
    }
}



inline bool nearZero(Vec3 v) {
    float s = 1e-8;
    return (metal::fabs(v.x) < s) && (metal::fabs(v.y) < s) && (metal::fabs(v.z) < s);
}

inline Vec3 reflect(const Vec3 v, const Vec3 n) {
    return v - 2 * dot(v, n) * n;
}

inline Vec3 refract(const Vec3 uv, const Vec3 n, float etaIOverEtat) {
    float cosTheta = metal::fmin(dot(-uv, n), 1.0);

    Vec3 rOutPerp = etaIOverEtat * (uv + cosTheta * n);
    Vec3 rOutPara = -metal::sqrt(metal::fabs(1.0 - lengthSquared(rOutPerp))) * n;
    return rOutPerp + rOutPara;
}


bool scatterLambertian(const Material mat, const Ray rIn, const HitRecord rec, Vec3 attenuation, Ray scattered) {
    Vec3 scatterDirection = rec.normal + randomUnitVector();

    if (nearZero(scatterDirection))
        scatterDirection = rec.normal;

    scattered = (Ray){rec.p, scatterDirection};
    attenuation = mat.albedo;
    return true;
}

// Metal
bool scatterMetal(const Material mat, const Ray rIn, const HitRecord rec, Vec3 attenuation, Ray scattered) {
    Vec3 reflected = reflect(rIn.direction, rec.normal);
    reflected = unitVector(reflected) + (mat.fuzz * randomUnitVector());
    scattered = (Ray){rec.p, reflected};
    attenuation = mat.albedo;

    return (dot(scattered.direction, rec.normal) > 0);
}

// Dielectric
bool scatterDielectric(const Material mat, const Ray rIn, const HitRecord rec, Vec3 attenuation, Ray scattered) {
    attenuation = (Vec3){1.0, 1.0, 1.0};
    float ri;
    if (rec.frontFace)
        ri = 1.0 / mat.refractionIndex;
    else
        ri = mat.refractionIndex;

    Vec3 unitDirection = unitVector(rIn.direction);
    float cosTheta = metal::fmin(dot(-unitDirection, rec.normal), 1.0);
    float sinTheta = metal::sqrt(1.0 - cosTheta * cosTheta);

    bool cannotRefract = ri * sinTheta > 1.0;
    Vec3 direction;

    if (cannotRefract)
        direction = reflect(unitDirection, rec.normal);
    else
        direction = refract(unitDirection, rec.normal, ri);
        

    scattered = (Ray){rec.p, direction};
    return true;
}




// Raytracing part
bool hit(const Object3D object, const Ray r, Interval rayT, HitRecord rec) {
    Vec3 oc = object.center - r.origin;
    float a = lengthSquared(r.direction);
    float b = dot(r.direction, oc);
    float c = lengthSquared(oc) - object.radius * object.radius;
    float descriminant = b * b - a * c;

    if (descriminant < 0)
        return false;

    float sqrtD = metal::sqrt(descriminant);
    float root = (b - sqrtD) / a;
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


bool gotHit (const Ray r, Interval rayT, HitRecord rec, Object3D* objects, int objectAmount) {
    HitRecord    tempRec;
    bool        hitAnything        = false;
    float        closestSoFar    = rayT.max;


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
                attenuationList = attenuationList * attenuation;
                currentRay = scattered;
                continue;
            }
            return (Vec3){0, 0, 0};
        }
        Vec3 unitDirection = unitVector(r.direction);
        float a = 0.5 * (unitDirection.y + 1.0);
        color = (1.0 - a) * (Vec3){1, 1, 1} + a * (Vec3){0.5, 0.7, 1.0};

        break;
    }
    return color * attenuationList;
}


