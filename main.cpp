#include "render_geometry.hpp"
#include "model.hpp"
#include "tgaimage.hpp"
#include <cmath>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);


int main(int argc, char** argv)
{
    // TGAImage image(1000, 1000, TGAImage::RGB);
    auto model_ptr = model::load_model("obj/test.obj");
    model::test_print_model(*model_ptr); 
    // image.flip_vertically(); // move origin to the bottom-left corner
    // image.write_tga_file("output.tga");
    return 0;
}
