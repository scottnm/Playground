#ifndef __RENDER_GEOMETRY_H__
#define __RENDER_GEOMETRY_H__

#include "ishader.hpp"
#include "model.hpp"
#include "tgaimage.hpp"
#include "z_buffer.hpp"
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

using glm::mat3;
using glm::mat4;
using glm::ivec3;
using glm::vec3;
using glm::vec2;

void render_triangle (
        TGAImage& img,
        z_buffer& zbuf,
        const ishader& shader,
        const mat3& vs,
        const mat3x2& vts,
        const mat3& vns);

void render_model (
        TGAImage& img,
        z_buffer& zbuf,
        const model& model,
        const mat4& viewmat,
        const ishader& shader,
        const vec3 to_cam);

#endif 
