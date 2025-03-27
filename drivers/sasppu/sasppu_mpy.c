// Include the header file to get access to the MicroPython API
#include "py/runtime.h"
#include <stdlib.h>
#include "py/binary.h"
#include "py/obj.h"
#include "py/objarray.h"
#include "py/objstr.h"
#include "sasppu.h"
#include "sasppu_help.h"
#include "display.h"

#include "sasppu_background.h"
#include "sasppu_cmath_state.h"
#include "sasppu_main_state.h"
#include "sasppu_sprite.h"

static mp_obj_t oam_accessor(size_t n_args, const mp_obj_t *args)
{
    if (n_args > 1)
    {
        mp_int_t index = mp_obj_get_int(args[0]);
        sasppu_sprite_t *value = MP_OBJ_TO_PTR(args[1]);
        SASPPU_oam[index] = value->dat;
        return mp_const_none;
    }
    else
    {
        mp_int_t index = mp_obj_get_int(args[0]);
        sasppu_sprite_t *value = mp_obj_malloc(sasppu_sprite_t, &sasppu_type_sprite);
        value->dat = SASPPU_oam[index];
        value->bound = index;
        return MP_OBJ_FROM_PTR(value);
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(oam_obj, 1, 2, oam_accessor);

static mp_obj_t bg0_map_accessor(size_t n_args, const mp_obj_t *args)
{
    mp_int_t index_x = mp_obj_get_int(args[0]);
    mp_int_t index_y = mp_obj_get_int(args[1]);
    mp_int_t index = index_y * MAP_WIDTH + index_x;
    if (n_args == 3)
    {
        SASPPU_bg0[index] = mp_obj_get_int(args[2]);
        return mp_const_none;
    }
    else if (n_args == 2)
    {
        return MP_OBJ_NEW_SMALL_INT(SASPPU_bg0[index]);
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(bg0_map_obj, 2, 3, bg0_map_accessor);

static mp_obj_t bg1_map_accessor(size_t n_args, const mp_obj_t *args)
{
    mp_int_t index_x = mp_obj_get_int(args[0]);
    mp_int_t index_y = mp_obj_get_int(args[1]);
    mp_int_t index = index_y * MAP_WIDTH + index_x;
    if (n_args == 3)
    {
        SASPPU_bg1[index] = mp_obj_get_int(args[2]);
        return mp_const_none;
    }
    else if (n_args == 2)
    {
        return MP_OBJ_NEW_SMALL_INT(SASPPU_bg1[index]);
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(bg1_map_obj, 2, 3, bg1_map_accessor);

static mp_obj_t init(void)
{
    SASPPU_gfx_reset();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(init_obj, init);

static mp_obj_t sasppu_copy_sprite(size_t n_args, const mp_obj_t *args)
{
    mp_int_t dst_x = mp_obj_get_int(args[0]);
    mp_int_t dst_y = mp_obj_get_int(args[1]);
    mp_int_t width = mp_obj_get_int(args[2]);
    mp_int_t height = mp_obj_get_int(args[3]);
    mp_int_t src_x = mp_obj_get_int(args[4]);
    mp_int_t src_y = mp_obj_get_int(args[5]);
    bool double_size = false;
    if (n_args == 7)
    {
        double_size = mp_obj_is_true(args[6]);
    }
    SASPPUImageCode res = SASPPU_copy_sprite(dst_x, dst_y, width, height, src_x, src_y, double_size);
    return MP_OBJ_NEW_SMALL_INT(res);
}

static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_copy_sprite_obj, 6, 7, sasppu_copy_sprite);

static mp_obj_t sasppu_copy_sprite_transparent(size_t n_args, const mp_obj_t *args)
{
    mp_int_t dst_x = mp_obj_get_int(args[0]);
    mp_int_t dst_y = mp_obj_get_int(args[1]);
    mp_int_t width = mp_obj_get_int(args[2]);
    mp_int_t height = mp_obj_get_int(args[3]);
    mp_int_t src_x = mp_obj_get_int(args[4]);
    mp_int_t src_y = mp_obj_get_int(args[5]);
    bool double_size = false;
    if (n_args == 7)
    {
        double_size = mp_obj_is_true(args[6]);
    }
    SASPPUImageCode res = SASPPU_copy_sprite_transparent(dst_x, dst_y, width, height, src_x, src_y, double_size);
    return MP_OBJ_NEW_SMALL_INT(res);
}

static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_copy_sprite_transparent_obj, 6, 7, sasppu_copy_sprite_transparent);

static mp_obj_t sasppu_fill_sprite(size_t n_args, const mp_obj_t *args)
{
    mp_int_t x = mp_obj_get_int(args[0]);
    mp_int_t y = mp_obj_get_int(args[1]);
    mp_int_t width = mp_obj_get_int(args[2]);
    mp_int_t height = mp_obj_get_int(args[3]);
    mp_int_t colour = mp_obj_get_int(args[4]);
    SASPPUImageCode res = SASPPU_fill_sprite(x, y, width, height, colour);
    return MP_OBJ_NEW_SMALL_INT(res);
}

static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_fill_sprite_obj, 5, 5, sasppu_fill_sprite);

static mp_obj_t sasppu_draw_text_sprite(size_t n_args, const mp_obj_t *args)
{
    mp_int_t x = mp_obj_get_int(args[0]);
    mp_int_t y = mp_obj_get_int(args[1]);
    mp_int_t colour = mp_obj_get_int(args[2]);
    mp_int_t line_width = mp_obj_get_int(args[3]);
    mp_check_self(mp_obj_is_str_or_bytes(args[4]));
    GET_STR_DATA_LEN(args[4], text, text_len);
    mp_int_t newline_height = 10;
    bool double_size = false;
    if (n_args == 6)
    {
        double_size = mp_obj_is_true(args[5]);
    }
    if (n_args == 7)
    {
        newline_height = mp_obj_get_int(args[6]);
    }
    SASPPUImageCode res = SASPPU_draw_text_sprite(x, y, colour, line_width, newline_height, double_size, (const char *)text);
    return MP_OBJ_NEW_SMALL_INT(res);
}

static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_draw_text_sprite_obj, 5, 7, sasppu_draw_text_sprite);

static mp_obj_t sasppu_copy_background(size_t n_args, const mp_obj_t *args)
{
    mp_int_t dst_x = mp_obj_get_int(args[0]);
    mp_int_t dst_y = mp_obj_get_int(args[1]);
    mp_int_t width = mp_obj_get_int(args[2]);
    mp_int_t height = mp_obj_get_int(args[3]);
    mp_int_t src_x = mp_obj_get_int(args[4]);
    mp_int_t src_y = mp_obj_get_int(args[5]);
    bool double_size = false;
    if (n_args == 7)
    {
        double_size = mp_obj_is_true(args[6]);
    }
    SASPPUImageCode res = SASPPU_copy_background(dst_x, dst_y, width, height, src_x, src_y, double_size);
    return MP_OBJ_NEW_SMALL_INT(res);
}

static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_copy_background_obj, 6, 7, sasppu_copy_background);

static mp_obj_t sasppu_copy_background_transparent(size_t n_args, const mp_obj_t *args)
{
    mp_int_t dst_x = mp_obj_get_int(args[0]);
    mp_int_t dst_y = mp_obj_get_int(args[1]);
    mp_int_t width = mp_obj_get_int(args[2]);
    mp_int_t height = mp_obj_get_int(args[3]);
    mp_int_t src_x = mp_obj_get_int(args[4]);
    mp_int_t src_y = mp_obj_get_int(args[5]);
    bool double_size = false;
    if (n_args == 7)
    {
        double_size = mp_obj_is_true(args[6]);
    }
    SASPPUImageCode res = SASPPU_copy_background_transparent(dst_x, dst_y, width, height, src_x, src_y, double_size);
    return MP_OBJ_NEW_SMALL_INT(res);
}

static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_copy_background_transparent_obj, 6, 7, sasppu_copy_background_transparent);

static mp_obj_t sasppu_fill_background(size_t n_args, const mp_obj_t *args)
{
    mp_int_t x = mp_obj_get_int(args[0]);
    mp_int_t y = mp_obj_get_int(args[1]);
    mp_int_t width = mp_obj_get_int(args[2]);
    mp_int_t height = mp_obj_get_int(args[3]);
    mp_int_t colour = mp_obj_get_int(args[4]);
    SASPPUImageCode res = SASPPU_fill_background(x, y, width, height, colour);
    return MP_OBJ_NEW_SMALL_INT(res);
}

static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_fill_background_obj, 5, 5, sasppu_fill_background);

static mp_obj_t sasppu_draw_text_background(size_t n_args, const mp_obj_t *args)
{
    mp_int_t x = mp_obj_get_int(args[0]);
    mp_int_t y = mp_obj_get_int(args[1]);
    mp_int_t colour = mp_obj_get_int(args[2]);
    mp_int_t line_width = mp_obj_get_int(args[3]);
    mp_check_self(mp_obj_is_str_or_bytes(args[4]));
    GET_STR_DATA_LEN(args[4], text, text_len);
    mp_int_t newline_height = 10;
    bool double_size = false;
    if (n_args == 6)
    {
        double_size = mp_obj_is_true(args[5]);
    }
    if (n_args == 7)
    {
        newline_height = mp_obj_get_int(args[6]);
    }
    SASPPUImageCode res = SASPPU_draw_text_background(x, y, colour, line_width, newline_height, double_size, (const char *)text);
    return MP_OBJ_NEW_SMALL_INT(res);
}

static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_draw_text_background_obj, 5, 7, sasppu_draw_text_background);

// Define all attributes of the module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
static const mp_rom_map_elem_t sasppu_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_sasppu)},
    {MP_ROM_QSTR(MP_QSTR___init__), MP_ROM_PTR(&init_obj)},

    {MP_ROM_QSTR(MP_QSTR_Background), MP_ROM_PTR(&sasppu_type_background)},
    {MP_ROM_QSTR(MP_QSTR_Sprite), MP_ROM_PTR(&sasppu_type_sprite)},
    {MP_ROM_QSTR(MP_QSTR_CMathState), MP_ROM_PTR(&sasppu_type_cmath_state)},
    {MP_ROM_QSTR(MP_QSTR_MainState), MP_ROM_PTR(&sasppu_type_main_state)},

    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_oam), MP_ROM_PTR(&oam_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_bg0_map), MP_ROM_PTR(&bg0_map_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_bg1_map), MP_ROM_PTR(&bg1_map_obj)},

    {MP_ROM_QSTR(MP_QSTR_copy_sprite), MP_ROM_PTR(&sasppu_copy_sprite_obj)},
    {MP_ROM_QSTR(MP_QSTR_copy_sprite_transparent), MP_ROM_PTR(&sasppu_copy_sprite_transparent_obj)},
    {MP_ROM_QSTR(MP_QSTR_fill_sprite), MP_ROM_PTR(&sasppu_fill_sprite_obj)},
    {MP_ROM_QSTR(MP_QSTR_draw_text_sprite), MP_ROM_PTR(&sasppu_draw_text_sprite_obj)},

    {MP_ROM_QSTR(MP_QSTR_copy_background), MP_ROM_PTR(&sasppu_copy_background_obj)},
    {MP_ROM_QSTR(MP_QSTR_copy_background_transparent), MP_ROM_PTR(&sasppu_copy_background_transparent_obj)},
    {MP_ROM_QSTR(MP_QSTR_fill_background), MP_ROM_PTR(&sasppu_fill_background_obj)},
    {MP_ROM_QSTR(MP_QSTR_draw_text_background), MP_ROM_PTR(&sasppu_draw_text_background_obj)},

    {MP_ROM_QSTR(MP_QSTR_SPRITE_COUNT), MP_ROM_INT(SPRITE_COUNT)},
    {MP_ROM_QSTR(MP_QSTR_SPRITE_CACHE), MP_ROM_INT(SPRITE_CACHE)},

    {MP_ROM_QSTR(MP_QSTR_MAP_WIDTH_POWER), MP_ROM_INT(MAP_WIDTH_POWER)},
    {MP_ROM_QSTR(MP_QSTR_MAP_HEIGHT_POWER), MP_ROM_INT(MAP_HEIGHT_POWER)},
    {MP_ROM_QSTR(MP_QSTR_MAP_WIDTH), MP_ROM_INT(MAP_WIDTH)},
    {MP_ROM_QSTR(MP_QSTR_MAP_HEIGHT), MP_ROM_INT(MAP_HEIGHT)},

    {MP_ROM_QSTR(MP_QSTR_IC_SUCCESS), MP_ROM_INT(SASPPU_IC_Success)},
    {MP_ROM_QSTR(MP_QSTR_IC_TOO_WIDE), MP_ROM_INT(SASPPU_IC_TooWide)},
    {MP_ROM_QSTR(MP_QSTR_IC_TOO_TALL), MP_ROM_INT(SASPPU_IC_TooTall)},
    {MP_ROM_QSTR(MP_QSTR_IC_INVALID_BIT_DEPTH), MP_ROM_INT(SASPPU_IC_InvalidBitdepth)},

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

// MP_REGISTER_MODULE_DELEGATION(sasppu_module, sasppu_module_attr);
