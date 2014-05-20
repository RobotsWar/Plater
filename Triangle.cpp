#include <iostream>
#include "Triangle.h"

using namespace std;

namespace Plater
{
    Triangle::Triangle(FPoint2 A, FPoint2 B, FPoint2 C)
        : A(A), B(B), C(C)
    {
        // Segments
        AB = FPoint2(B.x-A.x, B.y-A.y);
        BC = FPoint2(C.x-B.x, C.y-B.y);
        CA = FPoint2(A.x-C.x, A.y-C.y);

        // Segments normals
        nAB = FPoint2(AB.y, -AB.x);
        nBC = FPoint2(BC.y, -BC.x);
        nCA = FPoint2(CA.y, -CA.x);
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
}
