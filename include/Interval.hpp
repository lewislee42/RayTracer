
#ifndef INTERVAL_CLASS_HPP
# define INTERVAL_CLASS_HPP 

# include <Utils.hpp>

typedef struct Interval {
	float min;
	float max;
}	Interval;

float	size();
bool	contains(float x);
bool	surrounds(float x);
float	clamp(float x, float min, float max);

#endif /* INTERVAL_CLASS_HPP */
