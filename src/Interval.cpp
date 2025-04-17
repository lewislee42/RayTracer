
# include <Interval.hpp>

const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);

Interval::Interval(): min(+infinity), max(-infinity) {
}

Interval::Interval(const double& min, const double& max): min(min), max(max) {
}

double	Interval::size() const {
	return this->max - this->min;
}

bool	Interval::contains(double x) const {
	return (this->min <= x && x <= this->max);
}

bool	Interval::surrounds(double x) const {
	return (this->min < x && x < this->max);
}

double	Interval::clamp(double x) const {
	if (x < min)
		return min;
	if (x > max)
		return max;
	return x;
}
