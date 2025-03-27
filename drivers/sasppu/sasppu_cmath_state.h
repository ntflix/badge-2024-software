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
    mp_uint_t fade;
    mp_uint_t flags;
} sasppu_cmath_state_t;

extern const mp_obj_type_t sasppu_type_cmath_state;

mp_obj_t sasppu_cmath_state_default(mp_obj_t self_in);

mp_obj_t sasppu_cmath_state_from_struct(mp_obj_t self_in, CMathState cs);
CMathState sasppu_cmath_state_to_struct(mp_obj_t self_in);