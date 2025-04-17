
#ifndef RAY_CLASS_HPP
# define RAY_CLASS_HPP

# include <Vec3.hpp>
# include <Utils.hpp>

class Ray {
private:
	Vec3 _origin;
	Vec3 _direction;
	
public:
	Ray(const Vec3& origin, const Vec3& direction);
	
	const Vec3& getOrigin() const;
	const Vec3& getDirection() const;

	Vec3 at(double t) const;

};

#endif /* RAY_CLASS_HPP */
