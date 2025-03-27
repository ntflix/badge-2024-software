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
    mp_uint_t mainscreen_colour;
    mp_uint_t subscreen_colour;
    mp_uint_t window_1_left;
    mp_uint_t window_1_right;
    mp_uint_t window_2_left;
    mp_uint_t window_2_right;
    mp_obj_t bgcol_windows;
    mp_uint_t flags;
} sasppu_main_state_t;

extern const mp_obj_type_t sasppu_type_main_state;

mp_obj_t sasppu_main_state_default(mp_obj_t self_in);

mp_obj_t sasppu_main_state_from_struct(mp_obj_t self_in, MainState ms);
MainState sasppu_main_state_to_struct(mp_obj_t self_in);