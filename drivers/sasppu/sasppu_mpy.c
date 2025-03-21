// Include the header file to get access to the MicroPython API
#include "py/runtime.h"
#include "sasppu.h"
#include "sasppu_help.h"

#define DEFINE_TRIVIAL_ACCESSOR(name, mask)                              \
    static mp_obj_t name##_accessor(size_t n_args, const mp_obj_t *args) \
    {                                                                    \
        if (n_args > 0)                                                  \
        {                                                                \
            mp_int_t value = mp_obj_get_int(args[0]) & (mask);           \
            SASPPU_##name = value;                                       \
        }                                                                \
        return mp_obj_new_int(SASPPU_##name);                            \
    }                                                                    \
    static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(name##_obj, 0, 1, name##_accessor);

DEFINE_TRIVIAL_ACCESSOR(main_state_mainscreen_colour, 0xFFFF);
DEFINE_TRIVIAL_ACCESSOR(main_state_subscreen_colour, 0xFFFF);
DEFINE_TRIVIAL_ACCESSOR(main_state_window_1_left, 0xFF);
DEFINE_TRIVIAL_ACCESSOR(main_state_window_1_right, 0xFF);
DEFINE_TRIVIAL_ACCESSOR(main_state_window_2_left, 0xFF);
DEFINE_TRIVIAL_ACCESSOR(main_state_window_2_right, 0xFF);
DEFINE_TRIVIAL_ACCESSOR(main_state_bgcol_windows, 0xFF);
DEFINE_TRIVIAL_ACCESSOR(main_state_flags, 0xFF);
DEFINE_TRIVIAL_ACCESSOR(cmath_state_screen_fade, 0xFF);
DEFINE_TRIVIAL_ACCESSOR(cmath_state_flags, 0xFF);

typedef struct
{
    mp_obj_base_t base;
    mp_int_t x;
    mp_int_t y;
    mp_uint_t windows;
    mp_uint_t flags;
} sasppu_background_t;

static mp_obj_t sasppu_background_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in)
{
    sasppu_background_t *o = mp_obj_malloc(sasppu_background_t, type);

    return MP_OBJ_FROM_PTR(o);
}

static const mp_rom_map_elem_t sasppu_background_locals_dict_table[] = {};
static MP_DEFINE_CONST_DICT(sasppu_background_locals_dict, sasppu_background_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    sasppu_type_background,
    MP_QSTR_Background,
    MP_TYPE_FLAG_NONE,
    make_new, sasppu_background_make_new,
    locals_dict, &sasppu_background_locals_dict);

typedef struct
{
    mp_obj_base_t base;
    mp_int_t x;
    mp_int_t y;
    mp_uint_t width;
    mp_uint_t height;
    mp_uint_t graphics_x;
    mp_uint_t graphics_y;
    mp_uint_t windows;
    mp_uint_t flags;
} sasppu_sprite_t;

static mp_obj_t sasppu_sprite_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in)
{
    sasppu_sprite_t *o = mp_obj_malloc(sasppu_sprite_t, type);

    return MP_OBJ_FROM_PTR(o);
}

static const mp_rom_map_elem_t sasppu_sprite_locals_dict_table[] = {};
static MP_DEFINE_CONST_DICT(sasppu_sprite_locals_dict, sasppu_sprite_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    sasppu_type_sprite,
    MP_QSTR_Sprite,
    MP_TYPE_FLAG_NONE,
    make_new, sasppu_sprite_make_new,
    locals_dict, &sasppu_sprite_locals_dict);

typedef struct
{
    mp_obj_base_t base;
    mp_uint_t flip_x;
    mp_uint_t flip_y;
    mp_uint_t tile;
} sasppu_tile_t;

static mp_obj_t sasppu_tile_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in)
{
    sasppu_tile_t *o = mp_obj_malloc(sasppu_tile_t, type);

    return MP_OBJ_FROM_PTR(o);
}

static const mp_rom_map_elem_t sasppu_tile_locals_dict_table[] = {};
static MP_DEFINE_CONST_DICT(sasppu_tile_locals_dict, sasppu_tile_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    sasppu_type_tile,
    MP_QSTR_Tile,
    MP_TYPE_FLAG_NONE,
    make_new, sasppu_tile_make_new,
    locals_dict, &sasppu_tile_locals_dict);

static mp_obj_t bg0_state_accessor(size_t n_args, const mp_obj_t *args)
{
    if (n_args > 0)
    {
        sasppu_background_t *value = MP_OBJ_TO_PTR(args[0]);
        SASPPU_bg0_state.x = value->x;
        SASPPU_bg0_state.y = value->y;
        SASPPU_bg0_state.windows = value->windows;
        SASPPU_bg0_state.flags = value->flags;
        return mp_const_none;
    }
    else
    {
        sasppu_background_t *value = MP_OBJ_TO_PTR(sasppu_background_make_new((mp_obj_type_t *)&sasppu_type_background, 0, 0, NULL));
        value->x = SASPPU_bg0_state.x;
        value->y = SASPPU_bg0_state.y;
        value->windows = SASPPU_bg0_state.windows;
        value->flags = SASPPU_bg0_state.flags;
        return MP_OBJ_FROM_PTR(value);
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(bg0_state_obj, 0, 1, bg0_state_accessor);

static mp_obj_t bg1_state_accessor(size_t n_args, const mp_obj_t *args)
{
    if (n_args > 0)
    {
        sasppu_background_t *value = MP_OBJ_TO_PTR(args[0]);
        SASPPU_bg1_state.x = value->x;
        SASPPU_bg1_state.y = value->y;
        SASPPU_bg1_state.windows = value->windows;
        SASPPU_bg1_state.flags = value->flags;
        return mp_const_none;
    }
    else
    {
        sasppu_background_t *value = MP_OBJ_TO_PTR(sasppu_background_make_new((mp_obj_type_t *)&sasppu_type_background, 0, 0, NULL));
        value->x = SASPPU_bg1_state.x;
        value->y = SASPPU_bg1_state.y;
        value->windows = SASPPU_bg1_state.windows;
        value->flags = SASPPU_bg1_state.flags;
        return MP_OBJ_FROM_PTR(value);
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(bg1_state_obj, 0, 1, bg1_state_accessor);

static mp_obj_t oam_accessor(size_t n_args, const mp_obj_t *args)
{
    if (n_args > 1)
    {
        mp_int_t index = mp_obj_get_int(args[0]);
        sasppu_sprite_t *value = MP_OBJ_TO_PTR(args[1]);
        SASPPU_oam[index].x = value->x;
        SASPPU_oam[index].y = value->y;
        SASPPU_oam[index].width = value->width;
        SASPPU_oam[index].height = value->height;
        SASPPU_oam[index].graphics_x = value->graphics_x;
        SASPPU_oam[index].graphics_y = value->graphics_y;
        SASPPU_oam[index].windows = value->windows;
        SASPPU_oam[index].flags = value->flags;
        return mp_const_none;
    }
    else
    {
        mp_int_t index = mp_obj_get_int(args[0]);
        sasppu_sprite_t *value = MP_OBJ_TO_PTR(sasppu_sprite_make_new((mp_obj_type_t *)&sasppu_type_sprite, 0, 0, NULL));
        value->x = SASPPU_oam[index].x;
        value->y = SASPPU_oam[index].y;
        value->width = SASPPU_oam[index].width;
        value->height = SASPPU_oam[index].height;
        value->graphics_x = SASPPU_oam[index].graphics_x;
        value->graphics_y = SASPPU_oam[index].graphics_y;
        value->windows = SASPPU_oam[index].windows;
        value->flags = SASPPU_oam[index].flags;
        return MP_OBJ_FROM_PTR(value);
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(oam_obj, 1, 2, oam_accessor);

static mp_obj_t bg0_map_accessor(size_t n_args, const mp_obj_t *args)
{
    if (n_args == 3)
    {
        mp_int_t index_x = mp_obj_get_int(args[0]);
        mp_int_t index_y = mp_obj_get_int(args[1]);
        mp_int_t index = index_y * MAP_WIDTH + index_x;
        sasppu_tile_t *value = MP_OBJ_TO_PTR(args[2]);
        SASPPU_bg0[index] = (value->tile & 0xFFF8);
        SASPPU_bg0[index] |= (value->flip_x ? 0b001 : 0);
        SASPPU_bg0[index] |= (value->flip_y ? 0b010 : 0);
        return mp_const_none;
    }
    else if (n_args == 2)
    {
        mp_int_t index_x = mp_obj_get_int(args[0]);
        mp_int_t index_y = mp_obj_get_int(args[1]);
        mp_int_t index = index_y * MAP_WIDTH + index_x;
        sasppu_tile_t *value = MP_OBJ_TO_PTR(sasppu_tile_make_new((mp_obj_type_t *)&sasppu_type_tile, 0, 0, NULL));
        value->tile = SASPPU_bg0[index] & 0xFFF8;
        value->flip_x = (SASPPU_bg0[index] & 0b001) > 0;
        value->flip_y = (SASPPU_bg0[index] & 0b010) > 0;
        return MP_OBJ_FROM_PTR(value);
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(bg0_map_obj, 2, 3, bg0_map_accessor);

static mp_obj_t bg1_map_accessor(size_t n_args, const mp_obj_t *args)
{
    if (n_args == 3)
    {
        mp_int_t index_x = mp_obj_get_int(args[0]);
        mp_int_t index_y = mp_obj_get_int(args[1]);
        mp_int_t index = index_y * MAP_WIDTH + index_x;
        sasppu_tile_t *value = MP_OBJ_TO_PTR(args[2]);
        SASPPU_bg1[index] = (value->tile & 0xFFF8);
        SASPPU_bg1[index] |= (value->flip_x ? 0b001 : 0);
        SASPPU_bg1[index] |= (value->flip_y ? 0b010 : 0);
        return mp_const_none;
    }
    else if (n_args == 2)
    {
        mp_int_t index_x = mp_obj_get_int(args[0]);
        mp_int_t index_y = mp_obj_get_int(args[1]);
        mp_int_t index = index_y * MAP_WIDTH + index_x;
        sasppu_tile_t *value = MP_OBJ_TO_PTR(sasppu_tile_make_new((mp_obj_type_t *)&sasppu_type_tile, 0, 0, NULL));
        value->tile = SASPPU_bg1[index] & 0xFFF8;
        value->flip_x = (SASPPU_bg1[index] & 0b001) > 0;
        value->flip_y = (SASPPU_bg1[index] & 0b010) > 0;
        return MP_OBJ_FROM_PTR(value);
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(bg1_map_obj, 2, 3, bg1_map_accessor);

static mp_obj_t render(void)
{
    SASPPU_render();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(render_obj, render);

static mp_obj_t init(void)
{
    SASPPU_gfx_reset();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(init_obj, init);

// Define all attributes of the module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
static const mp_rom_map_elem_t sasppu_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_sasppu)},
    {MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&init_obj)},

    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_main_state_mainscreen_colour), MP_ROM_PTR(&main_state_mainscreen_colour_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_main_state_subscreen_colour), MP_ROM_PTR(&main_state_subscreen_colour_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_main_state_window_1_left), MP_ROM_PTR(&main_state_window_1_left_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_main_state_window_1_right), MP_ROM_PTR(&main_state_window_1_right_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_main_state_window_2_left), MP_ROM_PTR(&main_state_window_2_left_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_main_state_window_2_right), MP_ROM_PTR(&main_state_window_2_right_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_main_state_bgcol_windows), MP_ROM_PTR(&main_state_bgcol_windows_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_main_state_flags), MP_ROM_PTR(&main_state_flags_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_cmath_state_screen_fade), MP_ROM_PTR(&cmath_state_screen_fade_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_cmath_state_flags), MP_ROM_PTR(&cmath_state_flags_obj)},

    {MP_ROM_QSTR(MP_QSTR_Background), MP_ROM_PTR(&sasppu_type_background)},
    {MP_ROM_QSTR(MP_QSTR_Sprite), MP_ROM_PTR(&sasppu_type_sprite)},

    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_render), MP_ROM_PTR(&render_obj)},

    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_bg0_state), MP_ROM_PTR(&bg0_state_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_bg1_state), MP_ROM_PTR(&bg1_state_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_oam), MP_ROM_PTR(&oam_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_bg0_map), MP_ROM_PTR(&bg0_map_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_bg1_map), MP_ROM_PTR(&bg1_map_obj)},

    {MP_ROM_QSTR(MP_QSTR_BG_WIDTH_POWER), MP_ROM_INT(BG_WIDTH_POWER)},
    {MP_ROM_QSTR(MP_QSTR_BG_HEIGHT_POWER), MP_ROM_INT(BG_HEIGHT_POWER)},
    {MP_ROM_QSTR(MP_QSTR_BG_WIDTH), MP_ROM_INT(BG_WIDTH)},
    {MP_ROM_QSTR(MP_QSTR_BG_HEIGHT), MP_ROM_INT(BG_HEIGHT)},
    {MP_ROM_QSTR(MP_QSTR_SPRITE_COUNT), MP_ROM_INT(SPRITE_COUNT)},
    {MP_ROM_QSTR(MP_QSTR_SPRITE_CACHE), MP_ROM_INT(SPRITE_CACHE)},
    {MP_ROM_QSTR(MP_QSTR_SPR_WIDTH_POWER), MP_ROM_INT(SPR_WIDTH_POWER)},
    {MP_ROM_QSTR(MP_QSTR_SPR_HEIGHT_POWER), MP_ROM_INT(SPR_HEIGHT_POWER)},
    {MP_ROM_QSTR(MP_QSTR_SPR_WIDTH), MP_ROM_INT(SPR_WIDTH)},
    {MP_ROM_QSTR(MP_QSTR_SPR_HEIGHT), MP_ROM_INT(SPR_HEIGHT)},
    {MP_ROM_QSTR(MP_QSTR_MAP_WIDTH_POWER), MP_ROM_INT(MAP_WIDTH_POWER)},
    {MP_ROM_QSTR(MP_QSTR_MAP_HEIGHT_POWER), MP_ROM_INT(MAP_HEIGHT_POWER)},
    {MP_ROM_QSTR(MP_QSTR_MAP_WIDTH), MP_ROM_INT(MAP_WIDTH)},
    {MP_ROM_QSTR(MP_QSTR_MAP_HEIGHT), MP_ROM_INT(MAP_HEIGHT)},
    {MP_ROM_QSTR(MP_QSTR_SPR_ENABLED), MP_ROM_INT(SPR_ENABLED)},
    {MP_ROM_QSTR(MP_QSTR_SPR_PRIORITY), MP_ROM_INT(SPR_PRIORITY)},
    {MP_ROM_QSTR(MP_QSTR_SPR_FLIP_X), MP_ROM_INT(SPR_FLIP_X)},
    {MP_ROM_QSTR(MP_QSTR_SPR_FLIP_Y), MP_ROM_INT(SPR_FLIP_Y)},
    {MP_ROM_QSTR(MP_QSTR_SPR_C_MATH), MP_ROM_INT(SPR_C_MATH)},
    {MP_ROM_QSTR(MP_QSTR_SPR_DOUBLE), MP_ROM_INT(SPR_DOUBLE)},
    {MP_ROM_QSTR(MP_QSTR_BG_C_MATH), MP_ROM_INT(BG_C_MATH)},
    {MP_ROM_QSTR(MP_QSTR_CMATH_HALF_MAIN_SCREEN), MP_ROM_INT(CMATH_HALF_MAIN_SCREEN)},
    {MP_ROM_QSTR(MP_QSTR_CMATH_DOUBLE_MAIN_SCREEN), MP_ROM_INT(CMATH_DOUBLE_MAIN_SCREEN)},
    {MP_ROM_QSTR(MP_QSTR_CMATH_HALF_SUB_SCREEN), MP_ROM_INT(CMATH_HALF_SUB_SCREEN)},
    {MP_ROM_QSTR(MP_QSTR_CMATH_DOUBLE_SUB_SCREEN), MP_ROM_INT(CMATH_DOUBLE_SUB_SCREEN)},
    {MP_ROM_QSTR(MP_QSTR_CMATH_ADD_SUB_SCREEN), MP_ROM_INT(CMATH_ADD_SUB_SCREEN)},
    {MP_ROM_QSTR(MP_QSTR_CMATH_SUB_SUB_SCREEN), MP_ROM_INT(CMATH_SUB_SUB_SCREEN)},
    {MP_ROM_QSTR(MP_QSTR_CMATH_FADE_ENABLE), MP_ROM_INT(CMATH_FADE_ENABLE)},
    {MP_ROM_QSTR(MP_QSTR_CMATH_CMATH_ENABLE), MP_ROM_INT(CMATH_CMATH_ENABLE)},
    {MP_ROM_QSTR(MP_QSTR_MAIN_SPR0_ENABLE), MP_ROM_INT(MAIN_SPR0_ENABLE)},
    {MP_ROM_QSTR(MP_QSTR_MAIN_SPR1_ENABLE), MP_ROM_INT(MAIN_SPR1_ENABLE)},
    {MP_ROM_QSTR(MP_QSTR_MAIN_BG0_ENABLE), MP_ROM_INT(MAIN_BG0_ENABLE)},
    {MP_ROM_QSTR(MP_QSTR_MAIN_BG1_ENABLE), MP_ROM_INT(MAIN_BG1_ENABLE)},
    {MP_ROM_QSTR(MP_QSTR_MAIN_CMATH_ENABLE), MP_ROM_INT(MAIN_CMATH_ENABLE)},
    {MP_ROM_QSTR(MP_QSTR_MAIN_BGCOL_WINDOW_ENABLE), MP_ROM_INT(MAIN_BGCOL_WINDOW_ENABLE)},
    {MP_ROM_QSTR(MP_QSTR_WINDOW_A), MP_ROM_INT(WINDOW_A)},
    {MP_ROM_QSTR(MP_QSTR_WINDOW_B), MP_ROM_INT(WINDOW_B)},
    {MP_ROM_QSTR(MP_QSTR_WINDOW_AB), MP_ROM_INT(WINDOW_AB)},
    {MP_ROM_QSTR(MP_QSTR_WINDOW_X), MP_ROM_INT(WINDOW_X)},
    {MP_ROM_QSTR(MP_QSTR_WINDOW_ALL), MP_ROM_INT(WINDOW_ALL)},
};
static MP_DEFINE_CONST_DICT(sasppu_globals, sasppu_globals_table);

// Define module object.
const mp_obj_module_t sasppu_module = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&sasppu_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_sasppu, sasppu_module);
