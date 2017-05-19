#ifndef __VIEWMATRIX_H__
#define __VIEWMATRIX_H__

#include <glm/mat3x3.hpp>
#include <glm/mat3x4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

glm::mat4 perspective_proj_xform(
        float camera_distance);

glm::mat4 screenspace_xform(
        int screen_width,
        int screen_height,
        int x_scale,
        int y_scale);

glm::mat4 lookat_xform(
        glm::vec3 eye,
        glm::vec3 center,
        glm::vec3 up);


glm::mat3 retroproject(
        glm::mat3x4 m);

#endif // __VIEWMATRIX_H__
