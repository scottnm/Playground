#include "../render_geometry.hpp"
#include "gtest/gtest.h"

static const auto camera_dist = 5.0f;

TEST(BaryTests, BaryTest1)
{
    auto expected_value = vec2 {0, 0};
    auto actual_value = bary_lerp({0, 0}, {1, 0}, {2, 0}, {1, 0, 0});
    EXPECT_EQ(expected_value, actual_value);
}

TEST(ProjectionTests, ProjectionTest_Identity)
{
    auto world_space_coord = vec3 {1, 2, 0};
    auto camera_dist = 5.0f;
    EXPECT_EQ(world_space_coord,
              project_coord(camera_dist, world_space_coord)); 
}

TEST(ProjectionTests, ProjectionTest_OnNearClip)
{
    auto world_space_coord = vec3 {1, 2, 1};
    auto camera_dist = 5.0f;
    EXPECT_EQ(world_space_coord * 5.0f/4.0f,
              project_coord(camera_dist, world_space_coord)); 
}

TEST(ProjectionTests, ProjectionTest_1)
{
    auto world_space_coord = vec3 {1, 2, -3};
    auto camera_dist = 5.0f;
    EXPECT_EQ(world_space_coord * 5.0f/8.0f,
              project_coord(camera_dist, world_space_coord)); 
}
