#include <iostream>
#include "Triangle.h"
#include "util.h"

using namespace std;

namespace Plater
{
    Triangle::Triangle()
    {
    }

    Triangle::Triangle(FPoint2 A, FPoint2 B, FPoint2 C)
    {
        setPoints(A, B, C);
    }
            
    void Triangle::setPoints(FPoint2 A_, FPoint2 B_, FPoint2 C_)
    {
        A = A_;
        B = B_;
        C = C_;

        // Segments
        AB = FPoint2(B.x-A.x, B.y-A.y);
        BC = FPoint2(C.x-B.x, C.y-B.y);
        CA = FPoint2(A.x-C.x, A.y-C.y);

        // Segments normals
        nAB = FPoint2(AB.y, -AB.x);
        nBC = FPoint2(BC.y, -BC.x);
        nCA = FPoint2(CA.y, -CA.x);

        box.x1 = MIN(A.x, MIN(B.x, C.x));
        box.y1 = MIN(A.y, MIN(B.y, C.y));
        box.x2 = MAX(A.x, MAX(B.x, C.x));
        box.y2 = MAX(A.y, MAX(B.y, C.y));
    }

    static bool getSide(FPoint2 pt, FPoint2 n, FPoint2 s)
    {
        double scalarN = n.x*pt.x + n.y*pt.y;

        if (scalarN == 0) {
            double scalar = s.x*pt.x + s.y*pt.y;
            return scalar > 0;
        }

        return scalarN < 0;
    }

    bool Triangle::contains(double x, double y)
    {
        FPoint2 vectorA(x-A.x, y-A.y);
        FPoint2 vectorB(x-B.x, y-B.y);
        FPoint2 vectorC(x-C.x, y-C.y);

        bool sideA = getSide(vectorA, nAB, AB);
        bool sideB = getSide(vectorB, nBC, BC);
        bool sideC = getSide(vectorC, nCA, CA);

        return sideA && sideB && sideC;
    }

    bool Triangle::contains(const FPoint2 &p)
    {
        return contains(p.x, p.y);
    }
            
    bool Triangle::contains(const Rectangle &rect)
    {
        return contains(rect.x1, rect.y1)
            && contains(rect.x1, rect.y2)
            && contains(rect.x2, rect.y1)
            && contains(rect.x2, rect.y2);
    }
}
