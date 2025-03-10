// Include the header file to get access to the MicroPython API
#include "py/runtime.h"
#include "sasppu_fast.h"

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

// This defines the type(Timer) object.
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

// This defines the type(Timer) object.
MP_DEFINE_CONST_OBJ_TYPE(
    sasppu_type_sprite,
    MP_QSTR_Sprite,
    MP_TYPE_FLAG_NONE,
    make_new, sasppu_sprite_make_new,
    locals_dict, &sasppu_sprite_locals_dict);

static mp_obj_t bg0_state_accessor(size_t n_args, const mp_obj_t *args)
{
    if (n_args > 0)
    {
        sasppu_background_t *value = MP_OBJ_TO_PTR(args[0]);
        SASPPU_bg0_state.x = value->x;
        SASPPU_bg0_state.y = value->y;
        SASPPU_bg0_state.windows = value->windows;
        SASPPU_bg0_state.flags = value->flags;
        return args[0];
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
        return args[0];
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
        return args[0];
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
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(oam_obj, 1, 2, oam_accessor);

static mp_obj_t get_framebuffer(void)
{
    // Create a list holding all items from the framebuffer
    mp_obj_list_t *lst = MP_OBJ_TO_PTR(mp_obj_new_list(MP_ARRAY_SIZE(SASPPU_frame_buffer), NULL));
    for (int i = 0; i < MP_ARRAY_SIZE(SASPPU_frame_buffer); ++i)
    {
        lst->items[i] = mp_obj_new_int(SASPPU_frame_buffer[i]);
    }
    return MP_OBJ_FROM_PTR(lst);
}
static MP_DEFINE_CONST_FUN_OBJ_0(get_framebuffer_obj, get_framebuffer);

static mp_obj_t render(void)
{
    SASPPU_render();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(render_obj, render);

static mp_obj_t render_scanline(mp_obj_t x, mp_obj_t y)
{
    SASPPU_render_scanline(mp_obj_get_int(x), mp_obj_get_int(y));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(render_scanline_obj, render_scanline);

static mp_obj_t per_scanline(mp_obj_t x, mp_obj_t y)
{
    SASPPU_per_scanline(mp_obj_get_int(x), mp_obj_get_int(y));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(per_scanline_obj, per_scanline);

// Define all attributes of the module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
static const mp_rom_map_elem_t sasppu_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_sasppu)},

    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_main_state_mainscreen_colour), MP_ROM_PTR(&main_state_mainscreen_colour_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_main_state_subscreen_colour), MP_ROM_PTR(&main_state_subscreen_colour_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_main_state_window_1_left), MP_ROM_PTR(&main_state_window_1_left_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_main_state_window_1_right), MP_ROM_PTR(&main_state_window_1_right_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_main_state_window_2_left), MP_ROM_PTR(&main_state_window_2_left_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_main_state_window_2_right), MP_ROM_PTR(&main_state_window_2_right_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_main_state_flags), MP_ROM_PTR(&main_state_flags_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_cmath_state_screen_fade), MP_ROM_PTR(&cmath_state_screen_fade_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_cmath_state_flags), MP_ROM_PTR(&cmath_state_flags_obj)},

    {MP_ROM_QSTR(MP_QSTR_Background), MP_ROM_PTR(&sasppu_type_background)},
    {MP_ROM_QSTR(MP_QSTR_Sprite), MP_ROM_PTR(&sasppu_type_sprite)},

    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_render), MP_ROM_PTR(&render_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_render_scanline), MP_ROM_PTR(&render_scanline_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_per_scanline), MP_ROM_PTR(&per_scanline_obj)},

    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_bg0_state), MP_ROM_PTR(&bg0_state_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_bg1_state), MP_ROM_PTR(&bg1_state_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_oam), MP_ROM_PTR(&oam_obj)},

    {MP_ROM_QSTR(MP_QSTR_sasppuinteral_get_framebuffer), MP_ROM_PTR(&get_framebuffer_obj)},

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
    {MP_ROM_QSTR(MP_QSTR_WINDOW_A), MP_ROM_INT(WINDOW_A)},
    {MP_ROM_QSTR(MP_QSTR_WINDOW_B), MP_ROM_INT(WINDOW_B)},
    {MP_ROM_QSTR(MP_QSTR_WINDOW_AB), MP_ROM_INT(WINDOW_AB)},
    {MP_ROM_QSTR(MP_QSTR_WINDOW_X), MP_ROM_INT(WINDOW_X)},
};
static MP_DEFINE_CONST_DICT(sasppu_globals, sasppu_globals_table);

// Define module object.
const mp_obj_module_t sasppu_module = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&sasppu_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_sasppu, sasppu_module);
