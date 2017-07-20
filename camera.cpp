#include "camera.hpp"
#include <glm/glm.hpp>

static const auto eye = glm::vec3(1, 1, 4);
static const auto target = glm::vec3(0, 0, 0);
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
