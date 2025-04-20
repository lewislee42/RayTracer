#ifndef VEC3_CLASS_HPP
#define VEC3_CLASS_HPP

#include <cmath>
#include <iostream>

class Vec3 {
  public:
	float x;
    float y;
    float z;


    Vec3();
    Vec3(float x, float y, float z);

    Vec3 operator-() const;
    Vec3& operator+=(const Vec3& v);
    Vec3& operator*=(float t);
    Vec3& operator/=(float t);

    float	length() const;
    float	lengthSquared() const;
	bool	nearZero() const;
};

// point3 is just an alias for Vec3, but useful for geometric clarity in the code.
using point3 = Vec3;

// Vector Utility Functions

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
    return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
    return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return Vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline Vec3 operator*(double t, const Vec3& v) {
    return Vec3(t*v.x, t*v.y, t*v.z);
}

inline Vec3 operator*(const Vec3& v, double t) {
    return t * v;
}

inline Vec3 operator/(const Vec3& v, double t) {
    return (1/t) * v;
}

inline float dot(const Vec3& u, const Vec3& v) {
    return u.x * v.x
         + u.y * v.y
         + u.z * v.z;
}

inline Vec3 cross(const Vec3& u, const Vec3& v) {
    return Vec3(u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
}

inline Vec3 unitVector(const Vec3& v) {
    return v / v.length();
}

inline float randomDouble() {
	return std::rand() / (RAND_MAX + 1.0);
}

inline float randomDouble(float min, float max) {
	return min + (max - min) * randomDouble();
}

inline Vec3 randomVector() {
	return Vec3(randomDouble(), randomDouble(), randomDouble());
}

inline Vec3 randomVector(float min, float max) {
	return Vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
}

inline Vec3 randomUnitVector() {
	while (true) {
		Vec3 p = randomVector(-1, 1);
		float lensq = p.lengthSquared();
		if (1e-160 < lensq && lensq <= 1)
			return p / sqrt(lensq);
	}
}

inline Vec3 randomOnHemisphere(const Vec3& normal) {
	Vec3 onUnitSphere = randomUnitVector();
	if (dot(onUnitSphere, normal) > 0.0)
		return onUnitSphere;
	return -onUnitSphere;
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
	return v - 2 * dot(v, n) * n;
}

inline Vec3 refract(const Vec3&uv, const Vec3& n, float etaIOverEtat) {
	float cosTheta = std::fmin(dot(-uv, n), 1.0);

	Vec3 rOutPerp = etaIOverEtat * (uv + cosTheta * n);
	Vec3 rOutPara = -std::sqrt(std::fabs(1.0 - rOutPerp.lengthSquared())) * n;
	return rOutPerp + rOutPara;
}

#endif
