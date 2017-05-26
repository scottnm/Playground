#ifndef __RENDER_GEOMETRY_H__
#define __RENDER_GEOMETRY_H__

#include "ishader.hpp"
#include "model.hpp"
#include "tgaimage.hpp"
#include "z_buffer.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

using glm::mat4;
using glm::ivec3;
using glm::vec3;
using glm::vec2;

void render_triangle (
        TGAImage& img,
        z_buffer& zbuf,
        const ishader& shader,
        vec3 v1, vec3 v2, vec3 v3,
        vec2 vt1, vec2 vt2, vec2 vt3,
        vec3 vn1, vec3 vn2, vec3 vn3);

void render_model (
        TGAImage& img,
        z_buffer& zbuf,
        const model& model,
        const mat4& viewmat,
        const ishader& shader);

#endif 
