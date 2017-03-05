#include "gtest/gtest.h"
#include "../model.hpp"

static const std::string objdir = "test_objs/";

TEST(ModelTests, Face_Spec_Only_Verts)
{
    auto model_ptr = load_model(objdir + "only_v.obj");
    auto& verts = model_ptr->verts;
    EXPECT_EQ(verts.size(), 3);
    EXPECT_EQ(model_ptr->vert_norms.size(), 0);
    EXPECT_EQ(model_ptr->text_verts.size(), 0);

    auto& faces = model_ptr->faces;
    auto test_face = faces[faces.size() - 1];
    auto v0 = verts[test_face.vi[0]];
    auto ev0 = vec3{0, 0, 0};
    auto v1 = verts[test_face.vi[1]];
    auto ev1 = vec3{1, 1, 1};
    auto v2 = verts[test_face.vi[2]];
    auto ev2 = vec3{2, 2, 2};

    EXPECT_EQ(v0, ev0);
    EXPECT_EQ(v1, ev1);
    EXPECT_EQ(v2, ev2);
}

TEST(ModelTests, Face_Spec_Verts_And_Textures)
{
    auto model_ptr = load_model(objdir + "verts_and_vts.obj");
    auto& verts = model_ptr->verts;
    auto& text_verts = model_ptr->text_verts;
    EXPECT_EQ(verts.size(), 6);
    EXPECT_EQ(model_ptr->vert_norms.size(), 0);
    EXPECT_EQ(text_verts.size(), 4);

    auto& faces = model_ptr->faces;
    auto test_face = faces[faces.size() - 1];

    auto v0 = verts[test_face.vi[0]];
    auto ev0 = vec3{1, 1, 1};
    auto v1 = verts[test_face.vi[1]];
    auto ev1 = vec3{2, 2, 2};
    auto v2 = verts[test_face.vi[2]];
    auto ev2 = vec3{3, 3, 3};

    EXPECT_EQ(v0, ev0);
    EXPECT_EQ(v1, ev1);
    EXPECT_EQ(v2, ev2);

    auto vt0 = text_verts[test_face.vti[0]];
    auto evt0 = vec2{0, 1};
    auto vt1 = text_verts[test_face.vti[1]];
    auto evt1 = vec2{1, 0};
    auto vt2 = text_verts[test_face.vti[2]];
    auto evt2 = vec2{1, 1};

    EXPECT_EQ(vt0, evt0);
    EXPECT_EQ(vt1, evt1);
    EXPECT_EQ(vt2, evt2);
}

TEST(ModelTests, Face_Spec_Verts_And_Normals)
{
    auto model_ptr = load_model(objdir + "verts_and_vns.obj");
    auto& verts = model_ptr->verts;
    auto& vert_norms = model_ptr->vert_norms;
    EXPECT_EQ(verts.size(), 6);
    EXPECT_EQ(vert_norms.size(), 4);
    EXPECT_EQ(model_ptr->text_verts.size(), 0);

    auto& faces = model_ptr->faces;
    auto test_face = faces[faces.size() - 1];

    auto v0 = verts[test_face.vi[0]];
    auto ev0 = vec3{0, 0, 0};
    auto v1 = verts[test_face.vi[1]];
    auto v2 = verts[test_face.vi[2]];

    EXPECT_EQ(v0, ev0);
    EXPECT_EQ(v1, ev0);
    EXPECT_EQ(v2, ev0);

    auto vn0 = vert_norms[test_face.vni[0]];
    auto evn0 = vec3{0, 1, 0};
    auto vn1 = vert_norms[test_face.vni[1]];
    auto evn1 = vec3{1, 0, 0};
    auto vn2 = vert_norms[test_face.vni[2]];
    auto evn2 = vec3{1, 1, 0};

    EXPECT_EQ(vn0, evn0);
    EXPECT_EQ(vn1, evn1);
    EXPECT_EQ(vn2, evn2);
}

TEST(ModelTests, Face_Spec_Verts_Tex_And_Norms)
{
    auto model_ptr = load_model(objdir + "verts_vns_and_vts.obj");
    auto& verts = model_ptr->verts;
    auto& vert_norms = model_ptr->vert_norms;
    auto& text_verts = model_ptr->text_verts;
    EXPECT_EQ(verts.size(), 6);
    EXPECT_EQ(vert_norms.size(), 4);
    EXPECT_EQ(text_verts.size(), 3);

    auto& faces = model_ptr->faces;
    auto test_face = faces[faces.size() - 1];

    auto v0 = verts[test_face.vi[0]];
    auto ev0 = vec3{0, 0, 0};
    auto v1 = verts[test_face.vi[1]];
    auto ev1 = vec3{0, 0, 0};
    auto v2 = verts[test_face.vi[2]];
    auto ev2 = vec3{0, 0, 0};

    EXPECT_EQ(v0, ev0);
    EXPECT_EQ(v1, ev1);
    EXPECT_EQ(v2, ev2);

    auto vt0 = text_verts[test_face.vti[0]];
    auto evt0 = vec2{0, 0};
    auto vt1 = text_verts[test_face.vti[1]];
    auto evt1 = vec2{0, 1};
    auto vt2 = text_verts[test_face.vti[2]];
    auto evt2 = vec2{1, 0};

    EXPECT_EQ(vt0, evt0);
    EXPECT_EQ(vt1, evt1);
    EXPECT_EQ(vt2, evt2);

    auto vn0 = vert_norms[test_face.vni[0]];
    auto evn0 = vec3{0, 1, 0};
    auto vn1 = vert_norms[test_face.vni[1]];
    auto evn1 = vec3{1, 0, 0};
    auto vn2 = vert_norms[test_face.vni[2]];
    auto evn2 = vec3{1, 1, 0};

    EXPECT_EQ(vn0, evn0);
    EXPECT_EQ(vn1, evn1);
    EXPECT_EQ(vn2, evn2);
}
