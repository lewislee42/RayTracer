
# include <Interval.hpp>

const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);

Interval::Interval(): min(+infinity), max(-infinity) {
}

Interval::Interval(const float& min, const float& max): min(min), max(max) {
}

float	Interval::size() const {
	return this->max - this->min;
}

bool	Interval::contains(float x) const {
	return (this->min <= x && x <= this->max);
}

bool	Interval::surrounds(float x) const {
	return (this->min < x && x < this->max);
}

float	Interval::clamp(float x) const {
	if (x < min)
		return min;
	if (x > max)
		return max;
	return x;
}
