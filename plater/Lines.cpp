#include <algorithm>
#include "log.h"
#include "Lines.h"
#include "stl/Point3.h"

namespace Plater
{
    Lines::Lines()
    {
    }

    void Lines::addLine(Line line)
    {
        lines.push_back(line);
    }
            
    void Lines::merge(const Lines &other)
    {
        for (auto &line : other.lines) {
            lines.push_back(line);
        }
    }
			
    Bitmap *Lines::pixelize(float precision, float dilatation)
    {
        FPoint2 m = this->min();
        FPoint2 M = this->max();
        float width = M.x-m.x + 2*dilatation;
        float height = M.y-m.y + 2*dilatation;
        int w = width/precision;
        int h = height/precision;
        Bitmap *bmp = new Bitmap(w, h);

        for (int y=0; y<h; y++) {
            float X = m.x-dilatation;
            // Adding a silly constant to avoid glitches
            float Y = m.y-dilatation + y*precision + precision/2.0 + 1.1982381;
            Line ln(X, Y, width, 0);
            vector<float> is = intersections(ln);
            std::sort(is.begin(), is.end());

            for (int i=0; i+1<is.size(); i+=2) {
                int x1 = round(is[i]*w);
                int x2 = round(is[i+1]*w);
                for (int x=x1; x<x2; x++) {
                    bmp->setPoint(x, y, 2);
                }
            }
        }

        bmp->dilatation(dilatation/precision);
        return bmp;
    }

    FPoint2 Lines::min()
    {
        FPoint2 p;

        if (lines.size()) {
            p = lines[0].min();
            for (unsigned int i=1; i<lines.size(); i++) {
                FPoint2 pt = lines[i].min();
                if (pt.x < p.x) p.x = pt.x;
                if (pt.y < p.y) p.y = pt.y;
            }
        }

        return p;
    }
            
    std::vector<float> Lines::intersections(const Line &other)
    {
        std::vector<float> is;

        for (auto &line : lines) {
            float a, b;
            if (other.intersections(line, &a, &b)) {
                if (a>=0 && a<=1 && b>=0 && b<=1) {
                    is.push_back(a);
                }
            }
        }

        return is;
    }

    FPoint2 Lines::max()
    {
        FPoint2 p;

        if (lines.size()) {
            p = lines[0].max();
            for (unsigned int i=1; i<lines.size(); i++) {
                FPoint2 pt = lines[i].max();
                if (pt.x > p.x) p.x = pt.x;
                if (pt.y > p.y) p.y = pt.y;
            }
        }

        return p;
    }
}
