#include "camera.hpp"
#include "ishader.hpp"
#include "light.hpp"
#include "model.hpp"
#include "render_geometry.hpp"
#include "shaders.hpp"
#include "tgaimage.hpp"
#include "viewmatrix.hpp"
#include "z_buffer.hpp"

#include <cmath>
#include <chrono>
#include <glm/glm.hpp>
#include <memory>

using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::duration_cast;

const static std::string base_dir = "obj/african_head/";
const static std::string obj_file = base_dir + "model.obj";
const static std::string tex_file = base_dir + "diffuse.tga";
const static std::string normalmap_file = base_dir + "normalmap.tga";
const static std::string tangent_normalmap_file = base_dir + "tangent_normalmap.tga";
const static std::string specular_file = base_dir + "specular.tga";

const static auto window_width = 800;
const static auto window_height = 800;

int main(int argc, char** argv)
{
    // get the view matrix
    auto screenspace = screenspace_xform(window_width / 8, window_height / 8,
            window_width * .75, window_height * .75);
    auto lookat = lookat_xform(to_light(), camera_target(), vec3(0, 1, 0));
    auto proj = perspective_proj_xform(std::numeric_limits<float>::max());

    auto shadowpass_viewmat = screenspace * proj * lookat;

    // prepare the image to render to
    TGAImage shadow_buf(window_width, window_height, TGAImage::RGB);

    // prepare the zbuffer
    z_buffer shadow_zbuf(window_width, window_height);

    // shadow shader
    shadow_shader shadowpass;

    // load the model
    auto model_ptr = load_model(obj_file.c_str());

    render_model(shadow_buf, shadow_zbuf, *model_ptr, shadowpass_viewmat,
            shadowpass, to_light());

    // write the rendered model to a file
    shadow_buf.flip_vertically();
    shadow_buf.write_tga_file("depth.tga");

    // get the view matrix
    auto viewmat = screenspace * perspective_proj_xform(camera_distance())
                   * lookat_xform(camera_position(), camera_target(), camera_up());

    // prepare the image to render to
    TGAImage image(window_width, window_height, TGAImage::RGB);

    // prepare the zbuffer
    z_buffer zbuf(window_width, window_height);

    // select your shader 
    //  simple_texture_shader shader(tex_file);
    //  normal_shader shader(normalmap_file);
    //  simple_normal_shader shader;
    //  bumped_texture_shader shader(tex_file, normalmap_file);
    //  phong_shader shader(tex_file, normalmap_file, specular_file);
    //  phong_tangent_space_shader shader(tex_file, tangent_normalmap_file, specular_file);
    //  shadow_shader shader;
    shader_with_shadows shader(
            std::make_unique<phong_tangent_space_shader>(tex_file, tangent_normalmap_file, specular_file),
            shadow_zbuf,
            shadowpass_viewmat * inverse(viewmat));

    auto starttime = high_resolution_clock::now();
    render_model(image, zbuf, *model_ptr, viewmat, shader, to_camera());
    auto endtime = high_resolution_clock::now();

    // write the rendered model to a file
    image.flip_vertically();
    image.write_tga_file("output.tga");

    auto render_time = duration_cast<duration<double>>(endtime - starttime);
    printf("Render took: %f seconds\n", render_time.count());

    return 0;
}
