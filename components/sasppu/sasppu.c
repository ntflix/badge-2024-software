#include "sasppu.h"
#include "sasppu_internal.h"
#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#include "sasppu_gen.h"

#if SASPPU_ESP
#include "esp_attr.h"
#include "sdkconfig.h"
#else
#define EXT_RAM_BSS_ATTR
#endif

MainState SASPPU_main_state;
Background SASPPU_bg0_state;
Background SASPPU_bg1_state;
CMathState SASPPU_cmath_state;
uint8_t SASPPU_hdma_enable;

Sprite SASPPU_oam[SPRITE_COUNT];
uint16_t SASPPU_bg0[MAP_WIDTH * MAP_HEIGHT];
uint16_t SASPPU_bg1[MAP_WIDTH * MAP_HEIGHT];

EXT_RAM_BSS_ATTR uint16x8_t SASPPU_background[BG_WIDTH * BG_HEIGHT / 8];
EXT_RAM_BSS_ATTR uint16x8_t SASPPU_sprites[SPR_WIDTH * SPR_HEIGHT / 8];
#if SASPPU_FRAMEBUFFER
EXT_RAM_BSS_ATTR uint16x8_t SASPPU_frame_buffer[240 * 240 / 8];
#endif

Sprite *SASPPU_sprite_cache[2][SPRITE_CACHE];

uint16x8_t SASPPU_subscreen_scanline[240 / 8];
mask16x8_t SASPPU_window_cache[(240 / 8) * 2];

EXT_RAM_BSS_ATTR HDMAEntry SASPPU_hdma_tables[SASPPU_HDMA_TABLE_COUNT][240];

const uint16x8_t VECTOR_INCREMENTS = {0, 1, 2, 3, 4, 5, 6, 7};
const uint16x8_t VECTOR_SCANLINE_END = VBROADCAST(SCANLINE_END);
const uint16x8_t VECTOR_INCREMENTS_END = VECTOR_INCREMENTS + VECTOR_SCANLINE_END;

const uint16_t SASPPU_EIGHT = 8;
const uint16_t CMATH_BIT = 0x8000;
const uint16_t CMATH_MASK_LOW = 0b0000000000011111;
const uint16_t CMATH_MASK_GREEN = 0b0000000001100000;
const uint16_t CMATH_MASK_SPLIT = 0b0111110000000000;
const uint16_t CMATH_ONE = 1;
const uint16_t CMATH_TWO_FOUR = 1 << 4;
const uint16_t CMATH_TWO_FIVE = 1 << 5;
const uint16_t CMATH_TWO_EIGHT = 1 << 8;
const uint16_t CMATH_TWO_NINE = 1 << 9;
const uint16_t CMATH_TWO_TEN = 1 << 10;

const HandleSpriteType HANDLE_SPRITE_LOOKUP[16];
const HandleScanlineType HANDLE_SCANLINE_LOOKUP[64];
const HandleCMathType HANDLE_CMATH_LOOKUP[256];
const HandleWindowType HANDLE_WINDOW_LOOKUP[256];

#if USE_GCC_SIMD
const uint16x8_t REVERSE_MASK = {7, 6, 5, 4, 3, 2, 1, 0};
const uint16x8_t INTERLEAVE_MASK_LOW = {0, 8, 1, 9, 2, 10, 3, 11};
const uint16x8_t INTERLEAVE_MASK_HIGH = {4, 12, 5, 13, 6, 14, 7, 15};
const uint16x8_t VECTOR_SHUFFLES[9] = {
    {0, 1, 2, 3, 4, 5, 6, 7},
    {1, 2, 3, 4, 5, 6, 7, 8},
    {2, 3, 4, 5, 6, 7, 8, 9},
    {3, 4, 5, 6, 7, 8, 9, 10},
    {4, 5, 6, 7, 8, 9, 10, 11},
    {5, 6, 7, 8, 9, 10, 11, 12},
    {6, 7, 8, 9, 10, 11, 12, 13},
    {7, 8, 9, 10, 11, 12, 13, 14},
    {8, 9, 10, 11, 12, 13, 14, 15},
};
#endif

static inline void SASPPU_handle_hdma(uint8_t y)
{
    size_t table = 0;
    do
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
            SASPPU_main_state = entry->data.main;
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
            SASPPU_cmath_state = entry->data.cmath;
        }
        break;
        case HDMA_WRITE_OAM:
        {
            SASPPU_oam[entry->oam_index] = entry->data.oam;
        }
        break;
        }
    } while ((++table) < SASPPU_HDMA_TABLE_COUNT);
}

static inline void SASPPU_handle_sprite_cache(uint8_t y)
{
    uint32_t sprites_indcies[2] = {0, 0};
    size_t i = 0;
    do
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
    } while ((++i) < SPRITE_COUNT);
    do
    {
        SASPPU_sprite_cache[0][sprites_indcies[0]] = NULL;
    } while ((++sprites_indcies[0]) < SPRITE_CACHE);
    do
    {
        SASPPU_sprite_cache[1][sprites_indcies[1]] = NULL;
    } while ((++sprites_indcies[1]) < SPRITE_CACHE);
}

void SASPPU_render(uint16x8_t *fb, uint8_t section)
{
    // Screen is rendered top to bottom for sanity's sake
    size_t y = 60 * section;
    do
    {
        if (SASPPU_hdma_enable)
        {
            SASPPU_handle_hdma(y);
        }
        if (SASPPU_main_state.flags & (MAIN_SPR0_ENABLE | MAIN_SPR1_ENABLE))
        {
            SASPPU_handle_sprite_cache(y);
        }
        HANDLE_SCANLINE_LOOKUP[SASPPU_main_state.flags](fb + (y * 240 / 8), y);
    } while ((++y) < (60 * (section + 1)));
}