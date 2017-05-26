#include "camera.hpp"
#include "render_math.hpp"
#include "simple_texture_shader.hpp"

#include <glm/glm.hpp>
#include <glm/vec4.hpp>

using glm::clamp;
using glm::normalize;
using glm::dot;

static const auto to_light = glm::normalize(camera_position() - camera_target());

simple_texture_shader::simple_texture_shader(const string& texture_name)
{
    tex.read_tga_file(texture_name.c_str());
    tex.flip_vertically();
}

vec3 simple_texture_shader::vertex(const mat4& viewmat, vec3 v) const
{
    auto res = viewmat * glm::vec4(v, 1);
    return res / res.w;
}

frag_color simple_texture_shader::fragment(vec3 bary, mat3 verts,
        mat3x2 tex_coords, mat3 vert_norms) const
{
    auto pos = bary_lerp(verts[0], verts[1], verts[2], bary);
    auto uv = bary_lerp(tex_coords[0], tex_coords[1], tex_coords[2], bary);
    auto norm = bary_lerp(vert_norms[0], vert_norms[1], vert_norms[2], bary);

    float intensity = clamp(0.0f, 1.0f, dot(normalize(to_light), normalize(norm)));
    auto color = tex.get_from_ratio(uv[0], uv[1]);
    color.scale(intensity);

    return frag_color {color, intensity > 0};
}
