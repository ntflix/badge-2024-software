// Include the header file to get access to the MicroPython API
#include "sasppu_main_state.h"
#include <string.h>

mp_obj_t sasppu_main_state_default(mp_obj_t self_in)
{
    sasppu_main_state_t *self = MP_OBJ_TO_PTR(self_in);
    memset(&self->dat, 0, sizeof(MainState));
    self->dat.window_1_left = 0;
    self->dat.window_2_left = 0;
    self->dat.window_1_right = 255;
    self->dat.window_2_right = 255;
    self->dat.mainscreen_colour = SASPPU_TRANSPARENT_BLACK;
    self->dat.subscreen_colour = SASPPU_TRANSPARENT_BLACK;
    self->dat.bgcol_windows = 0xFF;
    self->dat.flags = 0;
    self->bound = false;
    return mp_const_none;
}

static mp_obj_t sasppu_main_state_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in)
{
    sasppu_main_state_t *o = mp_obj_malloc(sasppu_main_state_t, type);

    sasppu_main_state_default(MP_OBJ_FROM_PTR(o));

    return MP_OBJ_FROM_PTR(o);
}

static mp_obj_t sasppu_main_state_binary_op(mp_binary_op_t op, mp_obj_t lhs_in, mp_obj_t rhs_in)
{
    sasppu_main_state_t *self = MP_OBJ_TO_PTR(lhs_in);
    if (self->bound)
    {
        self->dat = SASPPU_main_state;
    }
    sasppu_main_state_t *other = MP_OBJ_TO_PTR(rhs_in);
    if (other->bound)
    {
        other->dat = SASPPU_main_state;
    }
    switch (op)
    {
    case MP_BINARY_OP_EQUAL:
    {
        return memcmp(&self->dat, &other->dat, sizeof(MainState)) ? mp_const_true : mp_const_false;
    }
    default:
        // op not supported
        return MP_OBJ_NULL;
    }
}

static void sasppu_main_state_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest)
{
    sasppu_main_state_t *self = MP_OBJ_TO_PTR(self_in);
    if (self->bound)
    {
        self->dat = SASPPU_main_state;
    }
    if (dest[0] == MP_OBJ_NULL)
    {
        // Load attribute.
        switch (attr)
        {
        case MP_QSTR_mainscreen_colour:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->dat.mainscreen_colour);
            break;
        case MP_QSTR_subscreen_colour:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->dat.subscreen_colour);
            break;
        case MP_QSTR_window_1_left:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->dat.window_1_left);
            break;
        case MP_QSTR_window_1_right:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->dat.window_1_right);
            break;
        case MP_QSTR_window_2_left:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->dat.window_2_left);
            break;
        case MP_QSTR_window_2_right:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->dat.window_2_right);
            break;
        case MP_QSTR_bgcol_windows:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->dat.bgcol_windows);
            break;
        case MP_QSTR_bgcol_window_1:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->dat.bgcol_windows & 0x0F);
            break;
        case MP_QSTR_bgcol_window_2:
            dest[0] = MP_OBJ_NEW_SMALL_INT((self->dat.bgcol_windows >> 4) & 0x0F);
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
        case MP_QSTR_mainscreen_colour:
            if ((val < 0) || (val > 0xFFFF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Colour value out of bounds"));
            }
            else
            {
                self->dat.mainscreen_colour = val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_subscreen_colour:
            if ((val < 0) || (val > 0xFFFF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Colour value out of bounds"));
            }
            else
            {
                self->dat.subscreen_colour = val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_window_1_left:
            if ((val < 0) || (val > 0xFF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Window bound out of bounds"));
            }
            else
            {
                self->dat.window_1_left = val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_window_1_right:
            if ((val < 0) || (val > 0xFF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Window bound out of bounds"));
            }
            else
            {
                self->dat.window_1_right = val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_window_2_left:
            if ((val < 0) || (val > 0xFF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Window bound out of bounds"));
            }
            else
            {
                self->dat.window_2_left = val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_window_2_right:
            if ((val < 0) || (val > 0xFF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Window bound out of bounds"));
            }
            else
            {
                self->dat.window_2_right = val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_bgcol_windows:
            if ((val < 0) || (val > 0xFF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Windows out of bounds"));
            }
            else
            {
                self->dat.bgcol_windows = val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_bgcol_window_1:
            if ((val < 0) || (val > 0xF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Window out of bounds"));
            }
            else
            {
                self->dat.bgcol_windows &= 0xF0;
                self->dat.bgcol_windows |= val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_bgcol_window_2:
            if ((val < 0) || (val > 0xF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Window out of bounds"));
            }
            else
            {
                self->dat.bgcol_windows &= 0x0F;
                self->dat.bgcol_windows |= (val << 4);
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
        SASPPU_main_state = self->dat;
    }
}

static mp_obj_t sasppu_main_state_unbind(mp_obj_t self_in)
{
    sasppu_main_state_t *self = MP_OBJ_TO_PTR(self_in);
    if (self->bound)
    {
        SASPPU_main_state = self->dat;
    }
    self->bound = false;
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(sasppu_main_state_unbind_obj, sasppu_main_state_unbind);

static mp_obj_t sasppu_main_state_bind(size_t n_args, const mp_obj_t *args)
{
    bool flush = true;
    if (n_args == 2)
    {
        flush = mp_obj_is_true(args[1]);
    }
    sasppu_main_state_t *self = MP_OBJ_TO_PTR(args[0]);
    if (self->bound)
    {
        sasppu_main_state_unbind(args[0]);
    }
    self->bound = true;
    if (flush)
    {
        SASPPU_main_state = self->dat;
    }
    else
    {
        self->dat = SASPPU_main_state;
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_main_state_bind_obj, 1, 2, sasppu_main_state_bind);

static mp_obj_t sasppu_main_state_get_bind_point(mp_obj_t self_in)
{
    sasppu_main_state_t *self = MP_OBJ_TO_PTR(self_in);
    return self->bound ? mp_const_true : mp_const_false;
}
static MP_DEFINE_CONST_FUN_OBJ_1(sasppu_main_state_get_bind_point_obj, sasppu_main_state_get_bind_point);

static const mp_rom_map_elem_t sasppu_main_state_locals_dict_table[] = {
    {MP_ROM_QSTR(MP_QSTR_bind), MP_ROM_PTR(&sasppu_main_state_bind_obj)},
    {MP_ROM_QSTR(MP_QSTR_unbind), MP_ROM_PTR(&sasppu_main_state_unbind_obj)},
    {MP_ROM_QSTR(MP_QSTR_get_bind_point), MP_ROM_PTR(&sasppu_main_state_get_bind_point_obj)},

    {MP_ROM_QSTR(MP_QSTR_SPR0_ENABLE), MP_ROM_INT(MAIN_SPR0_ENABLE)},
    {MP_ROM_QSTR(MP_QSTR_SPR1_ENABLE), MP_ROM_INT(MAIN_SPR1_ENABLE)},
    {MP_ROM_QSTR(MP_QSTR_BG0_ENABLE), MP_ROM_INT(MAIN_BG0_ENABLE)},
    {MP_ROM_QSTR(MP_QSTR_BG1_ENABLE), MP_ROM_INT(MAIN_BG1_ENABLE)},
    {MP_ROM_QSTR(MP_QSTR_CMATH_ENABLE), MP_ROM_INT(MAIN_CMATH_ENABLE)},
    {MP_ROM_QSTR(MP_QSTR_BGCOL_WINDOW_ENABLE), MP_ROM_INT(MAIN_BGCOL_WINDOW_ENABLE)},
};
static MP_DEFINE_CONST_DICT(sasppu_main_state_locals_dict, sasppu_main_state_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    sasppu_type_main_state,
    MP_QSTR_MainState,
    MP_TYPE_FLAG_NONE,
    make_new, sasppu_main_state_make_new,
    locals_dict, &sasppu_main_state_locals_dict,
    binary_op, sasppu_main_state_binary_op,
    attr, sasppu_main_state_attr);