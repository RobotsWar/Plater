#include "log.h"
#include "Lines.h"

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
			
    Bitmap *pixelize(float precision, float dilatation)
    {
        _log("Lines::pixelize not implemented");
        return NULL;
    }
}
