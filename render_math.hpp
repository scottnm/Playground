#ifndef __RENDER_MATH_H__
#define __RENDER_MATH_H__

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using glm::dot;
using glm::vec2;
using glm::vec3;

template<typename T>
auto bary_lerp(T v0, T v1, T v2, vec3 bary)
{
    return v0 * bary[0] + v1 * bary[1] + v2 * bary[2];
}

vec3 barycentric(vec2 a, vec2 b, vec2 c, vec2 p);

#endif //__RENDER_MATH_H__
