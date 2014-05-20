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

            void dilatation(int iterations);

            /**
             * Tests if this bitmap overlaps another one at a given offset
             */
            bool overlaps(const Bitmap *other, int offx, int offy);

            /**
             * Write all the pixel of another bitmap into this one at a certain
             * offset
             */
            void write(const Bitmap *other, int offx, int offy);

            /**
             * Create a new bitmap rotated around the center by r radian
             */
            static Bitmap *rotate(const Bitmap *other, float r);

            /**
             * Trim the white spaces around this bitmap
             */
            static Bitmap *trim(const Bitmap *bmp);

            // Image dimension
            int width, height;

            // Sum of the X, Y and number of pixels
            unsigned long long int sX, sY, pixels;

            // Center of the sprite
            float centerX, centerY;

        protected:
            // Image pixels
            unsigned char *data;
    };
}

#endif
