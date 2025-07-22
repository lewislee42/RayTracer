//
//  Vec3Shader.h
//  Raytracer
//
//  Created by Lewis lee on 07/07/2025.
//

#ifndef VECSHADER_HEADER
#define VECSHADER_HEADER

#include "RayTracingSharedStructs.h"
#include "../libs/Loki/loki_header.metal"

inline Vec3 operator+(const Vec3 u, const Vec3 v) {
    return (Vec3){u.x + v.x, u.y + v.y, u.z + v.z};
}

inline Vec3 operator-(const Vec3 u, const Vec3 v) {
    return (Vec3){u.x - v.x, u.y - v.y, u.z - v.z};
}

inline Vec3 operator*(const Vec3 u, const Vec3 v) {
    return (Vec3){u.x * v.x, u.y * v.y, u.z * v.z};
}

inline Vec3 operator+(const Vec3 v, const float n) {
    return (Vec3){n * v.x, n * v.y, n * v.z};
}

inline Vec3 operator*(float t, const Vec3 v) {
    return (Vec3){t*v.x, t*v.y, t*v.z};
}

inline Vec3 operator*(const Vec3 v, float t) {
    return t * v;
}

inline Vec3 operator/(const Vec3 v, float t) {
    return (1/t) * v;
}

inline float dot(const Vec3 u, const Vec3 v) {
    return u.x * v.x
         + u.y * v.y
         + u.z * v.z;
}

inline float dot(const Vec2 u, const Vec2 v) {
    return u.x * v.x
         + u.y * v.y;
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

inline Vec3 normalizeVec(Vec3 v) {
    float len = length(v);
    return (Vec3){v.x / len, v.y / len, v.z / len};
}

inline float randomFloat(float min, float max, thread Loki& loki) {
    return min + (max - min) * loki.rand();
}

inline Vec3 randomVector(float min, float max, thread Loki& loki) {
    return (Vec3){randomFloat(min, max, loki), randomFloat(min, max, loki), randomFloat(min, max, loki)};
}

inline float randomValueNormalDistribution(thread Loki& loki) {
	float theta = 2.0f * M_PI_F * randomFloat(0.0f, 1.0f, loki);
    float rho = metal::sqrt(-2 * metal::log(randomFloat(0.0f, 1.0f, loki)));
    return rho * metal::cos(theta);
}

inline Vec3 randomDirection(thread Loki& loki) {
    return normalizeVec(
		(Vec3){
			randomValueNormalDistribution(loki),
			randomValueNormalDistribution(loki),
			randomValueNormalDistribution(loki)
		}
	);
}

inline Vec3 randomUnitVector(thread Loki& loki) {

    for (int i = 0; i < 5; i++) {
        Vec3 vec = randomVector(-1.0f, 1.0f, loki);
        float lensq = lengthSquared(vec);
        if (0 < lensq && lensq <= 1) {
            return vec / metal::sqrt(lensq);
        }
    }
    // can just try to rand vec once and then normalize it?
    return randomVector(-1.0f, 1.0f, loki);
}

inline Vec3 randomOnHemisphere(const Vec3 normal, thread Loki& loki) {
    Vec3 onUnitSphere = randomDirection(loki);
    if (dot(onUnitSphere, normal) > 0.0)
        return onUnitSphere;
    return onUnitSphere * -1;
}

inline bool nearZero(Vec3 v) {
    float s = 1e-8;
    return (metal::fabs(v.x) < s) && (metal::fabs(v.y) < s) && (metal::fabs(v.z) < s);
}

inline Vec3 reflect(const Vec3 v, const Vec3 n) {
    return v - 2 * dot(v, n) * n;
}

inline Vec3 refract(const Vec3 uv, const Vec3 n, float etaIOverEtat) {
    float cosTheta = metal::fmin(dot(uv * -1, n), 1.0);

    Vec3 rOutPerp = etaIOverEtat * (uv + cosTheta * n);
    Vec3 rOutPara = -metal::sqrt(metal::fabs(1.0 - lengthSquared(rOutPerp))) * n;
    return rOutPerp + rOutPara;
}

/* Generates a 2d square bounding box
 Note:
 do not pass in a zero value in the seed where it could cause numbers
 generated numbers to be zero
*/
Vec3 sampleSquare(thread Loki& loki) {
	Vec3 seed1 = randomVector(0.1f, 1.0f, loki);
    
    seed1.x -= 0.5f;
    seed1.y -= 0.5f;
    seed1.z = 0;
    return seed1;
}

inline float linearToGamma(float linearComponent) {
    if (linearComponent > 0)
        return metal::sqrt(linearComponent);
    return 0;
}

inline Vec3 randomInUnitDisk(thread Loki& loki) {
    for (int i = 0; i < 5; i++) {
        Vec3 p = randomVector(-1.0f, 1.0f, loki);
        p.z = 0;
        if (length(p) < 1)
            return p;
    }
    Vec3 p = randomVector(-1.0f, 1.0f, loki);
    p = normalizeVec(p);
    p.z = 0;
    return p;
}

inline Interval intervalExpand(Interval interval, float delta) {
    float padding = delta / 2;
    return (Interval){interval.min - padding, interval.max + padding};
}

inline float intervalSize(float max, float min) {
    return max - min;
}




#endif
