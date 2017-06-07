#ifndef __SHADERS_H__
#define __SHADERS_H__

#include "ishader.hpp"
#include "tgaimage.hpp"
#include <string>
using std::string;

class normal_shader : public ishader
{
public:
    normal_shader(
            const string& normalmap_name);

    virtual vec3 vertex(
            const mat4& viewmat,
            const vec3& v) const;

    virtual frag_color fragment(
            const vec3& bary,
            const mat3& verts,
            const mat3x2& tex_coords,
            const mat3& vert_norms) const;

private:
    TGAImage normalmap;
};

class simple_texture_shader : public ishader
{
public:
    simple_texture_shader(
            const string& texture_name);

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

class bumped_texture_shader : public ishader
{
public:
    bumped_texture_shader(
            const string& texture_name,
            const string& normalmap_name);

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
    TGAImage normalmap;
};

class phong_shader : public ishader
{
public:
    phong_shader(
            const string& texture_name,
            const string& normalmap_name,
            const string& specular_name);

    virtual vec3 vertex(
            const mat4& viewmat,
            const vec3& v) const;

    virtual frag_color fragment(
            const vec3& bary,
            const mat3& verts,
            const mat3x2& tex_coords,
            const mat3& vert_norms) const;
private:
    TGAImage diffuse;
    TGAImage normalmap;
    TGAImage specular;
};

class phong_tangent_space_shader : public ishader
{
public:
    phong_tangent_space_shader(
            const string& texture_name,
            const string& normalmap_name,
            const string& specular_name);

    virtual vec3 vertex(
            const mat4& viewmat,
            const vec3& v) const;

    virtual frag_color fragment(
            const vec3& bary,
            const mat3& verts,
            const mat3x2& tex_coords,
            const mat3& vert_norms) const;
private:
    TGAImage diffuse;
    TGAImage normalmap;
    TGAImage specular;
};

#endif //__SHADERS_H__
