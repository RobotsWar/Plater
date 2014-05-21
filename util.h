#ifndef _PLATER_UTIL_H
#define _PLATER_UTIL_H

#include <string>
#include <vector>

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

#define RAD2DEG(a) ((a)*180.0/M_PI)
#define DEG2RAD(a) ((a)*M_PI/180.0)

#define ABS(a) ((a)>0 ? (a) : -(a))

#define RND(a) ((random()%a)==0)

#endif
