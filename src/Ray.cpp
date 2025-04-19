
#include <Ray.hpp>

Ray::Ray() {}

Ray::Ray(const Vec3& origin, const Vec3& direction): _origin(origin), _direction(direction) {
}

const Vec3& Ray::getOrigin() const {
	return this->_origin;
}

const Vec3& Ray::getDirection() const {
	return this->_direction;
}

Vec3 Ray::at(double t) const {
	return this->_origin + t * this->_direction;
}

