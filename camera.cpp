#include "camera.hpp"
#include <glm/glm.hpp>

static const auto eye = glm::vec3(1.25, 1, 1.25);
static const auto target = glm::vec3(-0.2, 0, 0.1);
static const auto up = glm::vec3(0, 1, 0);

vec3 to_camera(void)
{
    return eye - target;
}

vec3 camera_position(void)
{
    return eye;
}

vec3 camera_target(void)
{
    return target;
}

vec3 camera_up(void)
{
    return up;
}

float camera_distance(void)
{
    return glm::length(eye - target);
}
