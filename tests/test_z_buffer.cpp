#include "../z_buffer.hpp"
#include "gtest/gtest.h"

TEST(ZBufferTests, ZBufferTestInit)
{
    auto w = 10;
    auto h = 10;
    z_buffer test_buffer(w, h);
    auto expected_value = std::numeric_limits<float>::lowest(); 
    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            EXPECT_EQ(expected_value,
                    get_z_buffer(test_buffer, x, y));
        }
    }
}
