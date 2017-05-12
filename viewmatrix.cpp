#include "viewmatrix.hpp"
#include <cassert>

glm::mat4 perspective_proj_xform(float camera_distance)
{
    return { 1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, -1 / camera_distance,
             0, 0, 0, 1 };
}

glm::mat4 screenspace_xform(int w, int h)
{
    assert(false);
    glm::mat4 a;
    return a;
}
