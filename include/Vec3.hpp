#ifndef VEC3_CLASS_HPP
#define VEC3_CLASS_HPP

#include <cfloat>
#include <cmath>
# include <limits>
#include <cstdint>
#include <iostream>
#include <simd/simd.h>
#include <Utils.hpp>

typedef struct Vec3 {
	float x;
    float y;
    float z;
}	Vec3;

// point3 is just an alias for Vec3, but useful for geometric clarity in the code.
using point3 = Vec3;

// Vector Utility Functions

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
    return (Vec3){u.x + v.x, u.y + v.y, u.z + v.z};
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
    return (Vec3){u.x - v.x, u.y - v.y, u.z - v.z};
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return (Vec3){u.x * v.x, u.y * v.y, u.z * v.z};
}

inline Vec3 operator*(float t, const Vec3& v) {
    return (Vec3){t*v.x, t*v.y, t*v.z};
}

inline Vec3 operator*(const Vec3& v, float t) {
    return t * v;
}

inline Vec3 operator/(const Vec3& v, float t) {
    return (1/t) * v;
}

inline float	lengthSquared(const Vec3& v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline float	length(const Vec3& v) {
	return std::sqrt(lengthSquared(v));
}

inline bool 	nearZero(const Vec3& v) {
	float s = 1e-8;
	return (std::fabs(v.x) < s) && (std::fabs(v.y) < s) && (std::fabs(v.z) < s);
}


inline float dot(const Vec3& u, const Vec3& v) {
    return u.x * v.x
         + u.y * v.y
         + u.z * v.z;
}

inline Vec3 cross(const Vec3& u, const Vec3& v) {
    return (Vec3){u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x};
}

inline Vec3 unitVector(const Vec3& v) {
    return v / length(v);
}

inline unsigned TausStep(const unsigned z, const int s1, const int s2, const int s3, const unsigned M) {
    unsigned b=(((z << s1) ^ z) >> s2);
    return (((z & M) << s3) ^ b);
}

inline float randomDouble() {
	return std::rand() / (RAND_MAX + 1.0);
}

inline float randomDouble(float min, float max, Vec3 v) {

	unsigned seed = v.x * 1099087573UL;
	unsigned z1 = TausStep(seed,13,19,12,429496729UL);
	unsigned z2 = TausStep(seed,2,25,4,4294967288UL);
	unsigned z3 = TausStep(seed,3,11,17,429496280UL);
	unsigned z4 = (1664525*seed + 1013904223UL);

	return min + (max - min) * ((z1^z2^z3^z4) * 2.3283064365387e-10);
}

inline Vec3 randomVector() {
	return (Vec3){randomDouble(), randomDouble(), randomDouble()};
}

inline Vec3 randomVector(float min, float max, Vec3 v) {

	if (v.x < 0)
		v.x *= -1;
	if (v.y < 0)
		v.y *= -1;
	if (v.z < 0)
		v.z *= -1;

	unsigned seed = (v.x) * 1099087573UL;
	unsigned seedb = (v.y) * 1099087573UL;
	unsigned seedc = (v.z) * 1099087573UL;
	unsigned z1 = TausStep(seed,13,19,12,429496729UL);
	unsigned z2 = TausStep(seed,2,25,4,4294967288UL);
	unsigned z3 = TausStep(seed,3,11,17,429496280UL);
	unsigned z4 = (1664525*seed + 1013904223UL);

	unsigned r1 = (z1^z2^z3^z4^seedb);
	   z1 = TausStep(r1,13,19,12,429496729UL);
	   z2 = TausStep(r1,2,25,4,4294967288UL);
	   z3 = TausStep(r1,3,11,17,429496280UL);
	   z4 = (1664525*r1 + 1013904223UL);

	unsigned r2 = (z1^z2^z3^z4^seedc);
	   z1 = TausStep(r2,13,19,12,429496729UL);
	   z2 = TausStep(r2,2,25,4,4294967288UL);
	   z3 = TausStep(r2,3,11,17,429496280UL);
	   z4 = (1664525*r2 + 1013904223UL);

	unsigned r3 = (z1^z2^z3^z4^seedb);

	float fr1 = min + (max - min) * (r1 * 2.3283064365387e-10);
	float fr2 = min + (max - min) * (r2 * 2.3283064365387e-10);
	float fr3 = min + (max - min) * (r3 * 2.3283064365387e-10);

	return (Vec3){float(fr1), float(fr2), float(fr3)};
}

inline Vec3 randomUnitVector(Vec3 v) {
	Vec3 seedVec = v;
	while (true) {
		seedVec = randomVector(-1, 1, seedVec);
		float lensq = lengthSquared(seedVec);
		if (0 < lensq && lensq <= 1) {
			return seedVec / sqrt(lensq);
		}
		if (seedVec.x < 0)
			seedVec.x *= -1;
		if (seedVec.y < 0)
			seedVec.y *= -1;
		if (seedVec.z < 0)
			seedVec.z *= -1;
	}
}

inline Vec3 randomOnHemisphere(const Vec3& normal) {
	Vec3 onUnitSphere = randomUnitVector(normal);
	if (dot(onUnitSphere, normal) > 0.0)
		return onUnitSphere;
	return onUnitSphere * -1;
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
	return v - 2 * dot(v, n) * n;
}

inline Vec3 refract(const Vec3&uv, const Vec3& n, float etaIOverEtat) {
	float cosTheta = std::fmin(dot(uv * -1, n), 1.0);

	Vec3 rOutPerp = etaIOverEtat * (uv + cosTheta * n);
	Vec3 rOutPara = -std::sqrt(std::fabs(1.0 - lengthSquared(rOutPerp))) * n;
	return rOutPerp + rOutPara;
}

#endif
