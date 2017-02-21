#include "render_geometry.hpp"
#include "tgaimage.hpp"
#include "vec2i.hpp"
#include <cmath>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);


int main(int argc, char** argv)
{
    TGAImage image(1000, 1000, TGAImage::RGB);
    render_geometry::line(image, white, {130, 200}, {800, 400}); 
    render_geometry::line(image, red,   {200, 130}, {400, 800}); 
    render_geometry::line(image, red,   {800, 400}, {130, 200});
    image.flip_vertically(); // move origin to the bottom-left corner
    image.write_tga_file("output.tga");
    return 0;
}
