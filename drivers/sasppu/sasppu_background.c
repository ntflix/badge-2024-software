// Include the header file to get access to the MicroPython API
#include "sasppu_background.h"

mp_obj_t sasppu_background_default(mp_obj_t self_in)
{
    sasppu_background_t *self = MP_OBJ_TO_PTR(self_in);
    sasppu_windows_default(self->windows);
    self->x = 0;
    self->y = 0;
    self->flags = 0;
    return mp_const_none;
}

mp_obj_t sasppu_background_from_struct(mp_obj_t self_in, Background bg)
{
    sasppu_background_t *self = MP_OBJ_TO_PTR(self_in);
    sasppu_windows_from_int(self->windows, bg.windows);
    self->x = bg.x;
    self->y = bg.y;
    self->flags = bg.flags;
    return mp_const_none;
}

Background sasppu_background_to_struct(mp_obj_t self_in)
{
    sasppu_background_t *self = MP_OBJ_TO_PTR(self_in);
    Background bg;
    bg.windows = sasppu_windows_to_int(self->windows);
    bg.x = self->x;
    bg.y = self->y;
    bg.flags = self->flags;
    return bg;
}

static mp_obj_t sasppu_background_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in)
{
    sasppu_background_t *o = mp_obj_malloc(sasppu_background_t, type);

    o->windows = mp_obj_malloc(sasppu_windows_t, &sasppu_type_windows);

    sasppu_background_default(MP_OBJ_FROM_PTR(o));

    return MP_OBJ_FROM_PTR(o);
}

static mp_obj_t sasppu_background_binary_op(mp_binary_op_t op, mp_obj_t lhs_in, mp_obj_t rhs_in)
{
    sasppu_background_t *self = MP_OBJ_TO_PTR(lhs_in);
    sasppu_background_t *other = MP_OBJ_TO_PTR(rhs_in);
    switch (op)
    {
    case MP_BINARY_OP_EQUAL:
    {
        bool same = true;
        same &= (self->x == other->x);
        same &= (self->y == other->y);
        same &= (self->flags == other->flags);
        same &= (mp_obj_equal(self->windows, other->windows));
        return same ? mp_const_true : mp_const_false;
    }
    default:
        // op not supported
        return MP_OBJ_NULL;
    }
}

static void sasppu_background_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest)
{
    sasppu_background_t *self = MP_OBJ_TO_PTR(self_in);
    if (dest[0] == MP_OBJ_NULL)
    {
        // Load attribute.
        switch (attr)
        {
        case MP_QSTR_x:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->x);
            break;
        case MP_QSTR_y:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->y);
            break;
        case MP_QSTR_windows:
            dest[0] = self->windows;
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
        case MP_QSTR_x:
            if ((val < -0x8000) || (val > 0x7FFF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Position value out of bounds"));
            }
            else
            {
                self->x = val;
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
                self->y = val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_windows:
            sasppu_windows_from_int(self->windows, val);
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

static const mp_rom_map_elem_t sasppu_background_locals_dict_table[] = {
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