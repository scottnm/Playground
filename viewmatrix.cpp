#include "viewmatrix.hpp"
#include <cassert>

glm::mat4 perspective_proj_xform(float camera_distance)
{
    return { 1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, -1 / camera_distance,
             0, 0, 0, 1 };
}

glm::mat4 screenspace_xform(int sw, int sh, int xs, int ys)
{
    return { xs / 2,      0, 0, 0,
                  0, ys / 2, 0, 0,
                  0,      0, 1, 0,
             sw / 2, sh / 2, 0, 1 };
}

glm::vec3 retroproject(glm::vec4 v)
{
    return v / v.w;
}
