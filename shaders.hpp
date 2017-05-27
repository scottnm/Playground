#ifndef __SIMPLE_TEXTURE_SHADER_H__
#define __SIMPLE_TEXTURE_SHADER_H__

#include "ishader.hpp"
#include "tgaimage.hpp"
#include <string>
using std::string;

class simple_texture_shader : public ishader
{
public:
    simple_texture_shader(const string& str);

    virtual vec3 vertex(
            const mat4& viewmat,
            vec3 v) const;

    virtual frag_color fragment(
            vec3 bary,
            mat3 verts,
            mat3x2 tex_coords,
            mat3 vert_norms) const;

private:
    TGAImage tex;
};

class normal_shader : public ishader
{
public:
    virtual vec3 vertex(
            const mat4& viewmat,
            vec3 v) const;

    virtual frag_color fragment(
            vec3 bary,
            mat3 verts,
            mat3x2 tex_coords,
            mat3 vert_norms) const;
};

#endif // __SIMPLE_TEXTURE_SHADER_H__
