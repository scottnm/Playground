#include "z_buffer.hpp"

bool z_buffer::is_oob(int x, int y)
{
    return x >= w || x < 0 || y >= h || y < 0;
}

float z_buffer::get(int x, int y)
{
    if (is_oob(x, y))
    {
        return std::numeric_limits<float>::max();
    }
    return buf[y * w + x];
}

void z_buffer::set(int x, int y, float zval)
{
    buf[y * w + x] = zval;
}
