// Include the header file to get access to the MicroPython API
#include "py/runtime.h"
#include <stdlib.h>
#include "py/binary.h"
#include "py/obj.h"
#include "py/objarray.h"
#include "sasppu.h"
#include "sasppu_help.h"
#include "display.h"

#include "sasppu_windows.h"
#include "sasppu_background.h"
#include "sasppu_cmath_state.h"
#include "sasppu_main_state.h"
#include "sasppu_sprite.h"
#include "sasppu_tile.h"
#include "sasppu_global.h"

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
        SASPPU_oam[index].windows = sasppu_windows_to_int(value->windows);
        SASPPU_oam[index].flags = value->flags;
        return mp_const_none;
    }
    else
    {
        mp_int_t index = mp_obj_get_int(args[0]);
        sasppu_sprite_t *value = mp_obj_malloc(sasppu_sprite_t, &sasppu_type_sprite);
        value->windows = mp_obj_malloc(sasppu_windows_t, &sasppu_type_windows);
        sasppu_windows_from_int(value->windows, SASPPU_oam[index].windows);
        value->x = SASPPU_oam[index].x;
        value->y = SASPPU_oam[index].y;
        value->width = SASPPU_oam[index].width;
        value->height = SASPPU_oam[index].height;
        value->graphics_x = SASPPU_oam[index].graphics_x;
        value->graphics_y = SASPPU_oam[index].graphics_y;
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
        SASPPU_bg0[index] = sasppu_tile_to_int(args[2]);
        return mp_const_none;
    }
    else if (n_args == 2)
    {
        mp_int_t index_x = mp_obj_get_int(args[0]);
        mp_int_t index_y = mp_obj_get_int(args[1]);
        mp_int_t index = index_y * MAP_WIDTH + index_x;
        mp_obj_t value = mp_obj_malloc(sasppu_tile_t, &sasppu_type_tile);
        sasppu_tile_from_int(value, SASPPU_bg0[index]);
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
        SASPPU_bg1[index] = sasppu_tile_to_int(args[2]);
        return mp_const_none;
    }
    else if (n_args == 2)
    {
        mp_int_t index_x = mp_obj_get_int(args[0]);
        mp_int_t index_y = mp_obj_get_int(args[1]);
        mp_int_t index = index_y * MAP_WIDTH + index_x;
        mp_obj_t value = mp_obj_malloc(sasppu_tile_t, &sasppu_type_tile);
        sasppu_tile_from_int(value, SASPPU_bg1[index]);
        return MP_OBJ_FROM_PTR(value);
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
    {MP_ROM_QSTR(MP_QSTR_Tile), MP_ROM_PTR(&sasppu_type_tile)},
    {MP_ROM_QSTR(MP_QSTR_Windows), MP_ROM_PTR(&sasppu_type_windows)},
    {MP_ROM_QSTR(MP_QSTR_Global), MP_ROM_PTR(&sasppu_type_global)},

    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_oam), MP_ROM_PTR(&oam_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_bg0_map), MP_ROM_PTR(&bg0_map_obj)},
    {MP_ROM_QSTR(MP_QSTR_sasppuinternal_bg1_map), MP_ROM_PTR(&bg1_map_obj)},

    {MP_ROM_QSTR(MP_QSTR_SPRITE_COUNT), MP_ROM_INT(SPRITE_COUNT)},
    {MP_ROM_QSTR(MP_QSTR_SPRITE_CACHE), MP_ROM_INT(SPRITE_CACHE)},

    {MP_ROM_QSTR(MP_QSTR_MAP_WIDTH_POWER), MP_ROM_INT(MAP_WIDTH_POWER)},
    {MP_ROM_QSTR(MP_QSTR_MAP_HEIGHT_POWER), MP_ROM_INT(MAP_HEIGHT_POWER)},
    {MP_ROM_QSTR(MP_QSTR_MAP_WIDTH), MP_ROM_INT(MAP_WIDTH)},
    {MP_ROM_QSTR(MP_QSTR_MAP_HEIGHT), MP_ROM_INT(MAP_HEIGHT)},
};
static MP_DEFINE_CONST_DICT(sasppu_globals, sasppu_globals_table);

// Define module object.
const mp_obj_module_t sasppu_module = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&sasppu_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_sasppu, sasppu_module);
