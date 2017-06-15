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

using glm::mat3x4;
using glm::normalize;

static bool is_face_visible(mat3x4 verts, vec3 to_cam);

void render_triangle (
        TGAImage& img,
        z_buffer& zbuf,
        const ishader& shader,
        const mat3& vs,
        const mat3x2& vts,
        const mat3& vns)
{
    auto minx = std::min(vs[0].x, std::min(vs[1].x, vs[2].x));
    auto miny = std::min(vs[0].y, std::min(vs[1].y, vs[2].y));
    vec2 bboxmin = {minx, miny};

    auto maxx = std::max(vs[0].x, std::max(vs[1].x, vs[2].x));
    auto maxy = std::max(vs[0].y, std::max(vs[1].y, vs[2].y));
    vec2 bboxmax = {maxx, maxy};

    for (int x = bboxmin.x; x < bboxmax.x; ++x)
    {
        for (int y = bboxmin.y; y < bboxmax.y; ++y)
        {
            //printf("(%d, %d)\t\t", x, y);
            // check to make sure we are in the triangle
            auto bary = barycentric(vs[0], vs[1], vs[2], vec2(x, y));
            if (bary.x < 0 || bary.y < 0 || bary.z < 0) { continue; }

            // check to make sure the fragment is above the zdepth
            auto v_interpolated = bary_lerp(vs[0], vs[1], vs[2], bary);
            auto z_depth = v_interpolated.z; //bary_lerp(vs[0].z, vs[1].z, vs[2].z, bary);
            if (z_depth <= zbuf.get(x, y)) { continue; }

            auto frag_color = shader.fragment(bary, vs, vts, vns);
            if (frag_color.valid)
            {
                img.set(x, y, frag_color.c);
                zbuf.set(x, y, z_depth);
                printf("Setting zbuf :: [ %f ] @ <%d, %d> from floating <%f, %f> which rounds too <%d, %d>\n",
                        z_depth, x, y, v_interpolated.x, v_interpolated.y, (int)v_interpolated.x, (int)v_interpolated.y);
            }
            //printf("\n");
        }
    }
}

#include <cstdio>
void render_model (
        TGAImage& img,
        z_buffer& zbuf,
        const model& model,
        const mat4& viewmat,
        const ishader& shader,
        const vec3 to_cam)
{
    for (auto& face : model.faces)
    {
        auto verts = model.get_verts(face);
        auto verts_copy = verts;
        if (is_face_visible(verts, to_cam))
        {
            verts[0] = shader.vertex(viewmat, verts[0]);
            verts[1] = shader.vertex(viewmat, verts[1]);
            verts[2] = shader.vertex(viewmat, verts[2]);

            printf("A: <%f, %f, %f> -> <%f, %f, %f>\n"
                   "B: <%f, %f, %f> -> <%f, %f, %f>\n"
                   "C: <%f, %f, %f> -> <%f, %f, %f>\n",
                   verts_copy[0].x, verts_copy[0].y, verts_copy[0].z, verts[0].x, verts[0].y, verts[0].z,
                   verts_copy[1].x, verts_copy[1].y, verts_copy[1].z, verts[1].x, verts[1].y, verts[1].z,
                   verts_copy[2].x, verts_copy[2].y, verts_copy[2].z, verts[2].x, verts[2].y, verts[2].z);

            auto texture_verts = model.get_texture_verts(face);
            auto vert_norms = model.get_vertex_normals(face);
            render_triangle(img, zbuf, shader, verts, texture_verts, vert_norms);
        }
    }
}

static bool is_face_visible(mat3x4 verts, vec3 to_cam)
{
    return dot(to_cam, face_normal(verts)) > 0;
}
