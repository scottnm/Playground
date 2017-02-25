#include "render_geometry.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

static const TGAColor white = TGAColor(255, 255, 255, 255);
static const TGAColor black = TGAColor(0, 0, 0, 255);
static const TGAColor red   = TGAColor(255, 0, 0, 255);
static const TGAColor green = TGAColor(0, 255, 0, 255);
static const TGAColor blue  = TGAColor(0, 0, 255, 255);

void render_line (
    TGAImage& img,
    const TGAColor color,
    const vec3 start,
    const vec3 dest,
    const vec3 scale,
    const vec3 origin)
{
    ivec3 istart = {start.x * scale.x + origin.x, start.y * scale.y + origin.y, 0};
    ivec3 idest  = {dest.x * scale.x + origin.x, dest.y * scale.y + origin.y, 0};
    render_line(img, color, istart, idest);
}

void render_line (
        TGAImage& img,
        const TGAColor color,
        ivec3 start,
        ivec3 dest)
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
    }

    // draw the line
    int dx = dest.x - start.x;
    int dy = dest.y - start.y;
    for (int x = start.x; x < dest.x; ++x)
    {
        float t = (x - start.x)/(float)dx;
        int y = start.y + dy * t;
        if (transposed)
        {
            img.set(y, x, color);
        }
        else
        {
            img.set(x, y, color);
        }
    }
}


void render_triangle (
        TGAImage& img,
        const TGAColor color,
        vec3 v1,
        vec3 v2,
        vec3 v3,
        const vec3 scale,
        const vec3 origin)
{
    // bubble sort input verts so that v1 > v2 > v3 in y axis
    if (v3.y > v2.y) { std::swap(v2, v3); }
    if (v2.y > v1.y) { std::swap(v1, v2); }
    if (v3.y > v2.y) { std::swap(v2, v3); }

    // scale and convert to int
    auto iv1 = ivec3 { scale.x * v1.x, scale.y * v1.y, scale.z * v1.z };
    auto iv2 = ivec3 { scale.x * v2.x, scale.y * v2.y, scale.z * v2.z };
    auto iv3 = ivec3 { scale.x * v3.x, scale.y * v3.y, scale.z * v3.z };

    // loop over bottom segment 0 -> y drawing horizontal lines
    auto iv1_3 = iv1 - iv3;
    auto iv2_3 = iv2 - iv3;
    auto total_height = iv1_3.y;
    auto bottom_segment_height = iv2_3.y;
    for (int dy = 0; dy < bottom_segment_height; ++dy)
    {
        auto x0 = origin.x + iv3.x;
        
        auto t_alpha = (float)dy / total_height;
        auto x_alpha = (int)(x0 + t_alpha * iv1_3.x);

        auto t_beta = (float)dy / bottom_segment_height;
        auto x_beta = (int)(x0 + t_beta * iv2_3.x);

        if (x_alpha > x_beta)
        {
            std::swap(x_alpha, x_beta);
        }
        auto y = dy + iv3.y + (int)origin.y;
        render_line(img, color, {x_alpha, y, 0}, {x_beta, y, 0}); 
    }

    auto iv1_2 = iv1 - iv2;
    auto final_segment_height = iv1_2.y;
    for (int dy = 0; dy < final_segment_height; ++dy)
    {
        auto alpha0 = origin.x + iv3.x;
        auto beta0 = origin.x + iv2.x;
        
        auto t_alpha = (float)(dy + bottom_segment_height) / total_height;
        auto x_alpha = (int)(alpha0 + t_alpha * iv1_3.x);

        auto t_beta = (float)dy / final_segment_height;
        auto x_beta = (int)(beta0 + t_beta * iv1_2.x);

        if (x_alpha > x_beta)
        {
            std::swap(x_alpha, x_beta);
        }
        auto y = bottom_segment_height + dy + iv3.y + (int)origin.y;
        render_line(img, color, {x_alpha, y, 0}, {x_beta, y, 0}); 
    }

    render_line(img, black, v1, v2, scale, origin);
    render_line(img, black, v2, v3, scale, origin);
    render_line(img, black, v1, v3, scale, origin);
}


void render_model (
        TGAImage& img,
        const model& model,
        const vec3 scale,
        const vec3 origin)
{
    using glm::acos;
    using glm::dot;

    int i = 0;
    for (auto& face : model.faces)
    {
        auto v0 = model.verts[face.vi[0]];
        auto v1 = model.verts[face.vi[1]];
        auto v2 = model.verts[face.vi[2]];

        auto vn0 = model.vert_norms[face.vni[0]];
        auto vn1 = model.vert_norms[face.vni[1]];
        auto vn2 = model.vert_norms[face.vni[2]];

        auto face_normal = (vn0 + vn1 + vn2) / 3.0f;
        auto to_camera_vector = vec3(0, 0, 1);
        auto angle_between = acos(dot(to_camera_vector, face_normal) /
                glm::length(face_normal));
        auto brightness = angle_between == 0 ? 1 :
            (glm::half_pi<double>() - angle_between) / glm::half_pi<double>();
        if (brightness > 0)
        {
            auto normal_color = TGAColor(255 * brightness, 255 * brightness,
                    255 * brightness, 255);
            render_triangle(img, normal_color, v0, v1, v2, scale, origin);
        }
    }
}
