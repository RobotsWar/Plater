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

            std::string toPpm();

            unsigned char getPoint(int x, int y) const
            {
                if (data == NULL || x < 0 || y < 0 || x >= width || y >= height) {
                    return false;
                } else {
                    return data[BMP_POSITION(x,y)];
                }
            }

            void setPoint(int x, int y, unsigned char value)
            {
                if (!(data == NULL || x < 0 || y < 0 || x >= width || y >= height)) {
                    data[BMP_POSITION(x,y)] = value;
                    if (value) {
                        sX += x;
                        sY += y;
                        pixels++;
                    }
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
                                    setPoint(x, y, 1);
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
                            setPoint(x+offx, y+offy, other->getPoint(x, y));
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

                float oldCenterX = other->width/2;
                float oldCenterY = other->height/2;
                float centerX = width/2;
                float centerY = height/2;

                Bitmap *rotated = new Bitmap(width, height);
                for (int x=0; x<width; x++) {
                    for (int y=0; y<height; y++) {
                        int cX = round(x-centerX);
                        int cY = round(y-centerY);
                        int X = round((cos(r)*cX - sin(r)*cY) + oldCenterX);
                        int Y = round((sin(r)*cX + cos(r)*cY) + oldCenterY);
                        rotated->setPoint(x, y, other->getPoint(X, Y));
                    }
                }

                return rotated;
            }

            static Bitmap *trim(const Bitmap *bmp)
            {
                bool found = false;
                int minX=0, minY=0;
                int maxX=0, maxY=0;

                for (int x=0; x<bmp->width; x++) {
                    for (int y=0; y<bmp->height; y++) {
                        if (bmp->getPoint(x, y)) {
                            if (!found) {
                                found = true;
                                minX = maxX = x;
                                minY = maxY = y;
                            } else {
                                if (x < minX) minX = x;
                                if (y < minY) minY = y;
                                if (x > maxX) maxX = x;
                                if (y > maxY) maxY = y;
                            }
                        }
                    }
                }

                int deltaX = maxX-minX;
                int deltaY = maxY-minY;
                Bitmap *trimmed = new Bitmap(deltaX, deltaY);
                for (int x=0; x<deltaX; x++) {
                    for (int y=0; y<deltaY; y++) {
                        trimmed->setPoint(x, y, bmp->getPoint(x+minX, y+minY));
                    }
                }

                return trimmed;
            }

            // Image pixels
            unsigned char *data;

            // Image dimension
            int width, height;

            // Sum of the X, Y and number of pixels
            unsigned long long int sX, sY, pixels;
    };
}

#endif
