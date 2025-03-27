// Include the header file to get access to the MicroPython API
#include "sasppu_main_state.h"

mp_obj_t sasppu_main_state_default(mp_obj_t self_in)
{
    sasppu_main_state_t *self = MP_OBJ_TO_PTR(self_in);
    sasppu_windows_default(self->bgcol_windows);
    self->window_1_left = 0;
    self->window_2_left = 0;
    self->window_1_right = 255;
    self->window_2_right = 255;
    self->mainscreen_colour = SASPPU_TRANSPARENT_BLACK;
    self->subscreen_colour = SASPPU_TRANSPARENT_BLACK;
    self->flags = 0;
    return mp_const_none;
}

mp_obj_t sasppu_main_state_from_struct(mp_obj_t self_in, MainState ms)
{
    sasppu_main_state_t *self = MP_OBJ_TO_PTR(self_in);
    sasppu_windows_from_int(self->bgcol_windows, ms.bgcol_windows);
    self->window_1_left = ms.window_1_left;
    self->window_2_left = ms.window_2_left;
    self->window_1_right = ms.window_1_right;
    self->window_2_right = ms.window_2_right;
    self->mainscreen_colour = ms.mainscreen_colour;
    self->subscreen_colour = ms.subscreen_colour;
    self->flags = ms.flags;
    return mp_const_none;
}

MainState sasppu_main_state_to_struct(mp_obj_t self_in)
{
    sasppu_main_state_t *self = MP_OBJ_TO_PTR(self_in);
    MainState ms;
    ms.bgcol_windows = sasppu_windows_to_int(self->bgcol_windows);
    ms.window_1_left = self->window_1_left;
    ms.window_2_left = self->window_2_left;
    ms.window_1_right = self->window_1_right;
    ms.window_2_right = self->window_2_right;
    ms.mainscreen_colour = self->mainscreen_colour;
    ms.subscreen_colour = self->subscreen_colour;
    ms.flags = self->flags;
    return ms;
}

static mp_obj_t sasppu_main_state_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in)
{
    sasppu_main_state_t *o = mp_obj_malloc(sasppu_main_state_t, type);

    o->bgcol_windows = mp_obj_malloc(sasppu_windows_t, &sasppu_type_windows);

    sasppu_main_state_default(MP_OBJ_FROM_PTR(o));

    return MP_OBJ_FROM_PTR(o);
}

static mp_obj_t sasppu_main_state_binary_op(mp_binary_op_t op, mp_obj_t lhs_in, mp_obj_t rhs_in)
{
    sasppu_main_state_t *self = MP_OBJ_TO_PTR(lhs_in);
    sasppu_main_state_t *other = MP_OBJ_TO_PTR(rhs_in);
    switch (op)
    {
    case MP_BINARY_OP_EQUAL:
    {
        bool same = true;
        same &= (self->mainscreen_colour == other->mainscreen_colour);
        same &= (self->subscreen_colour == other->subscreen_colour);
        same &= (self->window_1_left == other->window_1_left);
        same &= (self->window_2_left == other->window_2_left);
        same &= (self->window_1_right == other->window_1_right);
        same &= (self->window_2_right == other->window_2_right);
        same &= (self->flags == other->flags);
        same &= (mp_obj_equal(self->bgcol_windows, other->bgcol_windows));
        return same ? mp_const_true : mp_const_false;
    }
    default:
        // op not supported
        return MP_OBJ_NULL;
    }
}

static void sasppu_main_state_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest)
{
    sasppu_main_state_t *self = MP_OBJ_TO_PTR(self_in);
    if (dest[0] == MP_OBJ_NULL)
    {
        // Load attribute.
        switch (attr)
        {
        case MP_QSTR_mainscreen_colour:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->mainscreen_colour);
            break;
        case MP_QSTR_subscreen_colour:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->subscreen_colour);
            break;
        case MP_QSTR_window_1_left:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->window_1_left);
            break;
        case MP_QSTR_window_1_right:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->window_1_right);
            break;
        case MP_QSTR_window_2_left:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->window_2_left);
            break;
        case MP_QSTR_window_2_right:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->window_2_right);
            break;
        case MP_QSTR_bgcol_windows:
            dest[0] = self->bgcol_windows;
            break;
        case MP_QSTR_flags:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->flags);
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
                self->mainscreen_colour = val;
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
                self->subscreen_colour = val;
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
                self->window_1_left = val;
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
                self->window_1_right = val;
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
                self->window_2_left = val;
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
                self->window_2_right = val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_bgcol_windows:
            sasppu_windows_from_int(self->bgcol_windows, val);
            dest[0] = MP_OBJ_NULL;
            break;
        case MP_QSTR_flags:
            if ((val < 0) || (val > 0xFF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Flags out of bounds"));
            }
            else
            {
                self->flags = val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        default:
            break;
        }
    }
}

static const mp_rom_map_elem_t sasppu_main_state_locals_dict_table[] = {
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
    MP_QSTR_Mainstate,
    MP_TYPE_FLAG_NONE,
    make_new, sasppu_main_state_make_new,
    locals_dict, &sasppu_main_state_locals_dict,
    binary_op, sasppu_main_state_binary_op,
    attr, sasppu_main_state_attr);