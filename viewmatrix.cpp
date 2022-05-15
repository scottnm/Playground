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

glm::mat4 screenspace_xform(int x, int y, int w, int h)
{
    return {   w/2.f,       0,    0, 0,
                   0,   h/2.f,    0, 0,
                   0,       0, 1000, 0,
             x + w/2.f, y + h/2.f, 1000, 1 };
}


glm::mat4 lookat_xform(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
    auto cam_z = glm::normalize(eye - center);
    auto cam_x = glm::normalize(glm::cross(up, cam_z));
    auto cam_y = glm::normalize(glm::cross(cam_z, cam_x));

    return  glm::mat4(cam_x.x, cam_y.x, cam_z.x, 0,
                      cam_x.y, cam_y.y, cam_z.y, 0,
                      cam_x.z, cam_y.z, cam_z.z, 0,
                      -center.x, -center.y, -center.z, 1);
}
