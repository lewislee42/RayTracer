#ifndef VEC3_CLASS_HPP
#define VEC3_CLASS_HPP

#include <cmath>
#include <iostream>

class Vec3 {
  public:
    double x;
    double y;
    double z;

    Vec3();
    Vec3(double x, double y, double z);

    Vec3 operator-() const;
    Vec3& operator+=(const Vec3& v);
    Vec3& operator*=(double t);
    Vec3& operator/=(double t);

    double length() const;
    double lengthSquared() const;
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

inline double dot(const Vec3& u, const Vec3& v) {
    return u.x * v.x
         + u.y * v.y
         + u.z * v.z;
}

inline Vec3 cross(const Vec3& u, const Vec3& v) {
    return Vec3(u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
}

inline Vec3 unit_vector(const Vec3& v) {
    return v / v.length();
}

inline double randomDouble() {
	return std::rand() / (RAND_MAX + 1.0);
}

inline double randomDouble(double min, double max) {
	return min + (max - min) * randomDouble();
}

inline Vec3 randomVector() {
	return Vec3(randomDouble(), randomDouble(), randomDouble());
}

inline Vec3 randomVector(double min, double max) {
	return Vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
}

inline Vec3 randomUnitVector() {
	while (true) {
		Vec3 p = randomVector(-1, 1);
		double lensq = p.lengthSquared();
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

#endif
