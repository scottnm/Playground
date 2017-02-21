#ifndef __RENDER_GEOMETRY_H__
#define __RENDER_GEOMETRY_H__

#include "tgaimage.hpp"
#include <glm/vec2.hpp>

using glm::ivec2;

namespace render_geometry
{
    void line(TGAImage& img, const TGAColor color, ivec2 start, ivec2 dest);
}

#endif 
