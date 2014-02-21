#ifndef UTILS_H
#define UTILS_H

#include <math.h>

#define PI		(3.1415926535897932384626433832795)
#define degtorad(d)	((d) * (PI / 180.))
#define radtodeg(r)	((r) * (180. / PI))

#define BUILD	(120)

#define imod(x, min, max)	((x) < (min)? (min) : ((x) > (max) ? (max) : (x)))
#define qmod(x, min, max)	((x) - (min) < 0 ? fmod((x) + (min), (max) - (min)) - (min) : fmod((x) - (min), (max) - (min)) + (min))

#endif
