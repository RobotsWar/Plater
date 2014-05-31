#ifndef POINT3_H
#define POINT3_H

namespace Plater
{
	class FPoint2
	{
	    public:
	        double x, y, z;
	        FPoint2() {}
	        FPoint2(double _x, double _y): x(_x), y(_y) {}
	};
    class Point3
    {
        public:
            double x, y, z;
            Point3();
            Point3(double x, double y, double z);
    };
}
#endif