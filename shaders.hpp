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
            const vec3& v) const;

    virtual frag_color fragment(
            const vec3& bary,
            const mat3& verts,
            const mat3x2& tex_coords,
            const mat3& vert_norms) const;

private:
    TGAImage tex;
};

class normal_shader : public ishader
{
public:
    virtual vec3 vertex(
            const mat4& viewmat,
            const vec3& v) const;

    virtual frag_color fragment(
            const vec3& bary,
            const mat3& verts,
            const mat3x2& tex_coords,
            const mat3& vert_norms) const;
};

#endif // __SIMPLE_TEXTURE_SHADER_H__
