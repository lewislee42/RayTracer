
# include <Vec3.hpp>

// Constructors
Vec3::Vec3(): x(0), y(0), z(0) {}

Vec3::Vec3(double x, double y, double z): x(x), y(y), z(z) {}


// Operator overload
Vec3 Vec3::operator-() const {
	return Vec3(-x, -y, -z);
}

Vec3& Vec3::operator+=(const Vec3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vec3& Vec3::operator*=(double t) {
	x *= t;
	y *= t;
	z *= t;
	return *this;
}

Vec3& Vec3::operator/=(double t) {
	return *this *= 1 / t;
}


// Member functions
double Vec3::length() const {
	return std::sqrt(lengthSquared());
}

double Vec3::lengthSquared() const {
	return x * x + y * y + z * z;
}

bool Vec3::nearZero() const {
	double s = 1e-8;
	return (std::fabs(x) < s) && (std::fabs(y) < s) && (std::fabs(z) < s);
}
