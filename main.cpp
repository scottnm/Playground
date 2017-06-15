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
#include <limits>
#include <memory>
#include <cstdio>

#include <glm/mat3x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::duration_cast;

const static std::string base_dir = "obj/single_tri/";
const static std::string obj_file = base_dir + "model.obj";
const static std::string tex_file = base_dir + "diffuse.tga";
const static std::string normalmap_file = base_dir + "normalmap.tga";
const static std::string tangent_normalmap_file = base_dir + "tangent_normalmap.tga";
const static std::string specular_file = base_dir + "specular.tga";

const static auto window_width = 50;
const static auto window_height = 50;

int main(int argc, char** argv)
{
    // get the view matrix
    auto screenspace = screenspace_xform(window_width, window_height,
            window_width, window_height);

    auto shadowpass_viewmat = screenspace
        * perspective_proj_xform(std::numeric_limits<float>::max())
        * lookat_xform(to_light() * camera_distance(), vec3(0, 0, 0), vec3(0, 1, 0));

    // prepare the image to render to
    TGAImage shadow_buf(window_width, window_height, TGAImage::RGB);

    // prepare the zbuffer
    z_buffer shadow_zbuf(window_width, window_height);

    // shadow shader
    shadow_shader shadowpass;

    // load the model
    auto model_ptr = load_model(obj_file.c_str());

    // render the shadow pass
    printf("SHADOW PASS\n");
    render_model(shadow_buf, shadow_zbuf, *model_ptr, shadowpass_viewmat,
            shadowpass, to_light());
    printf("\n");

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
    //          simple_texture_shader shader(tex_file);
    //          normal_shader shader(normalmap_file);
    //          bumped_texture_shader shader(tex_file, normalmap_file);
    //          phong_shader shader(tex_file, normalmap_file, specular_file);
    //          phong_tangent_space_shader shader(tex_file, tangent_normalmap_file, specular_file);
    //          shadow_shader shader;
    shader_with_shadows shader(std::make_unique<shadow_shader>(),
            shadow_zbuf,
            //inverse(viewmat));
            shadowpass_viewmat * inverse(viewmat));


    // render the model
    printf("PRIMARY PASS\n");

    { // test
        // TODO
        //
        //  NUKE Mat includes and cstdio includes
        //  NUKE vec
        auto vobj = glm::vec4(0, 0, 0.5, 1);
        auto vproj = viewmat * vobj;
        vproj /= vproj.w;
        auto vobj2 = inverse(viewmat) * vproj;
        vobj2 /= vobj2.w;


        // A = -1, -1, 0, 1
        // VM = viewmat
        //
        // B = VM * A
        // B = retroproj(B)
        // C = VM_i * B = VM_i * VM * A = A
        // C = retroproj(C)
        //
        // V = B                          
        // SHADOW_XFORM = VM_i  
        // SV = SHADOW_XFORM * V = VM_i * B
        // retroproj(SV)
        

        auto vsc = shadowpass_viewmat * vobj;
        vsc /= vsc.w;

        printf("vobj: <%f, %f, %f, %f>\n"
                "vproj: <%f, %f, %f, %f>\n"
                //"vobj2: <%f, %f, %f, %f>\n"
                "vshadcheck: <%f, %f, %f, %f\n",
                vobj.x, vobj.y, vobj.z, vobj.w,
                vproj.x, vproj.y, vproj.z, vproj.w,
                //vobj2.x, vobj2.y, vobj2.z, vobj2.w,
                vsc.x, vsc.y, vsc.z, vsc.w);



        glm::mat3x2 dummyvts;
        glm::mat3 dummyvns;
        shader.fragment(vec3(1, 0, 0), mat3(
                    vec3(19.307098, 19.200771, 0.429300),
                    vec3(0,0,0),
                    vec3(0,0,0)),
             dummyvts, dummyvns);
    }

    auto starttime = high_resolution_clock::now();
    render_model(image, zbuf, *model_ptr, viewmat, shader, to_camera());
    auto endtime = high_resolution_clock::now();

    // write the rendered model to a file
    image.flip_vertically();
    image.write_tga_file("output.tga");

    auto render_time = duration_cast<duration<double>>(endtime - starttime);
    printf("Render took: %f seconds\n", render_time.count());

    /*
    // get the view matrix
    auto screenspace = screenspace_xform(window_width, window_height,
            window_width, window_height);

    auto shadowpass_viewmat = screenspace
        * perspective_proj_xform(camera_distance())
        * lookat_xform(to_light() * camera_distance(), vec3(0, 0, 0), vec3(0, 1, 0));

    // prepare the image to render to
    TGAImage shadow_buf(window_width, window_height, TGAImage::RGB);

    // prepare the zbuffer
    z_buffer shadow_zbuf(window_width, window_height);

    // shadow shader
    shadow_shader shadowpass;

    // load the model
    auto model_ptr = load_model(obj_file.c_str());

    // render the model
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
    //          simple_texture_shader shader(tex_file);
    //          normal_shader shader(normalmap_file);
    //          bumped_texture_shader shader(tex_file, normalmap_file);
    //          phong_shader shader(tex_file, normalmap_file, specular_file);
    //          phong_tangent_space_shader shader(tex_file, tangent_normalmap_file, specular_file);
    //          shadow_shader shader;
    shader_with_shadows shader(std::make_unique<shadow_shader>(),
            shadow_zbuf, shadowpass_viewmat * inverse(viewmat));


    // render the model
    auto starttime = high_resolution_clock::now();
    render_model(image, zbuf, *model_ptr, viewmat, shader, to_camera());
    auto endtime = high_resolution_clock::now();

    // write the rendered model to a file
    image.flip_vertically();
    image.write_tga_file("output.tga");

    auto render_time = duration_cast<duration<double>>(endtime - starttime);
    printf("Render took: %f seconds\n", render_time.count());

    */
    return 0;
}
