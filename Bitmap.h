#ifndef _PLATER_BITMAP_H
#define _PLATER_BITMAP_H

#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "util.h"

using namespace std;

#define BMP_POSITION(x,y) (width*(y)+(x))

namespace Plater
{
    class Bitmap
    {
        public:
            Bitmap(const Bitmap *other);
            Bitmap(int width, int height);
            virtual ~Bitmap();

            void toPpm();

            bool getPoint(int x, int y) const
            {
                if (data == NULL || x < 0 || y < 0 || x >= width || y >= height) {
                    return false;
                } else {
                    return data[BMP_POSITION(x,y)];
                }
            }

            void setPoint(int x, int y, bool value)
            {
                if (!(data == NULL || x < 0 || y < 0 || x >= width || y >= height)) {
                    data[BMP_POSITION(x,y)] = value;
                }
            }

            void dilatation(int iterations)
            {
                for (int i=0; i<iterations; i++) {
                    Bitmap old(this);
                    for (int x=0; x<width; x++) {
                        for (int y=0; y<height; y++) {
                            if (!old.getPoint(x, y)) {
                                int score = 0;
                                for (int dx=-1; dx<=1; dx++) {
                                    for (int dy=-1; dy<=1; dy++) {
                                        if (old.getPoint(x+dx, y+dy)) {
                                            score++;
                                        }
                                    }
                                }
                                if (score >= 1) {
                                    setPoint(x, y, true);
                                }
                            }
                        }
                    }
                }
            }

            bool overlaps(const Bitmap *other, int offx, int offy)
            {
                for (int x=0; x<width; x++) {
                    for (int y=0; y<height; y++) {
                        if (getPoint(x, y) && other->getPoint(x+offx, y+offy)) {
                                return true;
                        }
                    }
                }
                return false;
            }

            void write(const Bitmap *other, int offx, int offy)
            {
                for (int x=0; x<other->width; x++) {
                    for (int y=0; y<other->height; y++) {
                        if (other->getPoint(x, y)) {
                            setPoint(x+offx, y+offy, true);
                        }
                    }
                }
            }

            static Bitmap *rotate(const Bitmap *other, float r)
            {
                float w = other->width;
                float h = other->height;

                int aX = (int)ceil(w*cos(r) - h*sin(r));
                int aY = (int)ceil(w*sin(r) + h*cos(r));

                int bX = (int)ceil(-h*sin(r));
                int bY = (int)ceil(h*cos(r));

                int cX = (int)ceil(w*cos(r));
                int cY = (int)ceil(w*sin(r));

                int xMin = MIN(MIN(0, aX), MIN(bX, cX));
                int xMax = MAX(MAX(0, aX), MAX(bX, cX));
                int yMin = MIN(MIN(0, aY), MIN(bY, cY));
                int yMax = MAX(MAX(0, aY), MAX(bY, cY));

                int width = xMax-xMin;
                int height = yMax-yMin;

                // cout << "W: " << width << ", H: " << height << endl;

                int oldCenterX = other->width/2;
                int oldCenterY = other->height/2;
                int centerX = width/2;
                int centerY = height/2;

                Bitmap *rotated = new Bitmap(width, height);
                for (int x=0; x<width; x++) {
                    for (int y=0; y<height; y++) {
                        int cX = x-centerX;
                        int cY = y-centerY;
                        int X = (cos(r)*cX - sin(r)*cY) + oldCenterX;
                        int Y = (sin(r)*cX + cos(r)*cY) + oldCenterY;
                        rotated->setPoint(x, y, other->getPoint(X, Y));
                    }
                }

                return rotated;
            }

            bool *data;
            int width, height;
    };
}

#endif
