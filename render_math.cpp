#include "render_math.hpp"

/*
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
*/

vec3 barycentric(vec2 a, vec2 b, vec2 c, vec2 p)
{
    vec3 s[2];
    for (int i=2; i--; ) {
        s[i][0] = c[i]-a[i];
        s[i][1] = b[i]-a[i];
        s[i][2] = a[i]-p[i];
    }
    vec3 u = cross(s[0], s[1]);
    // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
    if (std::abs(u[2])>1e-2)
    {
        return vec3(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    }
    // in this case generate negative coordinates, it will be thrown away by the rasterizator
    return vec3(-1,1,1); 
}

