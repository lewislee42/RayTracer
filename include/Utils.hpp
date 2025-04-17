
#ifndef UTILS_HPP
# define UTILS_HPP

# include <cmath>
# include <iostream>
# include <memory>
# include <cstdlib>
# include <limits>

using std::make_shared;
using std::shared_ptr;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degreesToRadians(double degrees) {
	return degrees * pi / 180;
}

#endif /* UTILS_HPP */
