#ifndef SASPPU_SASPPU_H_
#define SASPPU_SASPPU_H_

#include "stdint.h"
#include "stdbool.h"

typedef struct
{
    int16_t x;
    int16_t y;
    uint8_t width;
    uint8_t height;
    uint8_t graphics_x;
    uint8_t graphics_y;
    uint8_t windows;
    uint8_t flags;
} Sprite;

typedef struct
{
    int16_t x;
    int16_t y;
    uint8_t windows;
    uint8_t flags;
} Background;

#define BG_WIDTH_POWER (8)
#define BG_HEIGHT_POWER (8)
#define BG_WIDTH (1 << BG_WIDTH_POWER)
#define BG_HEIGHT (1 << BG_HEIGHT_POWER)

#define SPRITE_COUNT (256)
#define SPRITE_CACHE (16)

#define SPR_WIDTH_POWER (8)
#define SPR_HEIGHT_POWER (8)
#define SPR_WIDTH (1 << SPR_WIDTH_POWER)
#define SPR_HEIGHT (1 << SPR_HEIGHT_POWER)

#define MAP_WIDTH_POWER (6)
#define MAP_HEIGHT_POWER (6)
#define MAP_WIDTH (1 << MAP_WIDTH_POWER)
#define MAP_HEIGHT (1 << MAP_HEIGHT_POWER)

#define SPR_ENABLED (1 << 0)
#define SPR_PRIORITY (1 << 1)
#define SPR_FLIP_X (1 << 2)
#define SPR_FLIP_Y (1 << 3)
#define SPR_C_MATH (1 << 4)
#define SPR_DOUBLE (1 << 5)

#define BG_C_MATH (1 << 0)

#define CMATH_HALF_MAIN_SCREEN (1 << 0)
#define CMATH_DOUBLE_MAIN_SCREEN (1 << 1)
#define CMATH_HALF_SUB_SCREEN (1 << 2)
#define CMATH_DOUBLE_SUB_SCREEN (1 << 3)
#define CMATH_ADD_SUB_SCREEN (1 << 4)
#define CMATH_SUB_SUB_SCREEN (1 << 5)
#define CMATH_FADE_ENABLE (1 << 6)
#define CMATH_CMATH_ENABLE (1 << 7)

#define MAIN_SPR0_ENABLE (1 << 0)
#define MAIN_SPR1_ENABLE (1 << 1)
#define MAIN_BG0_ENABLE (1 << 2)
#define MAIN_BG1_ENABLE (1 << 3)
#define MAIN_CMATH_ENABLE (1 << 4)
#define MAIN_BGCOL_WINDOW_ENABLE (1 << 5)

#define WINDOW_A (0b0001)
#define WINDOW_B (0b0010)
#define WINDOW_AB (0b0100)
#define WINDOW_X (0b1000)
#define WINDOW_ALL (0b1111)

extern uint16_t SASPPU_main_state_mainscreen_colour;
extern uint16_t SASPPU_main_state_subscreen_colour;
extern uint8_t SASPPU_main_state_window_1_left;
extern uint8_t SASPPU_main_state_window_1_right;
extern uint8_t SASPPU_main_state_window_2_left;
extern uint8_t SASPPU_main_state_window_2_right;
extern uint8_t SASPPU_main_state_bgcol_windows;
extern uint8_t SASPPU_main_state_flags;
extern Background SASPPU_bg0_state;
extern Background SASPPU_bg1_state;
extern uint8_t SASPPU_cmath_state_screen_fade;
extern uint8_t SASPPU_cmath_state_flags;
extern bool SASPPU_hdma_enable;

extern Sprite SASPPU_oam[SPRITE_COUNT];
extern uint16_t SASPPU_bg0[MAP_WIDTH * MAP_HEIGHT];
extern uint16_t SASPPU_bg1[MAP_WIDTH * MAP_HEIGHT];

extern uint16_t SASPPU_background[BG_WIDTH * BG_HEIGHT];
extern uint16_t SASPPU_sprites[SPR_WIDTH * SPR_HEIGHT];

// extern uint16_t SASPPU_frame_buffer[240 * 240];

extern Sprite *SASPPU_sprite_cache[2][SPRITE_CACHE];

typedef enum
{
    HDMA_NOOP = 0,
    HDMA_WRITE_MAIN_STATE,
    HDMA_WRITE_BG0_STATE,
    HDMA_WRITE_BG1_STATE,
    HDMA_WRITE_CMATH_STATE,
    HDMA_WRITE_OAM,
    HDMA_WRITE_HDMA_DISABLE,
} HDMACommand;

typedef struct
{
    uint16_t mainscreen_colour;
    uint16_t subscreen_colour;
    uint8_t window_1_left;
    uint8_t window_1_right;
    uint8_t window_2_left;
    uint8_t window_2_right;
    uint8_t bgcol_windows;
    uint8_t flags;
} MainState;

typedef struct
{
    uint8_t screen_fade;
    uint8_t flags;
} CMathState;

typedef union
{
    MainState main;
    Background background;
    CMathState cmath;
    Sprite oam;
} HDMAData;

typedef struct
{
    HDMACommand command;
    uint8_t oam_index;
    HDMAData data;
} HDMAEntry;

#define SASPPU_HDMA_TABLE_COUNT (8)

extern HDMAEntry SASPPU_hdma_tables[SASPPU_HDMA_TABLE_COUNT][240];

void SASPPU_render(uint8_t *fb);

#endif // SASPPU_SASPPU_H_
