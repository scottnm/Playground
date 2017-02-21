#ifndef __RENDER_GEOMETRY_H__
#define __RENDER_GEOMETRY_H__

#include "model.hpp"
#include "tgaimage.hpp"
#include <glm/vec2.hpp>

using glm::ivec2;
using glm::vec3;

void render_line (
        TGAImage& img,
        const TGAColor color,
        const vec3 start,
        const vec3 dest,
        const vec3 scale,
        const vec3 origin);

void render_triangle (
        TGAImage& img,
        const TGAColor color,
        const vec3 v1,
        const vec3 v2,
        const vec3 v3,
        const vec3 scale,
        const vec3 origin);

void render_model (
        TGAImage& img,
        const model& model,
        const vec3 scale,
        const vec3 origin);

#endif 
