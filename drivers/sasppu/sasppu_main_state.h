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
    MainState dat;
    bool bound;
} sasppu_main_state_t;

extern const mp_obj_type_t sasppu_type_main_state;

mp_obj_t sasppu_main_state_default(mp_obj_t self_in);