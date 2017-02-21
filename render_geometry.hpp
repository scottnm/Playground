#ifndef __RENDER_GEOMETRY_H__
#define __RENDER_GEOMETRY_H__

#include "tgaimage.hpp"
#include "vec2i.hpp"

namespace render_geometry
{
    void line(TGAImage& img, const TGAColor color, vec2i start, vec2i dest);
}

#endif 
