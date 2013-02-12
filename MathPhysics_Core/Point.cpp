#include "Typedefs.h"
#include "CoreMathPhysics.h"
#include <cmath>


inline Point::Point(Float x, Float y) : x_(x), y_(y) { }

inline Point Point::rectangular(Float x, Float y)
{ 
	return Point(x, y);
}

inline Point Point::polar(Float radius, Float angle)
{ 
	return Point(radius*std::cos(angle), radius*std::sin(angle)); 
}