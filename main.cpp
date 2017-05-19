#include "render_geometry.hpp"
#include "model.hpp"
#include "tgaimage.hpp"
#include "viewmatrix.hpp"
#include "z_buffer.hpp"
#include <cmath>
#include <chrono>
#include <glm/glm.hpp>

using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::duration_cast;

const static std::string base_dir = "obj/african_head/";
const static std::string tex_file = base_dir + "diffuse.tga";
const static std::string obj_file = base_dir + "model.obj";

const static auto window_width = 1000;
const static auto window_height = 1000;

int main(int argc, char** argv)
{
    auto cam_eye = glm::vec3(1.25, 1, 1.25);
    auto cam_center = glm::vec3(-0.2, 0, 0.1);
    auto cam_up = glm::vec3(0, 1, 0);
    auto cam_distance = glm::length(cam_eye - cam_center);

    // get the view matrix
    auto lookat = lookat_xform(cam_eye, cam_center, cam_up);
    auto screenspace = screenspace_xform(window_width, window_height,
                                         window_width, window_height);
    auto perspective_projection = perspective_proj_xform(cam_distance);
    auto viewmat = screenspace * perspective_projection * lookat;

    // prepare the image and texture file
    TGAImage image(window_width, window_height, TGAImage::RGB);
    TGAImage tex;
    tex.read_tga_file(tex_file.c_str());
    tex.flip_vertically();

    // prepare the zbuffer
    z_buffer zbuf(window_width, window_height);

    // load the model
    auto model_ptr = load_model(obj_file.c_str());

    // render the model
    auto starttime = high_resolution_clock::now();
    render_model(image, tex, zbuf, *model_ptr, viewmat);
    auto endtime = high_resolution_clock::now();

    // write the rendered model to a file
    image.flip_vertically();
    image.write_tga_file("output.tga");

    auto render_time = duration_cast<duration<double>>(endtime - starttime);
    printf("Render took: %f seconds\n", render_time.count());

    return 0;
}
