#include "render_geometry.hpp"

static const TGAColor white = TGAColor(255, 255, 255, 255);
static const TGAColor red   = TGAColor(255, 0,   0,   255);


void render_line (
    TGAImage& img,
    const TGAColor color,
    const vec3 start,
    const vec3 dest,
    const vec3 scale,
    const vec3 origin)
{
    ivec2 istart = {start.x * scale.x + origin.x, start.y * scale.y + origin.y};
    ivec2 idest  = {dest.x * scale.x + origin.x, dest.y * scale.y + origin.y};

    // optionally transpose parameters so that the slope has a greater xdiff
    bool transposed = false;
    if (std::abs(idest.x - istart.x) < std::abs(idest.y - istart.y))
    {
        std::swap(istart.x, istart.y);
        std::swap(idest.x, idest.y);
        transposed = true;
    }

    // swap parameters if given in descending order
    if (istart.x > idest.x)
    {
        std::swap(istart, idest);
    }

    // draw the line
    int dx = idest.x - istart.x;
    int dy = idest.y - istart.y;
    for (int x = istart.x; x < idest.x; ++x)
    {
        float t = (x - istart.x)/(float)dx;
        int y = istart.y + dy * t;
        if (transposed) { img.set(y, x, color); }
        else            { img.set(x, y, color); }
    }
}

void render_triangle (
        TGAImage& img,
        const TGAColor color,
        const vec3 v1,
        const vec3 v2,
        const vec3 v3,
        const vec3 scale,
        const vec3 origin)
{
    render_line(img, color, v1, v2, scale, origin);
    render_line(img, color, v2, v3, scale, origin);
    render_line(img, color, v3, v1, scale, origin);
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
