#include "render_geometry.hpp"

static const TGAColor white = TGAColor(255, 255, 255, 255);
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
        auto x_alpha = (int)(iv3.x + ((float)dy / total_height) * iv1_3.x + origin.x);
        auto x_beta = (int)(iv3.x + ((float)dy / bottom_segment_height) * iv2_3.x + origin.x);
        if (x_alpha > x_beta)
        {
            std::swap(x_alpha, x_beta);
        }
        auto y = dy + iv3.y + (int)origin.y;
        render_line(img, white, {x_alpha, y, 0}, {x_beta, y, 0}); 
    }

    // loop over top segment 0 -> y drawing horizontal lines


    // draw border
    render_line(img, blue, v1, v2, scale, origin);
    render_line(img, green, v2, v3, scale, origin);
    render_line(img, red, v3, v1, scale, origin);
}

void render_model (
        TGAImage& img,
        const model& model,
        const vec3 scale,
        const vec3 origin)
{
    for (auto& face : model.faces)
    {
        render_triangle(img, white, model.verts[face[0]], model.verts[face[1]],
                model.verts[face[2]], scale, origin);
    }
}
