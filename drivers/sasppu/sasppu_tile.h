// Include the header file to get access to the MicroPython API
#pragma once
#include "py/runtime.h"
#include <stdlib.h>
#include "py/binary.h"
#include "py/obj.h"
#include "py/objarray.h"
#include "sasppu.h"
#include "sasppu_help.h"
#include "display.h"

typedef struct
{
    mp_obj_base_t base;
    mp_obj_t flip_x;
    mp_obj_t flip_y;
    mp_uint_t tile;
} sasppu_tile_t;

extern const mp_obj_type_t sasppu_type_tile;

mp_obj_t sasppu_tile_default(mp_obj_t self_in);

mp_obj_t sasppu_tile_from_int(mp_obj_t self_in, mp_int_t int_in);
mp_int_t sasppu_tile_to_int(mp_obj_t self_in);