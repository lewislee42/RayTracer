
# include <Interval.hpp>



float	size(float max, float min) {
	return max - min;
}

bool	contains(float x, float max, float min) {
	return (min <= x && x <= max);
}

bool	surrounds(float x, float max, float min) {
	return (min < x && x < max);
}

float	clamp(float x, float max, float min) {
	if (x < min)
		return min;
	if (x > max)
		return max;
	return x;
}
