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
    mp_uint_t window_1_state;
    mp_uint_t window_2_state;
} sasppu_windows_t;

extern const mp_obj_type_t sasppu_type_windows;

mp_obj_t sasppu_windows_default(mp_obj_t self_in);
mp_obj_t sasppu_windows_from_int(mp_obj_t self_in, mp_int_t int_in);
mp_int_t sasppu_windows_to_int(mp_obj_t self_in);