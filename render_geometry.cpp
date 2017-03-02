#include "render_geometry.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

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

        float val_in_zbuf = get_z_buffer(zbuf, xcopy, y);
        if (z > val_in_zbuf)
        {
            auto color = tex.get_from_ratio(uv[0], uv[1]);
            color.scale(brightness);
            img.set(xcopy, y, color);
            set_z_buffer(zbuf, xcopy, y, z);
        }
    }
}

static vec2 bary_lerp(vec2 v0, vec2 v1, vec2 v2, vec3 bary)
{
    return v0 * bary[0] + v1 * bary[1] + v2 * bary[2];
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
        vec2 vt3,
        const vec3 scale,
        const vec3 origin)
{
    // bubble sort input verts so that v1 > v2 > v3 in y axis

    using std::swap;
    if (v3.y > v2.y) { swap(v2, v3); swap(vt2, vt3); }
    if (v2.y > v1.y) { swap(v1, v2); swap(vt1, vt2); }
    if (v3.y > v2.y) { swap(v2, v3); swap(vt2, vt3); }

    // scale and convert to int
    auto iv1 = ivec3 { scale.x * v1.x, scale.y * v1.y, scale.z * v1.z };
    auto iv2 = ivec3 { scale.x * v2.x, scale.y * v2.y, scale.z * v2.z };
    auto iv3 = ivec3 { scale.x * v3.x, scale.y * v3.y, scale.z * v3.z };

    // loop over bottom segment 0 -> y drawing horizontal lines
    auto iv1_3 = iv1 - iv3;
    auto iv2_3 = iv2 - iv3;

    auto dz1_3 = v1.z - v3.z;
    auto dz2_3 = v2.z - v3.z;

    auto total_height = iv1_3.y;
    auto bottom_segment_height = iv2_3.y;

    for (int dy = 0; dy < bottom_segment_height; ++dy)
    {
        auto x0 = origin.x + iv3.x;
        auto z0 = origin.z + v3.z;
        
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

        // TODO (scott): check if unnecessary since the same swap happens in
        // render line
        if (x_alpha > x_beta)
        {
            swap(x_alpha, x_beta);
            swap(z_alpha, z_beta);
            swap(texture_alpha, texture_beta);
        }
        auto y = dy + iv3.y + (int)origin.y;
        render_line(img, tex, brightness, zbuf, {x_alpha, y, z_alpha},
                {x_beta, y, z_beta}, texture_alpha, texture_beta); 
    }

    auto iv1_2 = iv1 - iv2;
    auto dz1_2 = v1.z - v2.z;
    auto final_segment_height = iv1_2.y;
    for (int dy = 0; dy < final_segment_height; ++dy)
    {
        auto alpha0 = origin.x + iv3.x;
        auto alpha_z0 = origin.z + v3.z;
        auto beta0 = origin.x + iv2.x;
        auto beta_z0 = origin.z + v2.z;
        
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

        if (x_alpha > x_beta)
        {
            std::swap(x_alpha, x_beta);
            std::swap(z_alpha, z_beta);
            swap(texture_alpha, texture_beta);
        }

        auto y = bottom_segment_height + dy + iv3.y + (int)origin.y;
        render_line(img, tex, brightness, zbuf, {x_alpha, y, z_alpha},
                {x_beta, y, z_beta}, texture_alpha, texture_beta); 
    }

    /*
    render_line(img, zbuf, black, v1, v2, scale, origin);
    render_line(img, zbuf, black, v2, v3, scale, origin);
    render_line(img, zbuf, black, v1, v3, scale, origin);
    */
}


void render_model (
        TGAImage& img,
        TGAImage& tex,
        z_buffer& zbuf,
        const model& model,
        const vec3 scale,
        const vec3 origin)
{
    using glm::acos;
    using glm::dot;
    using glm::cross;

    int i = 0;
    for (auto& face : model.faces)
    {
        auto v0 = model.verts[face.vi[0]];
        auto v1 = model.verts[face.vi[1]];
        auto v2 = model.verts[face.vi[2]];

        auto v1_0 = v1 - v0;
        auto v2_1 = v2 - v1;
        auto face_normal = glm::cross(v1_0, v2_1);

        auto to_camera_vector = vec3(0, 0, 1);
        auto angle_between = acos(dot(to_camera_vector, face_normal) /
                glm::length(face_normal));
        auto brightness = angle_between == 0 ? 1 :
            (glm::half_pi<double>() - angle_between) / glm::half_pi<double>();
        if (brightness > 0)
        {
            auto vt0 = model.text_verts[face.vti[0]];
            auto vt1 = model.text_verts[face.vti[1]];
            auto vt2 = model.text_verts[face.vti[2]];
            render_triangle(img, tex, brightness, zbuf, v0, v1, v2, vt0, vt1,
                    vt2, scale, origin);
        }
    }
}
