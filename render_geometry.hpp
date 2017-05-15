#ifndef __RENDER_GEOMETRY_H__
#define __RENDER_GEOMETRY_H__

#include "model.hpp"
#include "tgaimage.hpp"
#include "z_buffer.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

using glm::mat4;
using glm::ivec3;
using glm::vec3;
using glm::vec2;

void render_line (
        TGAImage& img,
        TGAImage& tex,
        z_buffer& zbuf,
        vec3 start, vec3 dest,
        vec2 start_uv, vec2 end_uv,
        vec3 start_brightness, vec3 end_brightness);

void render_triangle (
        TGAImage& img,
        TGAImage& tex,
        z_buffer& zbuf,
        vec3 v1, vec3 v2, vec3 v3,
        vec2 vt1, vec2 vt2, vec2 vt3,
        vec3 vn1, vec3 vn2, vec3 vn3);

void render_model (
        TGAImage& img,
        TGAImage& tex,
        z_buffer& zbuf,
        const model& model,
        const mat4 viewmat);

template<typename T>
auto bary_lerp(T v0, T v1, T v2, vec3 bary)
{
    return v0 * bary[0] + v1 * bary[1] + v2 * bary[2];
}

#endif 
