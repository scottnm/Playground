#ifndef __RENDER_GEOMETRY_H__
#define __RENDER_GEOMETRY_H__

#include "model.hpp"
#include "tgaimage.hpp"
#include "z_buffer.hpp"
#include <glm/vec2.hpp>

using glm::ivec3;
using glm::vec3;
using glm::vec2;

void render_line (
        TGAImage& img,
        TGAImage& tex,
        float brightness,
        z_buffer& zbuf,
        vec3 s,
        vec3 d);

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
        vec2 vt3,
        const vec3 scale,
        const vec3 origin);

void render_model (
        TGAImage& img,
        TGAImage& tex,
        z_buffer& zbuf,
        const model& model,
        const vec3 scale,
        const vec3 origin);

#endif 
