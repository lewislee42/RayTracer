
#ifndef UTILS_HPP
# define UTILS_HPP

# include <cmath>
# include <iostream>
# include <memory>
# include <cstdlib>
# include <limits>

using std::make_shared;
using std::shared_ptr;

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

inline float degreesToRadians(float degrees) {
	return degrees * pi / 180;
}

inline float linearToGamma(float linearComponent) {
	if (linearComponent > 0)
		return sqrt(linearComponent);
	return 0;
}

#endif /* UTILS_HPP */
