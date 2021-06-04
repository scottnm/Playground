#include "pch.h"

// FIXME: stubbing
int
main(
    void)
{
#define RGB16(r,g,b)  ((r)+(g<<5)+(b<<10))

    char x,y;
    unsigned short* Screen = (unsigned short*)0x6000000;
    *(unsigned long*)0x4000000 = 0x403; // mode3, bg2 on

    // clear screen, and draw a blue back ground
    for(x = 0; x<240;x++)   //loop through all x
    {
        for(y = 0; y<160; y++)  //loop through all y
        {
            Screen[x+y*240] = RGB16(0,0,31);
        }
    }

    // draw a white HI on the background
    for(x = 20; x<=60; x+=15)
        for(y = 30; y<50; y++)
            Screen[x+y*240] = RGB16(31,31,31);
    for (x = 20; x < 35; x++)
        Screen[x+40*240] = RGB16(31,31,31);

    while(1){}    //loop forever
    return 0;
}

/*
void
main(
    void)
{
    // TODO(scottnm): what's this all about?
    {
        unsigned long* mode_memory = (unsigned long*)0x4000000;
        *mode_memory = 0x403; // set mode3 with bg2 on
    }

    u16_span_t screen_memory = get_gba_screen_buffer();

    // clear screen, and draw a blue background
    for (uint32_t pixel = 0; pixel < screen_memory.count; pixel += 1)
    {
        screen_memory.data[pixel] = COLOR_BLUE;
    }

    // draw a white HI on the background
    {
        // Draw the 3 vertical lines for the H and I
        for (uint32_t vert_line = 0; vert_line < 3; vert_line += 1)
        {
            uint32_t col = vert_line * 15 + 20;
            for (uint32_t row = 30; row < 50; row += 1)
            {
                uint32_t pixel_index = get_gba_pixel_index(row, col);
                screen_memory.data[pixel_index] = COLOR_WHITE;
            }
        }

        // draw the horizontal bar for the H
        uint32_t h_bar_offset = get_gba_pixel_index(40, 20);
        for (uint32_t i = 0; i < 15; i += 1)
        {
            screen_memory.data[h_bar_offset + i] = COLOR_WHITE;
        }
    }

    while (true)
    {
        // Loop forever
    }
}
*/
