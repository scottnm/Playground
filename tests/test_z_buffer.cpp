#include "../z_buffer.hpp"
#include "gtest/gtest.h"

TEST(ZBufferTests, ZBuffer_Test_Init)
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
                    test_buffer.get(x, y));
        }
    }
}

TEST(ZBufferTests, ZBuffer_Test_Random_Elem)
{
    auto w = 10;
    auto h = 10;
    z_buffer test_buffer(w, h);
    
    test_buffer.set(2, 2, 1.5f);
    EXPECT_EQ(test_buffer.get(2, 2), 1.5f);
    EXPECT_EQ(test_buffer.get(3, 2),
            std::numeric_limits<float>::lowest());
}
