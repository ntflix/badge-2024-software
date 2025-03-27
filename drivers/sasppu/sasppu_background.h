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

#include "sasppu_windows.h"

typedef struct
{
    mp_obj_base_t base;
    mp_int_t x;
    mp_int_t y;
    mp_obj_t windows;
    mp_uint_t flags;
} sasppu_background_t;

extern const mp_obj_type_t sasppu_type_background;

mp_obj_t sasppu_background_default(mp_obj_t self_in);

mp_obj_t sasppu_background_from_struct(mp_obj_t self_in, Background bg);
Background sasppu_background_to_struct(mp_obj_t self_in);