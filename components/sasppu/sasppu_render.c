#include "sasppu.h"
#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#include "esp_attr.h"
#include "sdkconfig.h"

EXT_RAM_BSS_ATTR uint16_t SASPPU_background[BG_WIDTH * BG_HEIGHT];
EXT_RAM_BSS_ATTR uint16_t SASPPU_sprites[SPR_WIDTH * SPR_HEIGHT];
// EXT_RAM_BSS_ATTR uint8_t SASPPU_frame_buffer[240 * 240 * 2];

EXT_RAM_BSS_ATTR HDMAEntry SASPPU_hdma_tables[8][240];

uint8_t SASPPU_hdma_enable;

// Defined in sasppu_fast.S
void SASPPU_render_scanline(uint8_t y, uint8_t *fb);

static inline void SASPPU_per_scanline(uint8_t y, uint8_t *fb)
{
    for (size_t table = 0; table < SASPPU_HDMA_TABLE_COUNT; table++)
    {
        if (((SASPPU_hdma_enable >> table) & 1) == 0)
        {
            continue;
        }

        HDMAEntry *entry = &SASPPU_hdma_tables[table][y];

        if (entry->command == HDMA_DISABLE)
        {
            SASPPU_hdma_enable &= ~(1 << table);
            break;
        }

        switch (entry->command)
        {
        case HDMA_NOOP:
        default:
            break;
        case HDMA_WRITE_MAIN_STATE:
        {
            MainState *state = &entry->data.main;
            SASPPU_main_state_mainscreen_colour = state->mainscreen_colour;
            SASPPU_main_state_subscreen_colour = state->subscreen_colour;
            SASPPU_main_state_window_1_left = state->window_1_left;
            SASPPU_main_state_window_1_right = state->window_1_right;
            SASPPU_main_state_window_2_left = state->window_2_left;
            SASPPU_main_state_window_2_right = state->window_2_right;
            SASPPU_main_state_bgcol_windows = state->bgcol_windows;
            SASPPU_main_state_flags = state->flags;
        }
        break;
        case HDMA_WRITE_BG0_STATE:
        {
            SASPPU_bg0_state = entry->data.background;
        }
        break;
        case HDMA_WRITE_BG1_STATE:
        {
            SASPPU_bg1_state = entry->data.background;
        }
        break;
        case HDMA_WRITE_CMATH_STATE:
        {
            CMathState *state = &entry->data.cmath;
            SASPPU_cmath_state_screen_fade = state->screen_fade;
            SASPPU_cmath_state_flags = state->flags;
        }
        break;
        case HDMA_WRITE_OAM:
        {
            SASPPU_oam[entry->oam_index] = entry->data.oam;
        }
        break;
        }
    }

    uint32_t sprites_indcies[2] = {0, 0};
    for (size_t i = 0; i < SPRITE_COUNT; i++)
    {
        Sprite *spr = &SASPPU_oam[i];
        uint8_t flags = spr->flags;
        uint8_t windows = spr->windows;
        int16_t iy = (int16_t)y;

        int16_t spr_height = (int16_t)(spr->height);
        int16_t spr_width = (int16_t)(spr->width);

        bool main_screen_enable = (windows & 0x0F) > 0;
        bool sub_screen_enable = (windows & 0xF0) > 0;

        bool enabled = (flags & SPR_ENABLED) > 0;
        bool priority = (flags & SPR_PRIORITY) > 0;
        // bool flip_x = (flags & SPR_FLIP_X) > 0;
        // bool flip_y = (flags & SPR_FLIP_Y) > 0;
        // bool cmath_enabled = (flags & SPR_C_MATH) > 0;
        bool double_enabled = (flags & SPR_DOUBLE) > 0;

        // If not enabled, skip
        if (!enabled)
        {
            continue;
        }

        // If we've hit the limit, skip
        if ((priority && (sprites_indcies[1] == SPRITE_CACHE)) || (!priority && (sprites_indcies[0] == SPRITE_CACHE)))
        {
            continue;
        }

        bool window_enabled = (main_screen_enable) || (sub_screen_enable);
        bool top_border = spr->y <= iy;
        bool bottom_border = double_enabled ? (spr->y > (iy - (spr_height << 1))) : (spr->y > (iy - (spr_height)));
        bool right_border = spr->x < 240;
        bool left_border = double_enabled ? (spr->x > -(spr_width << 1)) : (spr->x > -(spr_width));

        if (window_enabled && top_border && bottom_border && right_border && left_border)
        {
            if (priority)
            {
                SASPPU_sprite_cache[1][sprites_indcies[1]] = spr;
                sprites_indcies[1] += 1;
            }
            else
            {
                SASPPU_sprite_cache[0][sprites_indcies[0]] = spr;
                sprites_indcies[0] += 1;
            }

            if ((sprites_indcies[1] == SPRITE_CACHE) && (sprites_indcies[0] == SPRITE_CACHE))
            {
                break;
            }
        }
    }
    for (; sprites_indcies[0] < SPRITE_CACHE; sprites_indcies[0]++)
    {
        SASPPU_sprite_cache[0][sprites_indcies[0]] = NULL;
    }
    for (; sprites_indcies[1] < SPRITE_CACHE; sprites_indcies[1]++)
    {
        SASPPU_sprite_cache[1][sprites_indcies[1]] = NULL;
    }

    return SASPPU_render_scanline(y, fb);
}

void SASPPU_render(uint8_t *fb, uint8_t section)
{
    // Screen is rendered top to bottom for sanity's sake
    for (size_t y = 60 * section; y < 60 * (section + 1); y++)
    {
        SASPPU_per_scanline(y, fb);
    }
}