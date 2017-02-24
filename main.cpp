#include "render_geometry.hpp"
#include "model.hpp"
#include "tgaimage.hpp"
#include <cmath>
#include <chrono>

int main(int argc, char** argv)
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::duration_cast;

    const int window_width = 1000;
    const int window_height = 1000;
    TGAImage image(window_width, window_height, TGAImage::RGB);

    auto model_ptr = load_model("obj/african_head.obj");
    auto starttime = high_resolution_clock::now();
    render_model(image, *model_ptr,
            glm::vec3 {window_width / 2, window_height / 2, 1},
            glm::vec3 {window_width / 2, window_height / 2, 0});
    auto endtime = high_resolution_clock::now();
    auto render_time = duration_cast<duration<double>>(endtime - starttime);
    printf("Render took: %f seconds\n", render_time.count());
    image.flip_vertically(); // move origin to the bottom-left corner
    image.write_tga_file("output.tga");
    return 0;
}
