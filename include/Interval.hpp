
#ifndef INTERVAL_CLASS_HPP
# define INTERVAL_CLASS_HPP 

# include <Utils.hpp>

class Interval {
public:
	double min;
	double max;

	static const Interval empty;
	static const Interval universe;


	Interval();
	Interval(const double& min, const double& max);

	double	size() const;
	bool	contains(double x) const;
	bool	surrounds(double x) const;
	double	clamp(double x) const;

};


#endif /* INTERVAL_CLASS_HPP */
