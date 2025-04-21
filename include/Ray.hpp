
#ifndef RAY_CLASS_HPP
# define RAY_CLASS_HPP

# include <Vec3.hpp>
# include <Utils.hpp>

typedef struct Ray {
	Vec3	origin;
	Vec3	direction;
}	Ray;

Vec3 at(const Ray& r, const double& t);

#endif /* RAY_CLASS_HPP */
