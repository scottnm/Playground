#include "render_geometry.hpp"
#include "viewmatrix.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/vec4.hpp>

static glm::vec3 get_face_normal(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
static double orientation_to_brightness(glm::vec3 face_normal);

vec2 bary_lerp(vec2 v0, vec2 v1, vec2 v2, vec3 bary)
{
    return v0 * bary[0] + v1 * bary[1] + v2 * bary[2];
}

void render_line (
        TGAImage& img,
        TGAImage& tex,
        float brightness,
        z_buffer& zbuf,
        vec3 start,
        vec3 dest,
        vec2 startuv,
        vec2 enduv)
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
        std::swap(startuv, enduv);
    }

    // draw the line
    int dx = dest.x - start.x;
    int dy = dest.y - start.y;
    float dz = dest.z - start.z;
    auto duv = enduv - startuv;

    for (int x = start.x; x < dest.x; ++x)
    {
        float t = (x - start.x)/(float)dx;
        int y = (int)(start.y + dy * t);
        float z = start.z + dz * t;
        auto uv = startuv + duv * t;

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
        float brightness,
        z_buffer& zbuf,
        vec3 v1,
        vec3 v2,
        vec3 v3,
        vec2 vt1,
        vec2 vt2,
        vec2 vt3)
{
    // bubble sort input verts so that v1 > v2 > v3 in y axis
    using std::swap;
    if (v3.y > v2.y) { swap(v2, v3); swap(vt2, vt3); }
    if (v2.y > v1.y) { swap(v1, v2); swap(vt1, vt2); }
    if (v3.y > v2.y) { swap(v2, v3); swap(vt2, vt3); }

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
        auto texture_alpha = bary_lerp(vt1, vt2, vt3, 
                vec3 {t_alpha, 0, 1 - t_alpha});

        auto t_beta = (float)dy / bottom_segment_height;
        auto x_beta = (int)(x0 + t_beta * iv2_3.x);
        auto z_beta = z0 + t_beta * dz2_3;
        auto texture_beta = bary_lerp(vt1, vt2, vt3,
                vec3 {0, t_beta, 1 - t_beta});

        auto y = dy + iv3.y;
        render_line(img, tex, brightness, zbuf, {x_alpha, y, z_alpha},
                {x_beta, y, z_beta}, texture_alpha, texture_beta); 
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
        auto texture_alpha = bary_lerp(vt1, vt2, vt3,
                vec3 {t_alpha, 0, 1 - t_alpha});

        auto t_beta = (float)dy / final_segment_height;
        auto x_beta = (int)(beta0 + t_beta * iv1_2.x);
        auto z_beta = beta_z0 + t_beta * dz1_2;
        auto texture_beta = bary_lerp(vt1, vt2, vt3,
                vec3 {t_beta, 1 - t_beta, 0});

        auto y = bottom_segment_height + dy + iv3.y;
        render_line(img, tex, brightness, zbuf, {x_alpha, y, z_alpha},
                {x_beta, y, z_beta}, texture_alpha, texture_beta); 
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
        auto v0 = model.verts[face.vi[0]];
        auto v1 = model.verts[face.vi[1]];
        auto v2 = model.verts[face.vi[2]];
        auto face_normal = get_face_normal(v0, v1, v2);
        auto brightness = orientation_to_brightness(face_normal);
        if (brightness > 0)
        {
            auto pv0 = retroproject(viewmat * glm::vec4(v0, 1));
            auto pv1 = retroproject(viewmat * glm::vec4(v1, 1));
            auto pv2 = retroproject(viewmat * glm::vec4(v2, 1));

            auto vt0 = model.text_verts[face.vti[0]];
            auto vt1 = model.text_verts[face.vti[1]];
            auto vt2 = model.text_verts[face.vti[2]];

            render_triangle(img, tex, brightness, zbuf,
                    pv0, pv1, pv2, vt0, vt1, vt2);
        }
    }
}

static glm::vec3 get_face_normal(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2)
{
    auto v1_0 = v1 - v0;
    auto v2_1 = v2 - v1;
    return glm::cross(v1_0, v2_1);
}

static double orientation_to_brightness(glm::vec3 face_normal)
{
    static const auto half_pi = glm::half_pi<double>();
    static const auto to_cam = glm::vec3(0, 0, 1);
    auto angle_between = acos(dot(to_cam, face_normal) / length(face_normal));
    return angle_between == 0 ? 1 : (half_pi - angle_between) / half_pi;
}
