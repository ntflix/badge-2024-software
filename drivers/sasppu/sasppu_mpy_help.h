#pragma once
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

extern const mp_obj_fun_builtin_var_t sasppu_copy_sprite_obj;
extern const mp_obj_fun_builtin_var_t sasppu_copy_sprite_transparent_obj;
extern const mp_obj_fun_builtin_var_t sasppu_fill_sprite_obj;
extern const mp_obj_fun_builtin_var_t sasppu_draw_text_sprite_obj;

extern const mp_obj_fun_builtin_var_t sasppu_copy_background_obj;
extern const mp_obj_fun_builtin_var_t sasppu_copy_background_transparent_obj;
extern const mp_obj_fun_builtin_var_t sasppu_fill_background_obj;
extern const mp_obj_fun_builtin_var_t sasppu_draw_text_background_obj;