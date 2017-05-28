#include "camera.hpp"
#include "render_math.hpp"
#include "shaders.hpp"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <cmath>

using glm::normalize;
using glm::reflect;
using glm::dot;
using std::abs;
using std::max;
using std::pow;

static const auto to_light = glm::normalize(camera_position() - camera_target());
static const auto to_cam = glm::normalize(camera_position() - camera_target());

//
// NORMAL SHADER
//
normal_shader::normal_shader(
        const string& normalmap_name)
{
    normalmap.read_tga_file(normalmap_name.c_str());
    normalmap.flip_vertically();
}


vec3 normal_shader::vertex(
        const mat4& viewmat,
        const vec3& v) const
{
    auto res = viewmat * glm::vec4(v, 1);
    return res / res.w;
}

frag_color normal_shader::fragment(
        const vec3& bary,
        const mat3& verts,
        const mat3x2& tex_coords,
        const mat3& vert_norms) const
{
    auto uv = bary_lerp(tex_coords[0], tex_coords[1], tex_coords[2], bary);
    auto normval = normalmap.get_from_ratio(uv[0], uv[1]);
    auto normal = normalize(vec3(normval.r - 127.5, normval.g - 127.5, normval.b - 127.5));
    float intensity = max(0.0f, dot(to_light, normal));

    vec3 norm_color = normalize(bary_lerp(vert_norms[0], vert_norms[1],
                vert_norms[2], bary)) * 255.0f;
    TGAColor c(abs(norm_color.x), abs(norm_color.y), abs(norm_color.z), 255);
    c.scale(intensity);

    return frag_color {c, intensity > 0};
}

//
// SIMPLE TEXTURE SHADER
//

simple_texture_shader::simple_texture_shader(
        const string& texture_name)
{
    tex.read_tga_file(texture_name.c_str());
    tex.flip_vertically();
}

vec3 simple_texture_shader::vertex(
        const mat4& viewmat,
        const vec3& v) const
{
    auto res = viewmat * glm::vec4(v, 1);
    return res / res.w;
}

frag_color simple_texture_shader::fragment(
        const vec3& bary,
        const mat3& verts,
        const mat3x2& tex_coords,
        const mat3& vert_norms) const
{
    auto norm = bary_lerp(vert_norms[0], vert_norms[1], vert_norms[2], bary);
    float intensity = max(0.0f, dot(to_light, normalize(norm)));
    auto uv = bary_lerp(tex_coords[0], tex_coords[1], tex_coords[2], bary);
    auto color = tex.get_from_ratio(uv[0], uv[1]);
    color.scale(intensity);

    return frag_color {color, intensity > 0};
}


//
// BUMPED TEXTURE SHADER
//

bumped_texture_shader::bumped_texture_shader(
        const string& texture_name,
        const string& normalmap_name)
{
    tex.read_tga_file(texture_name.c_str());
    tex.flip_vertically();
    normalmap.read_tga_file(normalmap_name.c_str());
    normalmap.flip_vertically();
}

vec3 bumped_texture_shader::vertex(
        const mat4& viewmat,
        const vec3& v) const
{
    auto res = viewmat * glm::vec4(v, 1);
    return res / res.w;
}


frag_color bumped_texture_shader::fragment(
        const vec3& bary,
        const mat3& verts,
        const mat3x2& tex_coords,
        const mat3& vert_norms) const
{
    auto uv = bary_lerp(tex_coords[0], tex_coords[1], tex_coords[2], bary);
    auto normval = normalmap.get_from_ratio(uv[0], uv[1]);
    auto norm = normalize(vec3(normval.r - 127.5, normval.g - 127.5, normval.b - 127.5));
    float intensity = max(0.0f, dot(to_light, norm));

    auto color = tex.get_from_ratio(uv[0], uv[1]);
    color.scale(intensity);

    return frag_color {color, intensity > 0};
}


//
// PHONG TEXTURE SHADER
//

phong_shader::phong_shader(
        const string& diffuse_name,
        const string& normalmap_name,
        const string& specular_name)
{
    diffuse.read_tga_file(diffuse_name.c_str());
    diffuse.flip_vertically();
    normalmap.read_tga_file(normalmap_name.c_str());
    normalmap.flip_vertically();
    specular.read_tga_file(specular_name.c_str());
    specular.flip_vertically();
}

vec3 phong_shader::vertex(
        const mat4& viewmat,
        const vec3& v) const
{
    auto res = viewmat * glm::vec4(v, 1);
    return res / res.w;
}

frag_color phong_shader::fragment(
        const vec3& bary,
        const mat3& verts,
        const mat3x2& tex_coords,
        const mat3& vert_norms) const
{
    auto uv = bary_lerp(tex_coords[0], tex_coords[1], tex_coords[2], bary);
    auto normval = normalmap.get_from_ratio(uv[0], uv[1]);
    auto norm = normalize(vec3(normval.r - 127.5, normval.g - 127.5, normval.b - 127.5));

    // ambient color
    TGAColor ambient(5, 5, 5, 255);

    // specular color
    auto spec_val = specular.get_from_ratio(uv[0], uv[1]).raw[0];
    auto r = normalize(reflect(-to_light, norm));
    auto spec_intensity = pow(max(0.0f, dot(r, to_cam)), spec_val);

    // diffuse color
    float diff_intensity = max(0.0f, dot(to_light, norm));
    auto diff_color = diffuse.get_from_ratio(uv[0], uv[1]);

    diff_color.scale(diff_intensity + spec_intensity * .6);

    return frag_color {ambient + diff_color, diff_intensity > 0};
}
