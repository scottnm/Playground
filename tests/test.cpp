#include "../render_geometry.hpp"
#include "gtest/gtest.h"

vec2 bary_lerp(
        vec2 v0,
        vec2 v1,
        vec2 v2,
        vec3 bary);

TEST(BaryTests, BaryTest1)
{
    auto expected_value = vec2 {0, 0};
    auto actual_value = bary_lerp({0, 0}, {1, 0}, {2, 0}, {1, 0, 0});
    EXPECT_EQ(expected_value, actual_value);
}
