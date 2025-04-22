// Include the header file to get access to the MicroPython API
#include "py/runtime.h"
#include <stdlib.h>
#include "py/binary.h"
#include "py/obj.h"
#include "py/objarray.h"
#include "py/objstr.h"
#include "sasppu/sasppu.h"
#include "sasppu/help.h"
#include "display.h"

#include "sasppu_background.h"
#include "sasppu_cmath_state.h"
#include "sasppu_main_state.h"
#include "sasppu_sprite.h"

#include "sasppu_oam.h"
#include "sasppu_map.h"

#include "sasppu_mpy_help.h"

static mp_obj_t init(void)
{
    SASPPU_gfx_reset();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(init_obj, init);

void sasppu_module_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest)
{
    if (dest[0] == MP_OBJ_NULL)
    {
        // Load attribute.
        switch (attr)
        {
        case MP_QSTR_oam:
            dest[0] = mp_obj_malloc(sasppu_oam_t, &sasppu_type_oam);
            break;
        case MP_QSTR_bg0:
            dest[0] = mp_obj_malloc(sasppu_map_t, &sasppu_type_map);
            ((sasppu_map_t *)MP_OBJ_TO_PTR(dest[0]))->bg = 0;
            break;
        case MP_QSTR_bg1:
            dest[0] = mp_obj_malloc(sasppu_map_t, &sasppu_type_map);
            ((sasppu_map_t *)MP_OBJ_TO_PTR(dest[0]))->bg = 1;
            break;
        default:
            dest[1] = MP_OBJ_SENTINEL;
            break;
        }
    }
}

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

    {MP_ROM_QSTR(MP_QSTR_copy_sprite), MP_ROM_PTR(&sasppu_copy_sprite_obj)},
    {MP_ROM_QSTR(MP_QSTR_copy_sprite_transparent), MP_ROM_PTR(&sasppu_copy_sprite_transparent_obj)},
    {MP_ROM_QSTR(MP_QSTR_fill_sprite), MP_ROM_PTR(&sasppu_fill_sprite_obj)},
    {MP_ROM_QSTR(MP_QSTR_blit_sprite), MP_ROM_PTR(&sasppu_blit_sprite_obj)},
    {MP_ROM_QSTR(MP_QSTR_blit_sprite_transparent), MP_ROM_PTR(&sasppu_blit_sprite_transparent_obj)},
    {MP_ROM_QSTR(MP_QSTR_paletted_sprite), MP_ROM_PTR(&sasppu_paletted_sprite_obj)},
    {MP_ROM_QSTR(MP_QSTR_paletted_sprite_transparent), MP_ROM_PTR(&sasppu_paletted_sprite_transparent_obj)},
    {MP_ROM_QSTR(MP_QSTR_compressed_sprite), MP_ROM_PTR(&sasppu_compressed_sprite_obj)},
    {MP_ROM_QSTR(MP_QSTR_compressed_sprite_transparent), MP_ROM_PTR(&sasppu_compressed_sprite_transparent_obj)},
    {MP_ROM_QSTR(MP_QSTR_draw_text_sprite), MP_ROM_PTR(&sasppu_draw_text_sprite_obj)},
    {MP_ROM_QSTR(MP_QSTR_draw_text_next_sprite), MP_ROM_PTR(&sasppu_draw_text_next_sprite_obj)},

    {MP_ROM_QSTR(MP_QSTR_copy_background), MP_ROM_PTR(&sasppu_copy_background_obj)},
    {MP_ROM_QSTR(MP_QSTR_copy_background_transparent), MP_ROM_PTR(&sasppu_copy_background_transparent_obj)},
    {MP_ROM_QSTR(MP_QSTR_fill_background), MP_ROM_PTR(&sasppu_fill_background_obj)},
    {MP_ROM_QSTR(MP_QSTR_blit_background), MP_ROM_PTR(&sasppu_blit_background_obj)},
    {MP_ROM_QSTR(MP_QSTR_blit_background_transparent), MP_ROM_PTR(&sasppu_blit_background_transparent_obj)},
    {MP_ROM_QSTR(MP_QSTR_paletted_background), MP_ROM_PTR(&sasppu_paletted_background_obj)},
    {MP_ROM_QSTR(MP_QSTR_paletted_background_transparent), MP_ROM_PTR(&sasppu_paletted_background_transparent_obj)},
    {MP_ROM_QSTR(MP_QSTR_compressed_background), MP_ROM_PTR(&sasppu_compressed_background_obj)},
    {MP_ROM_QSTR(MP_QSTR_compressed_background_transparent), MP_ROM_PTR(&sasppu_compressed_background_transparent_obj)},
    {MP_ROM_QSTR(MP_QSTR_draw_text_background), MP_ROM_PTR(&sasppu_draw_text_background_obj)},
    {MP_ROM_QSTR(MP_QSTR_draw_text_next_background), MP_ROM_PTR(&sasppu_draw_text_next_background_obj)},

    {MP_ROM_QSTR(MP_QSTR_mul_rgb555), MP_ROM_PTR(&sasppu_macro_mul_rgb555_obj)},
    {MP_ROM_QSTR(MP_QSTR_cmath), MP_ROM_PTR(&sasppu_macro_cmath_obj)},
    {MP_ROM_QSTR(MP_QSTR_rgb555), MP_ROM_PTR(&sasppu_macro_rgb555_obj)},
    {MP_ROM_QSTR(MP_QSTR_rgb555_cmath), MP_ROM_PTR(&sasppu_macro_rgb555_cmath_obj)},
    {MP_ROM_QSTR(MP_QSTR_rgb888), MP_ROM_PTR(&sasppu_macro_rgb888_obj)},
    {MP_ROM_QSTR(MP_QSTR_rgb888_cmath), MP_ROM_PTR(&sasppu_macro_rgb888_cmath_obj)},
    {MP_ROM_QSTR(MP_QSTR_grey555), MP_ROM_PTR(&sasppu_macro_grey555_obj)},
    {MP_ROM_QSTR(MP_QSTR_grey555_cmath), MP_ROM_PTR(&sasppu_macro_grey555_cmath_obj)},
    {MP_ROM_QSTR(MP_QSTR_mul_channel), MP_ROM_PTR(&sasppu_macro_mul_channel_obj)},
    {MP_ROM_QSTR(MP_QSTR_r_channel), MP_ROM_PTR(&sasppu_macro_r_channel_obj)},
    {MP_ROM_QSTR(MP_QSTR_g_channel), MP_ROM_PTR(&sasppu_macro_g_channel_obj)},
    {MP_ROM_QSTR(MP_QSTR_b_channel), MP_ROM_PTR(&sasppu_macro_b_channel_obj)},
    {MP_ROM_QSTR(MP_QSTR_cmath_channel), MP_ROM_PTR(&sasppu_macro_cmath_channel_obj)},
    {MP_ROM_QSTR(MP_QSTR_mul_col), MP_ROM_PTR(&sasppu_macro_mul_col_obj)},

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

    {MP_ROM_QSTR(MP_QSTR_BPP1), MP_ROM_INT(0)},
    {MP_ROM_QSTR(MP_QSTR_BPP2), MP_ROM_INT(1)},
    {MP_ROM_QSTR(MP_QSTR_BPP4), MP_ROM_INT(2)},
    {MP_ROM_QSTR(MP_QSTR_BPP8), MP_ROM_INT(3)},

    {MP_ROM_QSTR(MP_QSTR_TRANSPARENT_BLACK), MP_ROM_INT(SASPPU_RGB555(0, 0, 0))},
    {MP_ROM_QSTR(MP_QSTR_OPAQUE_BLACK), MP_ROM_INT(SASPPU_RGB555(1, 0, 0))},
    {MP_ROM_QSTR(MP_QSTR_RED), MP_ROM_INT(SASPPU_RGB555(31, 0, 0))},
    {MP_ROM_QSTR(MP_QSTR_GREEN), MP_ROM_INT(SASPPU_RGB555(0, 31, 0))},
    {MP_ROM_QSTR(MP_QSTR_BLUE), MP_ROM_INT(SASPPU_RGB555(0, 0, 31))},
    {MP_ROM_QSTR(MP_QSTR_WHITE), MP_ROM_INT(SASPPU_RGB555(31, 31, 31))},
};
static MP_DEFINE_CONST_DICT(sasppu_globals, sasppu_globals_table);

// Define module object.
const mp_obj_module_t sasppu_module = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&sasppu_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_sasppu, sasppu_module);

MP_REGISTER_MODULE_DELEGATION(sasppu_module, sasppu_module_attr);
