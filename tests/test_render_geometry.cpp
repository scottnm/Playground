#include "../render_geometry.hpp"
#include "gtest/gtest.h"

TEST(BaryTests, BaryTest1)
{
    auto expected_value = vec2 {0, 0};
    auto actual_value = bary_lerp({0, 0}, {1, 0}, {2, 0}, {1, 0, 0});
    EXPECT_EQ(expected_value, actual_value);
}
