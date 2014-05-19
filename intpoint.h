/** Copyright (C) 2013 David Braam - Released under terms of the AGPLv3 License */
#ifndef INT_POINT_H
#define INT_POINT_H

/*
The integer point classes are used as soon as possible and represent microns in 2D or 3D space.
Integer points are used to avoid floating point rounding errors, and because ClipperLib uses them.
*/
#define INLINE static inline

#include <limits.h>
#include <stdint.h>
#include <math.h>

#define INT2MM(n) (double(n) / 1000.0)
#define MM2INT(n) (int64_t((n) * 1000))

#define INT2MICRON(n) ((n) / 1)
#define MICRON2INT(n) ((n) * 1)

class Point3
{
public:
    int32_t x,y,z;
    Point3() {}
    Point3(const int32_t _x, const int32_t _y, const int32_t _z): x(_x), y(_y), z(_z) {}
    
    Point3 operator+(const Point3& p) const { return Point3(x+p.x, y+p.y, z+p.z); }
    Point3 operator-(const Point3& p) const { return Point3(x-p.x, y-p.y, z-p.z); }
    Point3 operator/(const int32_t i) const { return Point3(x/i, y/i, z/i); }
    
    Point3& operator += (const Point3& p) { x += p.x; y += p.y; z += p.z; return *this; }
    Point3& operator -= (const Point3& p) { x -= p.x; y -= p.y; z -= p.z; return *this; }
    
    bool operator==(const Point3& p) const { return x==p.x&&y==p.y&&z==p.z; }
    bool operator!=(const Point3& p) const { return x!=p.x||y!=p.y||z!=p.z; }
    
    int32_t max()
    {
        if (x > y && x > z) return x;
        if (y > z) return y;
        return z;
    }
    
    bool testLength(int32_t len)
    {
        if (x > len || x < -len)
            return false;
        if (y > len || y < -len)
            return false;
        if (z > len || z < -len)
            return false;
        return vSize2() <= len*len;
    }
    
    int64_t vSize2()
    {
        return int64_t(x)*int64_t(x)+int64_t(y)*int64_t(y)+int64_t(z)*int64_t(z);
    }
    
    int32_t vSize()
    {
        return sqrt(vSize2());
    }
    
    Point3 cross(const Point3& p)
    {
        return Point3(
            y*p.z-z*p.y,
            z*p.x-x*p.z,
            x*p.y-y*p.x);
    }
};

#endif//INT_POINT_H
