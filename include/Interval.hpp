
#ifndef INTERVAL_CLASS_HPP
# define INTERVAL_CLASS_HPP 

# include <Utils.hpp>

class Interval {
public:
	float min;
	float max;

	static const Interval empty;
	static const Interval universe;


	Interval();
	Interval(const float& min, const float& max);

	float	size() const;
	bool	contains(float x) const;
	bool	surrounds(float x) const;
	float	clamp(float x) const;


};


#endif /* INTERVAL_CLASS_HPP */
