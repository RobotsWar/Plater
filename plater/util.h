#ifndef _PLATER_UTIL_H
#define _PLATER_UTIL_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <string>
#include <vector>

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

#define RAD2DEG(a) ((a)*180.0/M_PI)
#define DEG2RAD(a) ((a)*M_PI/180.0)

#define ABS(a) ((a)>0 ? (a) : -(a))

#define RND(a) ((random()%a)==0)

std::string getDirectory(const std::string &filename);
std::string getBasename(const std::string &filename);
bool chdirFile(const std::string &filename);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
std::vector<std::string> splitWithEscape(const std::string &s, char delim);
bool isNumeric(std::string str);

std::string ltrim(std::string s);
std::string rtrim(std::string s);
std::string trim(std::string s);

#endif
