#ifndef __ISHADER_H__
#define __ISHADER_H__

#include "tgaimage.hpp"

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat3x2.hpp>
#include <glm/mat4x4.hpp>

using glm::vec3;
using glm::mat3;
using glm::mat3x2;
using glm::mat4;

typedef struct
{
    TGAColor c;
    bool valid;
}
frag_color;

class ishader
{
public:
    virtual vec3 vertex(
            const mat4& viewmat,
            const vec3& v) const = 0;

    virtual frag_color fragment(
            const vec3& bary,
            const mat3& verts,
            const mat3x2& tex_coords,
            const mat3& vert_norms) const = 0;
};

#endif // __ISHADER_H__
