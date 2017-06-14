#include "light.hpp"
#include <glm/glm.hpp>

static const auto light_direction = normalize(vec3(0, 1, -1));

vec3 light_dir(void)
{
    return light_direction;
}

vec3 to_light(void)
{
    return -light_direction;
}
