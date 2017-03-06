#ifndef __Z_BUF_H__
#define __Z_BUF_H__

#include <limits>
#include <vector>

typedef struct z_buffer
{
    int w;
    int h;
    std::vector<float> buf;

    z_buffer(int width, int height) : w(width), h(height)
    {
        buf.reserve(w * h);
        float float_min = std::numeric_limits<float>::lowest();
        for (int i = 0; i < w * h; ++i)
        {
            buf.push_back(float_min);
        }
    }

    bool is_oob(int x, int y);

    float get(int x, int y);

    void set(int x, int y, float zval);
}
z_buffer;

#endif // __Z_BUF_H__
