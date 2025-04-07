// Include the header file to get access to the MicroPython API
#include "sasppu_cmath_state.h"
#include <string.h>

mp_obj_t sasppu_cmath_state_default(mp_obj_t self_in)
{
    sasppu_cmath_state_t *self = MP_OBJ_TO_PTR(self_in);
    memset(&self->dat, 0, sizeof(CMathState));
    self->dat.screen_fade = 255;
    self->dat.flags = 0;
    self->bound = false;
    return mp_const_none;
}

static mp_obj_t sasppu_cmath_state_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in)
{
    sasppu_cmath_state_t *o = mp_obj_malloc(sasppu_cmath_state_t, type);

    sasppu_cmath_state_default(MP_OBJ_FROM_PTR(o));

    return MP_OBJ_FROM_PTR(o);
}

static mp_obj_t sasppu_cmath_state_binary_op(mp_binary_op_t op, mp_obj_t lhs_in, mp_obj_t rhs_in)
{
    sasppu_cmath_state_t *self = MP_OBJ_TO_PTR(lhs_in);
    if (self->bound)
    {
        self->dat = SASPPU_cmath_state;
    }
    sasppu_cmath_state_t *other = MP_OBJ_TO_PTR(rhs_in);
    if (other->bound)
    {
        other->dat = SASPPU_cmath_state;
    }
    switch (op)
    {
    case MP_BINARY_OP_EQUAL:
    {
        return memcmp(&self->dat, &other->dat, sizeof(CMathState)) ? mp_const_true : mp_const_false;
    }
    default:
        // op not supported
        return MP_OBJ_NULL;
    }
}

static void sasppu_cmath_state_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest)
{
    sasppu_cmath_state_t *self = MP_OBJ_TO_PTR(self_in);
    if (self->bound)
    {
        self->dat = SASPPU_cmath_state;
    }
    if (dest[0] == MP_OBJ_NULL)
    {
        // Load attribute.
        switch (attr)
        {
        case MP_QSTR_fade:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->dat.screen_fade);
            break;
        case MP_QSTR_flags:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->dat.flags);
            break;
        default:
            dest[1] = MP_OBJ_SENTINEL;
            break;
        }
    }
    else if (dest[1] != MP_OBJ_NULL)
    {
        // Store attribute.
        mp_int_t val = mp_obj_get_int(dest[1]);
        switch (attr)
        {
        case MP_QSTR_fade:
            if ((val < 0) || (val > 0xFF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Fade out of bounds"));
            }
            else
            {
                self->dat.screen_fade = val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_flags:
            if ((val < 0) || (val > 0xFF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Flags out of bounds"));
            }
            else
            {
                self->dat.flags = val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        default:
            break;
        }
    }
    if (self->bound)
    {
        SASPPU_cmath_state =self->dat;
    }
}

static mp_obj_t sasppu_cmath_state_unbind(mp_obj_t self_in)
{
    sasppu_cmath_state_t *self = MP_OBJ_TO_PTR(self_in);
    if (self->bound)
    {
        self->dat = SASPPU_cmath_state;
    }
    self->bound = false;
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(sasppu_cmath_state_unbind_obj, sasppu_cmath_state_unbind);

static mp_obj_t sasppu_cmath_state_bind(size_t n_args, const mp_obj_t *args)
{
    bool flush = true;
    if (n_args == 2)
    {
        flush = mp_obj_is_true(args[1]);
    }
    sasppu_cmath_state_t *self = MP_OBJ_TO_PTR(args[0]);
    if (self->bound)
    {
        sasppu_cmath_state_unbind(args[0]);
    }
    self->bound = true;
    if (flush)
    {
        SASPPU_cmath_state = self->dat;
    }
    else
    {
        self->dat = SASPPU_cmath_state;
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_cmath_state_bind_obj, 1, 2, sasppu_cmath_state_bind);

static mp_obj_t sasppu_cmath_state_get_bind_point(mp_obj_t self_in)
{
    sasppu_cmath_state_t *self = MP_OBJ_TO_PTR(self_in);
    return self->bound ? mp_const_true : mp_const_false;
}
static MP_DEFINE_CONST_FUN_OBJ_1(sasppu_cmath_state_get_bind_point_obj, sasppu_cmath_state_get_bind_point);

static const mp_rom_map_elem_t sasppu_cmath_state_locals_dict_table[] = {
    {MP_ROM_QSTR(MP_QSTR_bind), MP_ROM_PTR(&sasppu_cmath_state_bind_obj)},
    {MP_ROM_QSTR(MP_QSTR_unbind), MP_ROM_PTR(&sasppu_cmath_state_unbind_obj)},
    {MP_ROM_QSTR(MP_QSTR_get_bind_point), MP_ROM_PTR(&sasppu_cmath_state_get_bind_point_obj)},

    {MP_ROM_QSTR(MP_QSTR_HALF_MAIN_SCREEN), MP_ROM_INT(CMATH_HALF_MAIN_SCREEN)},
    {MP_ROM_QSTR(MP_QSTR_DOUBLE_MAIN_SCREEN), MP_ROM_INT(CMATH_DOUBLE_MAIN_SCREEN)},
    {MP_ROM_QSTR(MP_QSTR_HALF_SUB_SCREEN), MP_ROM_INT(CMATH_HALF_SUB_SCREEN)},
    {MP_ROM_QSTR(MP_QSTR_DOUBLE_SUB_SCREEN), MP_ROM_INT(CMATH_DOUBLE_SUB_SCREEN)},
    {MP_ROM_QSTR(MP_QSTR_ADD_SUB_SCREEN), MP_ROM_INT(CMATH_ADD_SUB_SCREEN)},
    {MP_ROM_QSTR(MP_QSTR_SUB_SUB_SCREEN), MP_ROM_INT(CMATH_SUB_SUB_SCREEN)},
    {MP_ROM_QSTR(MP_QSTR_FADE_ENABLE), MP_ROM_INT(CMATH_FADE_ENABLE)},
    {MP_ROM_QSTR(MP_QSTR_CMATH_ENABLE), MP_ROM_INT(CMATH_CMATH_ENABLE)},
};
static MP_DEFINE_CONST_DICT(sasppu_cmath_state_locals_dict, sasppu_cmath_state_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    sasppu_type_cmath_state,
    MP_QSTR_CMathState,
    MP_TYPE_FLAG_NONE,
    make_new, sasppu_cmath_state_make_new,
    locals_dict, &sasppu_cmath_state_locals_dict,
    binary_op, sasppu_cmath_state_binary_op,
    attr, sasppu_cmath_state_attr);