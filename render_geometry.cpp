#include "camera.hpp"
#include "render_geometry.hpp"
#include "render_math.hpp"
#include "viewmatrix.hpp"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat3x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

static bool is_face_visible(glm::mat3x4 verts);

void render_triangle (
        TGAImage& img,
        z_buffer& zbuf,
        const ishader& shader,
        vec3 v1, vec3 v2, vec3 v3,
        vec2 vt1, vec2 vt2, vec2 vt3,
        vec3 vn1, vec3 vn2, vec3 vn3)
{
    auto minx = std::min(v1.x, std::min(v2.x, v3.x));
    auto miny = std::min(v1.y, std::min(v2.y, v3.y));
    vec2 bboxmin = {minx, miny};

    auto maxx = std::max(v1.x, std::max(v2.x, v3.x));
    auto maxy = std::max(v1.y, std::max(v2.y, v3.y));
    vec2 bboxmax = {maxx, maxy};

    for (int x = bboxmin.x; x < bboxmax.x; ++x)
    {
        for (int y = bboxmin.y; y < bboxmax.y; ++y)
        {
            auto bary = barycentric(vec2(v1), vec2(v2), vec2(v3), vec2(x, y));
            if (bary.x < 0 || bary.y < 0 || bary.z < 0) { continue; }

            auto z_depth = bary_lerp(v1.z, v2.z, v3.z, bary);
            if (z_depth <= zbuf.get(x, y)) { continue; }

            auto frag_color = shader.fragment(bary, {v1, v2, v3},
                    {vt1, vt2, vt3}, {vn1, vn2, vn3});
            if (frag_color.valid)
            {
                img.set(x, y, frag_color.c);
                zbuf.set(x, y, z_depth);
            }
        }
    }
}

void render_model (
        TGAImage& img,
        z_buffer& zbuf,
        const model& model,
        const mat4& viewmat,
        const ishader& shader)
{
    for (auto& face : model.faces)
    {
        auto verts = model.get_verts(face);
        if (is_face_visible(verts))
        {
            verts[0] = shader.vertex(viewmat, verts[0]);
            verts[1] = shader.vertex(viewmat, verts[1]);
            verts[2] = shader.vertex(viewmat, verts[2]);
            auto texture_verts = model.get_texture_verts(face);
            auto vert_norms = model.get_vertex_normals(face);
            render_triangle(img, zbuf, shader,
                    verts[0], verts[1], verts[2],
                    texture_verts[0], texture_verts[1], texture_verts[2],
                    vert_norms[0], vert_norms[1], vert_norms[2]);
        }
    }
}

static bool is_face_visible(glm::mat3x4 verts)
{
    static const auto to_cam = glm::normalize(camera_position() - camera_target());
    return dot(to_cam, face_normal(verts)) > 0;
}
