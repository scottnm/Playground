#include "render_geometry.hpp"

namespace render_geometry
{
    void line(TGAImage& img, const TGAColor color, ivec2 start, ivec2 dest)
    {
        // optionally transpose parameters so that the slope has a greater xdiff
        bool transposed = false;
        if (std::abs(dest.x - start.x) < std::abs(dest.y - start.y))
        {
            std::swap(start.x, start.y);
            std::swap(dest.x, dest.y);
            transposed = true;
        }

        // swap parameters if given in descending order
        if (start.x > dest.x)
        {
            std::swap(start, dest);
        }

        // draw the line
        int dx = dest.x - start.x;
        int dy = dest.y - start.y;
        for (int x = start.x; x < dest.x; ++x)
        {
            float t = (x - start.x)/(float)dx;
            int y = start.y + dy * t;
            if (transposed) { img.set(y, x, color); }
            else           { img.set(x, y, color); }
        }
    }
}
