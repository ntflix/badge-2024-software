// Include the header file to get access to the MicroPython API
#include "sasppu_background.h"
#include <string.h>

mp_obj_t sasppu_background_default(mp_obj_t self_in)
{
    sasppu_background_t *self = MP_OBJ_TO_PTR(self_in);
    memset(&self->dat, 0, sizeof(Background));
    self->dat.x = 0;
    self->dat.y = 0;
    self->dat.windows = 0xFF;
    self->dat.flags = 0;
    self->bound = -1;
    return mp_const_none;
}

static mp_obj_t sasppu_background_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in)
{
    sasppu_background_t *o = mp_obj_malloc(sasppu_background_t, type);

    sasppu_background_default(MP_OBJ_FROM_PTR(o));

    return MP_OBJ_FROM_PTR(o);
}

static mp_obj_t sasppu_background_binary_op(mp_binary_op_t op, mp_obj_t lhs_in, mp_obj_t rhs_in)
{
    sasppu_background_t *self = MP_OBJ_TO_PTR(lhs_in);
    if (self->bound == 0)
    {
        self->dat = SASPPU_bg0_state;
    }
    else if (self->bound == 1)
    {
        self->dat = SASPPU_bg1_state;
    }
    sasppu_background_t *other = MP_OBJ_TO_PTR(rhs_in);
    if (other->bound == 0)
    {
        other->dat = SASPPU_bg0_state;
    }
    else if (other->bound == 1)
    {
        other->dat = SASPPU_bg1_state;
    }
    switch (op)
    {
    case MP_BINARY_OP_EQUAL:
    {
        return memcmp(&self->dat, &other->dat, sizeof(Background)) ? mp_const_true : mp_const_false;
    }
    default:
        // op not supported
        return MP_OBJ_NULL;
    }
}

static void sasppu_background_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest)
{
    sasppu_background_t *self = MP_OBJ_TO_PTR(self_in);
    if (self->bound == 0)
    {
        self->dat = SASPPU_bg0_state;
    }
    else if (self->bound == 1)
    {
        self->dat = SASPPU_bg1_state;
    }
    if (dest[0] == MP_OBJ_NULL)
    {
        // Load attribute.
        switch (attr)
        {
        case MP_QSTR_x:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->dat.x);
            break;
        case MP_QSTR_y:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->dat.y);
            break;
        case MP_QSTR_windows:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->dat.windows);
            break;
        case MP_QSTR_window_1:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->dat.windows & 0x0F);
            break;
        case MP_QSTR_window_2:
            dest[0] = MP_OBJ_NEW_SMALL_INT((self->dat.windows >> 4) & 0x0F);
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
        case MP_QSTR_x:
            if ((val < -0x8000) || (val > 0x7FFF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Position value out of bounds"));
            }
            else
            {
                self->dat.x = val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_y:
            if ((val < -0x8000) || (val > 0x7FFF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Position value out of bounds"));
            }
            else
            {
                self->dat.y = val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_windows:
            if ((val < 0) || (val > 0xFF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Windows out of bounds"));
            }
            else
            {
                self->dat.windows = val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_window_1:
            if ((val < 0) || (val > 0xF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Window out of bounds"));
            }
            else
            {
                self->dat.windows &= 0xF0;
                self->dat.windows |= val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_window_2:
            if ((val < 0) || (val > 0xF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Window out of bounds"));
            }
            else
            {
                self->dat.windows &= 0x0F;
                self->dat.windows |= (val << 4);
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
    if (self->bound == 0)
    {
        SASPPU_bg0_state = self->dat;
    }
    else if (self->bound == 1)
    {
        SASPPU_bg1_state = self->dat;
    }
}

static mp_obj_t sasppu_background_unbind(mp_obj_t self_in)
{
    sasppu_background_t *self = MP_OBJ_TO_PTR(self_in);
    if (self->bound == 0)
    {
        self->dat = SASPPU_bg0_state;
    }
    else if (self->bound == 1)
    {
        self->dat = SASPPU_bg1_state;
    }
    self->bound = -1;
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(sasppu_background_unbind_obj, sasppu_background_unbind);

static mp_obj_t sasppu_background_bind(size_t n_args, const mp_obj_t *args)
{
    bool flush = true;
    if (n_args == 3)
    {
        flush = mp_obj_is_true(args[2]);
    }
    sasppu_background_t *self = MP_OBJ_TO_PTR(args[0]);
    mp_int_t bind_point = mp_obj_get_int(args[1]);
    if (bind_point < 0)
    {
        sasppu_background_unbind(args[0]);
        return mp_const_none;
    }
    if (bind_point > 1)
    {
        mp_raise_ValueError(MP_ERROR_TEXT("Bind point out of bounds"));
        return mp_const_none;
    }
    if (self->bound >= 0)
    {
        sasppu_background_unbind(args[0]);
    }
    self->bound = bind_point;
    if (flush)
    {
        if (bind_point == 0)
        {
            SASPPU_bg0_state = self->dat;
        }
        else if (bind_point == 1)
        {
            SASPPU_bg1_state = self->dat;
        }
    }
    else
    {
        if (bind_point == 0)
        {
            self->dat = SASPPU_bg0_state;
        }
        else if (bind_point == 1)
        {
            self->dat = SASPPU_bg1_state;
        }
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_background_bind_obj, 2, 3, sasppu_background_bind);

static mp_obj_t sasppu_background_get_bind_point(mp_obj_t self_in)
{
    sasppu_background_t *self = MP_OBJ_TO_PTR(self_in);
    if (self->bound < 0)
    {
        return mp_const_none;
    }
    return MP_OBJ_NEW_SMALL_INT(self->bound);
}
static MP_DEFINE_CONST_FUN_OBJ_1(sasppu_background_get_bind_point_obj, sasppu_background_get_bind_point);

static const mp_rom_map_elem_t sasppu_background_locals_dict_table[] = {
    {MP_ROM_QSTR(MP_QSTR_bind), MP_ROM_PTR(&sasppu_background_bind_obj)},
    {MP_ROM_QSTR(MP_QSTR_unbind), MP_ROM_PTR(&sasppu_background_unbind_obj)},
    {MP_ROM_QSTR(MP_QSTR_get_bind_point), MP_ROM_PTR(&sasppu_background_get_bind_point_obj)},

    {MP_ROM_QSTR(MP_QSTR_WIDTH_POWER), MP_ROM_INT(BG_WIDTH_POWER)},
    {MP_ROM_QSTR(MP_QSTR_HEIGHT_POWER), MP_ROM_INT(BG_HEIGHT_POWER)},
    {MP_ROM_QSTR(MP_QSTR_WIDTH), MP_ROM_INT(BG_WIDTH)},
    {MP_ROM_QSTR(MP_QSTR_HEIGHT), MP_ROM_INT(BG_HEIGHT)},
    {MP_ROM_QSTR(MP_QSTR_C_MATH), MP_ROM_INT(BG_C_MATH)},
};
static MP_DEFINE_CONST_DICT(sasppu_background_locals_dict, sasppu_background_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    sasppu_type_background,
    MP_QSTR_MainState,
    MP_TYPE_FLAG_NONE,
    make_new, sasppu_background_make_new,
    locals_dict, &sasppu_background_locals_dict,
    binary_op, sasppu_background_binary_op,
    attr, sasppu_background_attr);