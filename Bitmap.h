#ifndef _PLATER_BITMAP_H
#define _PLATER_BITMAP_H

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

            bool getPoint(int x, int y)
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
                        for (int y=0; y<width; y++) {
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

        protected:
            bool *data;
            int width, height;
    };
}

#endif
