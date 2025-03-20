#include "sasppu_help.h"
#include "sasppu.h"
#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"
#include "string.h"

void SASPPU_copy_sprite(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t *data)
{
    for (uint8_t yi = y; yi < (y + height); yi++)
    {
        for (uint8_t xi = x; xi < (x + width); xi++)
        {
            SASPPU_sprites[(yi * SPR_WIDTH) + xi] = *(data++);
        }
    }
}

void SASPPU_copy_sprite_transparent(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t *data)
{
    for (uint8_t yi = y; yi < (y + height); yi++)
    {
        for (uint8_t xi = x; xi < (x + width); xi++)
        {
            uint16_t pixel = *(data++);
            if (pixel)
            {
                SASPPU_sprites[(yi * SPR_WIDTH) + xi] = pixel;
            }
        }
    }
}

void SASPPU_fill_sprite(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t colour)
{
    for (uint8_t yi = y; yi < (y + height); yi++)
    {
        for (uint8_t xi = x; xi < (x + width); xi++)
        {
            SASPPU_sprites[(yi * SPR_WIDTH) + xi] = colour;
        }
    }
}

void SASPPU_copy_background(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t *data)
{
    for (uint8_t yi = y; yi < (y + height); yi++)
    {
        for (uint8_t xi = x; xi < (x + width); xi++)
        {
            SASPPU_background[(yi * SPR_WIDTH) + xi] = *(data++);
        }
    }
}

void SASPPU_copy_background_transparent(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t *data)
{
    for (uint8_t yi = y; yi < (y + height); yi++)
    {
        for (uint8_t xi = x; xi < (x + width); xi++)
        {
            uint16_t pixel = *(data++);
            if (pixel)
            {
                SASPPU_background[(yi * SPR_WIDTH) + xi] = pixel;
            }
        }
    }
}

void SASPPU_fill_background(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t colour)
{
    for (uint8_t yi = y; yi < (y + height); yi++)
    {
        for (uint8_t xi = x; xi < (x + width); xi++)
        {
            SASPPU_background[(yi * SPR_WIDTH) + xi] = colour;
        }
    }
}

void SASPPU_gfx_reset()
{
    SASPPU_main_state_mainscreen_colour = 0x0000;
    SASPPU_main_state_subscreen_colour = 0x0000;
    SASPPU_main_state_window_1_left = 0;
    SASPPU_main_state_window_1_right = 255;
    SASPPU_main_state_window_2_left = 0;
    SASPPU_main_state_window_2_right = 255;
    SASPPU_main_state_bgcol_windows = 0x0F;
    SASPPU_main_state_flags = 0;

    SASPPU_bg0_state.x = 0;
    SASPPU_bg0_state.y = 0;
    SASPPU_bg0_state.windows = 0x0F;
    SASPPU_bg0_state.flags = 0;
    SASPPU_bg1_state.x = 0;
    SASPPU_bg1_state.y = 0;
    SASPPU_bg1_state.windows = 0x0F;
    SASPPU_bg1_state.flags = 0;
    SASPPU_cmath_state_screen_fade = 255;
    SASPPU_cmath_state_flags = 0;
    SASPPU_hdma_enable = false;

    for (int i = 0; i < SPRITE_COUNT; i++)
    {
        SASPPU_oam[i].x = 0;
        SASPPU_oam[i].y = 0;
        SASPPU_oam[i].width = 16;
        SASPPU_oam[i].height = 16;
        SASPPU_oam[i].graphics_x = 0;
        SASPPU_oam[i].graphics_y = 0;
        SASPPU_oam[i].windows = 0x0F;
        SASPPU_oam[i].flags = 0;
    }

    memset(SASPPU_bg0, 0, sizeof(SASPPU_bg0));
    memset(SASPPU_bg1, 0, sizeof(SASPPU_bg1));
    memset(SASPPU_background, 0, sizeof(SASPPU_background));
    memset(SASPPU_sprites, 0, sizeof(SASPPU_sprites));
    memset(SASPPU_hdma_tables, 0, sizeof(SASPPU_hdma_tables));
    // memset(SASPPU_frame_buffer, 0, sizeof(SASPPU_frame_buffer));
}