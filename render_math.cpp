#include "render_math.hpp"

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

