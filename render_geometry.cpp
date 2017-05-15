#include "render_geometry.hpp"
#include "viewmatrix.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat3x4.hpp>
#include <glm/vec4.hpp>

static double normal_to_brightness(glm::vec3 normal);
static bool is_face_visible(glm::mat3x4 verts);
static glm::mat3x4 expand_matrix(glm::mat3 m);

void render_line (
        TGAImage& img,
        TGAImage& tex,
        z_buffer& zbuf,
        vec3 start, vec3 dest,
        vec2 start_uv, vec2 end_uv,
        float start_brightness, float end_brightness)
{
    // optionally transpose parameters so that the slope has a greater xdiff
    bool transposed = false;
    if (std::abs(dest.x - start.x) < std::abs(dest.y - start.y))
    {
        std::swap(start.x, start.y);
        std::swap(dest.x, dest.y);
        transposed = true;
    }

    // swap parameters if given in descending order
    if (start.x > dest.x)
    {
        std::swap(start, dest);
        std::swap(start_uv, end_uv);
        std::swap(start_brightness, end_brightness);
    }

    // rates of change (diffs) for each param
    int dx = dest.x - start.x;
    int dy = dest.y - start.y;
    float dz = dest.z - start.z;
    auto d_uv = end_uv - start_uv;
    auto d_brightness = end_brightness - start_brightness;

    // draw the line
    for (int x = start.x; x < dest.x; ++x)
    {
        float t = (x - start.x)/(float)dx;
        int y = (int)(start.y + dy * t);
        float z = start.z + dz * t;
        auto uv = start_uv + d_uv * t;
        auto brightness = start_brightness + d_brightness * t;

        int xcopy = x;
        if (transposed)
        {
            std::swap(xcopy, y);
            std::swap(uv[0], uv[1]);
        }

        float val_in_zbuf = zbuf.get(xcopy, y);
        if (z > val_in_zbuf)
        {
            auto color = tex.get_from_ratio(uv[0], uv[1]);
            color.scale(brightness);
            img.set(xcopy, y, color);
            zbuf.set(xcopy, y, z);
        }
    }
}

void render_triangle (
        TGAImage& img,
        TGAImage& tex,
        z_buffer& zbuf,
        vec3 v1, vec3 v2, vec3 v3,
        vec2 vt1, vec2 vt2, vec2 vt3,
        vec3 vn1, vec3 vn2, vec3 vn3)
{
    // transform the vertex normals into brightness values
    // must clamp because vertex normals not guaranteed to face camera
    auto b1 = glm::clamp(normal_to_brightness(vn1), 0.0, 1.0);
    auto b2 = glm::clamp(normal_to_brightness(vn2), 0.0, 1.0);
    auto b3 = glm::clamp(normal_to_brightness(vn3), 0.0, 1.0);

    // bubble sort input verts so that v1 > v2 > v3 in y axis
    using std::swap;
    if (v3.y > v2.y) { swap(v2, v3); swap(vt2, vt3); swap(b2, b3); }
    if (v2.y > v1.y) { swap(v1, v2); swap(vt1, vt2); swap(b1, b2); }
    if (v3.y > v2.y) { swap(v2, v3); swap(vt2, vt3); swap(b2, b3); }

    // convert to int
    glm::ivec3 iv1 = v1;
    glm::ivec3 iv2 = v2;
    glm::ivec3 iv3 = v3;

    // loop over bottom segment 0 -> y drawing horizontal lines
    auto iv1_3 = iv1 - iv3;
    auto iv2_3 = iv2 - iv3;

    auto dz1_3 = v1.z - v3.z;
    auto dz2_3 = v2.z - v3.z;

    auto total_height = iv1_3.y;
    auto bottom_segment_height = iv2_3.y;

    for (int dy = 0; dy < bottom_segment_height; ++dy)
    {
        auto x0 = iv3.x;
        auto z0 = v3.z;
        
        auto t_alpha = (float)dy / total_height;
        auto x_alpha = (int)(x0 + t_alpha * iv1_3.x);
        auto z_alpha = z0 + t_alpha * dz1_3;
        auto texture_alpha = bary_lerp(vt1, vt2, vt3, vec3(t_alpha, 0, 1 - t_alpha));
        auto brightness_alpha = bary_lerp(b1, b2, b3, vec3(t_alpha, 0, 1 - t_alpha));

        auto t_beta = (float)dy / bottom_segment_height;
        auto x_beta = (int)(x0 + t_beta * iv2_3.x);
        auto z_beta = z0 + t_beta * dz2_3;
        auto texture_beta = bary_lerp(vt1, vt2, vt3, vec3(0, t_beta, 1 - t_beta));
        auto brightness_beta = bary_lerp(b1, b2, b3, vec3(0, t_beta, 1 - t_beta));

        auto y = dy + iv3.y;
        render_line(img, tex, zbuf, {x_alpha, y, z_alpha}, {x_beta, y, z_beta},
                texture_alpha, texture_beta, brightness_alpha, brightness_beta); 
    }

    auto iv1_2 = iv1 - iv2;
    auto dz1_2 = v1.z - v2.z;
    auto final_segment_height = iv1_2.y;
    for (int dy = 0; dy < final_segment_height; ++dy)
    {
        auto alpha0 = iv3.x;
        auto alpha_z0 = v3.z;
        auto beta0 = iv2.x;
        auto beta_z0 = v2.z;
        
        auto t_alpha = (float)(dy + bottom_segment_height) / total_height;
        auto x_alpha = (int)(alpha0 + t_alpha * iv1_3.x);
        auto z_alpha = alpha_z0 + t_alpha * dz1_3;
        auto texture_alpha = bary_lerp(vt1, vt2, vt3, vec3(t_alpha, 0, 1 - t_alpha));
        auto brightness_alpha = bary_lerp(b1, b2, b3, vec3(t_alpha, 0, 1 - t_alpha));

        auto t_beta = (float)dy / final_segment_height;
        auto x_beta = (int)(beta0 + t_beta * iv1_2.x);
        auto z_beta = beta_z0 + t_beta * dz1_2;
        auto texture_beta = bary_lerp(vt1, vt2, vt3, vec3(t_beta, 1 - t_beta, 0));
        auto brightness_beta = bary_lerp(b1, b2, b3, vec3(t_beta, 1 - t_beta, 0));

        auto y = bottom_segment_height + dy + iv3.y;
        render_line(img, tex, zbuf, {x_alpha, y, z_alpha}, {x_beta, y, z_beta},
                texture_alpha, texture_beta, brightness_alpha, brightness_beta); 
    }
}

void render_model (
        TGAImage& img,
        TGAImage& tex,
        z_buffer& zbuf,
        const model& model,
        const mat4 viewmat)
{
    using glm::acos;
    using glm::dot;
    using glm::cross;
    using glm::length;

    for (auto& face : model.faces)
    {
        auto verts = model.get_verts(face);
        if (is_face_visible(verts))
        {
            auto projected_verts = retroproject(viewmat * expand_matrix(verts));
            auto texture_verts = model.get_texture_verts(face);
            auto vert_norms = model.get_vertex_normals(face);
            render_triangle(img, tex, zbuf,
                    projected_verts[0], projected_verts[1], projected_verts[2],
                    texture_verts[0], texture_verts[1], texture_verts[2],
                    vert_norms[0], vert_norms[1], vert_norms[2]);
        }
    }
}

static bool is_face_visible(glm::mat3x4 verts)
{
    auto normal = face_normal(verts);
    return normal_to_brightness(normal) > 0;
}

static double normal_to_brightness(glm::vec3 normal)
{
    static const auto half_pi = glm::half_pi<double>();
    static const auto to_cam = glm::vec3(0, 0, 1);
    auto angle_between = acos(dot(to_cam, normal) / length(normal));
    return angle_between == 0 ? 1 : (half_pi - angle_between) / half_pi;
}

static glm::mat3x4 expand_matrix(glm::mat3 m)
{
    return glm::mat3x4(m[0][0], m[0][1], m[0][2], 1,
                       m[1][0], m[1][1], m[1][2], 1,
                       m[2][0], m[2][1], m[2][2], 1);
}
