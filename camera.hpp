#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glm/vec3.hpp>
using glm::vec3;

vec3 camera_position(void);
vec3 camera_target(void);
vec3 camera_up(void);
float camera_distance(void);

#endif // __CAMERA_H__
