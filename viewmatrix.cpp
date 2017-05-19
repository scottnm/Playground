#include "viewmatrix.hpp"
#include <cassert>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>

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


glm::mat4 lookat_xform(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
    auto cam_z = glm::normalize(eye - center);
    auto cam_x = glm::normalize(glm::cross(up, cam_z));
    auto cam_y = glm::normalize(glm::cross(cam_z, cam_x));

    auto cam_orientation = glm::mat4(cam_x.x, cam_y.x, cam_z.x, 0,
                                     cam_x.y, cam_y.y, cam_z.y, 0,
                                     cam_x.z, cam_y.z, cam_z.z, 0,
                                           0,       0,       0, 1);
    auto cam_pos = glm::mat4(1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             -center.x, -center.y, -center.z, 1);
    return cam_orientation * cam_pos;
}


glm::mat3 retroproject(glm::mat3x4 m)
{
    m[0] = m[0] / m[0].w;
    m[1] = m[1] / m[1].w;
    m[2] = m[2] / m[2].w;
    return m;
}
