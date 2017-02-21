#include "line.hpp"
#include "tgaimage.hpp"
#include "vec2i.hpp"
#include <cmath>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);


int main(int argc, char** argv)
{
    TGAImage image(100, 100, TGAImage::RGB);
    for (int i = 0; i < 1000000; ++i)
    {
        line(image, white, {13, 20}, {80, 40}); 
        line(image, red, {20, 13}, {40, 80}); 
        line(image, red, {80, 40}, {13, 20});
    }
    image.flip_vertically(); // move origin to the bottom-left corner
    image.write_tga_file("output.tga");
    return 0;
}
