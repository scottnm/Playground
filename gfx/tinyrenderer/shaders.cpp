#include "camera.hpp"
#include "light.hpp"
#include "render_math.hpp"
#include "shaders.hpp"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x2.hpp>
#include <cmath>

using glm::dot;
using glm::inverse;
using glm::mat2;
using glm::mat3x2;
using glm::normalize;
using glm::reflect;
using glm::transpose;
using glm::vec4;

using std::abs;
using std::max;
using std::pow;

static const auto to_cam = glm::normalize(to_camera());

vec3 simple_normal_shader::vertex(
        const mat4& viewmat,
        const vec3& v) const
{
    auto res = viewmat * vec4(v, 1);
    return res / res.w;
}

frag_color simple_normal_shader::fragment(
        const vec3& bary,
        const mat3& verts,
        const mat3x2& tex_coords,
        const mat3& vert_norms) const
{
    vec3 norm_color = normalize(bary_lerp(vert_norms[0], vert_norms[1],
                vert_norms[2], bary)) * 255.0f;
    TGAColor c(abs(norm_color.x), abs(norm_color.y), abs(norm_color.z), 255);

    return frag_color {c, true};
}

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
    auto res = viewmat * vec4(v, 1);
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
    float intensity = max(0.0f, dot(to_light(), normal));

    vec3 norm_color = normalize(bary_lerp(vert_norms[0], vert_norms[1],
                vert_norms[2], bary)) * 255.0f;
    TGAColor c(abs(norm_color.x), abs(norm_color.y), abs(norm_color.z), 255);
    c.scale(intensity);

    return frag_color {c, true};
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
    auto res = viewmat * vec4(v, 1);
    return res / res.w;
}

frag_color simple_texture_shader::fragment(
        const vec3& bary,
        const mat3& verts,
        const mat3x2& tex_coords,
        const mat3& vert_norms) const
{
    auto norm = bary_lerp(vert_norms[0], vert_norms[1], vert_norms[2], bary);
    float intensity = max(0.0f, dot(to_light(), normalize(norm)));
    auto uv = bary_lerp(tex_coords[0], tex_coords[1], tex_coords[2], bary);
    auto color = tex.get_from_ratio(uv[0], uv[1]);
    color.scale(intensity);

    return frag_color {color, true};
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
    auto res = viewmat * vec4(v, 1);
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
    float intensity = max(0.0f, dot(to_light(), norm));

    auto color = tex.get_from_ratio(uv[0], uv[1]);
    color.scale(intensity);

    return frag_color {color, true};
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
    auto res = viewmat * vec4(v, 1);
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
    auto r = normalize(reflect(light_dir(), norm));
    auto spec_intensity = pow(max(0.0f, dot(r, to_cam)), spec_val);

    // diffuse color
    float diff_intensity = max(0.0f, dot(to_light(), norm));
    auto diff_color = diffuse.get_from_ratio(uv[0], uv[1]);

    diff_color.scale(diff_intensity + spec_intensity * .6);

    return frag_color {ambient + diff_color, true};
}



//
// PHONG TANGENT SPACE TEXTURE SHADER
//

phong_tangent_space_shader::phong_tangent_space_shader(
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

vec3 phong_tangent_space_shader::vertex(
        const mat4& viewmat,
        const vec3& v) const
{
    auto res = viewmat * vec4(v, 1);
    return res / res.w;
}

frag_color phong_tangent_space_shader::fragment(
        const vec3& bary,
        const mat3& verts,
        const mat3x2& tex_coords,
        const mat3& vert_norms) const
{
    auto uv = bary_lerp(tex_coords[0], tex_coords[1], tex_coords[2], bary);
    auto objspace_norm = bary_lerp(vert_norms[0], vert_norms[1], vert_norms[2], bary);
    auto ts_normval = normalmap.get_from_ratio(uv[0], uv[1]);
    auto tanspace_norm = normalize(vec3(ts_normval.r - 127.5,
                                        ts_normval.g - 127.5,
                                        ts_normval.b - 127.5));

    // objspace and texture space verts
    auto o1 = verts[1] - verts[0];
    auto o2 = verts[2] - verts[0];
    auto t1 = tex_coords[1] - tex_coords[0];
    auto t2 = tex_coords[2] - tex_coords[0];

    auto tanspace_mat = mat2(t1.x, t2.x,
                             t1.y, t2.y);
    auto objspace_mat = mat3x2(o1.x, o2.x,
                               o1.y, o2.y,
                               o1.z, o2.z);

    // S * t1.x + T * t1.y = o1
    // S * t2.x + T * t2.y = o2
    //
    // [t1.x    t1.y]  *  [tan      =  [o1.x    o1.y   o1.z]
    //  t2.x    t2.y]      bitan]       o2.x    o2.y   o2.z]
    //
    //  tanspace_mat * tan_bitan_mat = objspace_mat
    //  inverse(tanspace_mat) * tanspace_mat * tan_bitan_mat = inverse(tanspace_mat) * objspace_mat
    //  I * tan_bitan_mat = inverse(tansapce_mat) * objspace_mat
    //  tan_bitan_mat = inverse(tanspace_mat) * objspace_mat
    auto tan_bitan_mat = transpose(inverse(tanspace_mat) * objspace_mat);
    auto tan_to_objspace = mat3(normalize(tan_bitan_mat[0]),
                                normalize(tan_bitan_mat[1]),
                                normalize(objspace_norm));
    auto norm = normalize(tan_to_objspace * tanspace_norm);

    // ambient color
    TGAColor ambient(5, 5, 5, 255);

    // specular color
    auto spec_val = specular.get_from_ratio(uv[0], uv[1]).raw[0];
    auto r = normalize(reflect(light_dir(), norm));
    auto spec_intensity = pow(max(0.0f, dot(r, to_cam)), spec_val);

    // diffuse color
    float diff_intensity = max(0.0f, dot(to_light(), norm));
    auto diff_color = diffuse.get_from_ratio(uv[0], uv[1]);

    diff_color.scale(diff_intensity + spec_intensity * .6);

    return frag_color {ambient + diff_color, true};
}



//
// SHADOW SHADER
//
vec3 shadow_shader::vertex(
        const mat4& viewmat,
        const vec3& v) const
{
    auto res = viewmat * vec4(v, 1);
    return res / res.w;
}

frag_color shadow_shader::fragment(
        const vec3& bary,
        const mat3& verts,
        const mat3x2& tex_coords,
        const mat3& vert_norms) const
{
    static const auto light_len = length(to_light());

    auto vert = bary_lerp(verts[0], verts[1], verts[2], bary);
    auto intensity = vert.z / light_len;
    auto c = TGAColor(255, 255, 255, 255);
    c.scale(intensity);
    return frag_color {c, true};
}


//
// SHADER WITH SHADOWS
//

shader_with_shadows::shader_with_shadows(
        unique_ptr<ishader> shader,
        const z_buffer& shadow_zbuf,
        const mat4& shadow_xform)
    : primary_shader(std::move(shader)), shadow_zbuf(shadow_zbuf), shadow_xform(shadow_xform) {}

vec3 shader_with_shadows::vertex(
        const mat4& viewmat,
        const vec3& v) const
{
    auto res = viewmat * vec4(v, 1);
    return res / res.w;
}

frag_color shader_with_shadows::fragment(
        const vec3& bary,
        const mat3& verts,
        const mat3x2& tex_coords,
        const mat3& vert_norms) const
{
    auto v = bary_lerp(verts[0], verts[1], verts[2], bary);
    auto shadow_v = shadow_xform * vec4(v, 1);
    shadow_v = shadow_v / shadow_v.w;

    auto shadow_z_val = shadow_zbuf.get((int)shadow_v.x, (int)shadow_v.y);
    float shadow_coeff = .3 + .7 * (shadow_z_val < shadow_v.z);
    auto frag_c = primary_shader->fragment(bary, verts, tex_coords, vert_norms);
    frag_c.c.scale(shadow_coeff);
    return frag_c;
}
