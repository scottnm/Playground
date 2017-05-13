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
        float brightness,
        z_buffer& zbuf,
        vec3 start,
        vec3 dest,
        vec2 startuv,
        vec2 enduv);

void render_triangle (
        TGAImage& img,
        TGAImage& tex,
        float brightness,
        z_buffer& zbuf,
        vec3 v1,
        vec3 v2,
        vec3 v3,
        vec2 vt1,
        vec2 vt2,
        vec2 vt3);

void render_model (
        TGAImage& img,
        TGAImage& tex,
        z_buffer& zbuf,
        const model& model,
        const mat4 viewmat);

vec2 bary_lerp(
        vec2 v0,
        vec2 v1,
        vec2 v2,
        vec3 bary);

#endif 
