#ifndef __RENDER_MATH_H__
#define __RENDER_MATH_H__

#include <glm/glm.hpp>

using glm::dot;

template<typename T>
auto bary_lerp(T v0, T v1, T v2, vec3 bary)
{
    return v0 * bary[0] + v1 * bary[1] + v2 * bary[2];
}

template <typename T>
vec3 barycentric(T a, T b, T c, T p)
{
    auto v0 = b - a;
    auto v1 = c - a;
    auto v2 = p - a;
    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);
    float denom = (d00 * d11) - (d01 * d01);
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;
    return {u, v, w};
} 

#endif //__RENDER_MATH_H__
