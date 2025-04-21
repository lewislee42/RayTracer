#ifndef VEC3_CLASS_HPP
#define VEC3_CLASS_HPP

#include <cmath>
#include <iostream>

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

inline float randomDouble() {
	return std::rand() / (RAND_MAX + 1.0);
}

inline float randomDouble(float min, float max) {
	return min + (max - min) * randomDouble();
}

inline Vec3 randomVector() {
	return (Vec3){randomDouble(), randomDouble(), randomDouble()};
}

inline Vec3 randomVector(float min, float max) {
	return (Vec3){randomDouble(min, max), randomDouble(min, max), randomDouble(min, max)};
}

inline Vec3 randomUnitVector() {
	while (true) {
		Vec3 p = randomVector(-1, 1);
		float lensq = lengthSquared(p);
		if (0 < lensq && lensq <= 1)
			return p / sqrt(lensq);
	}
}

inline Vec3 randomOnHemisphere(const Vec3& normal) {
	Vec3 onUnitSphere = randomUnitVector();
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
