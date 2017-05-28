#include "render_math.hpp"

vec3 barycentric(vec2 a, vec2 b, vec2 c, vec2 p)
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

