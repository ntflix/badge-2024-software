// Include the header file to get access to the MicroPython API
#include "sasppu_windows.h"

mp_obj_t sasppu_windows_default(mp_obj_t self_in)
{
    sasppu_windows_t *self = MP_OBJ_TO_PTR(self_in);
    self->window_1_state = WINDOW_ALL;
    self->window_2_state = WINDOW_ALL;
    return mp_const_none;
}

mp_obj_t sasppu_windows_from_int(mp_obj_t self_in, mp_int_t int_in)
{
    sasppu_windows_t *self = MP_OBJ_TO_PTR(self_in);
    if (int_in < 0 || int_in > 255)
    {
        mp_raise_ValueError(MP_ERROR_TEXT("Window value out of bounds"));
        return MP_OBJ_NULL;
    }
    self->window_1_state = int_in & 0x0F;
    self->window_2_state = (int_in >> 4) & 0x0F;
    return mp_const_none;
}

mp_int_t sasppu_windows_to_int(mp_obj_t obj_in)
{
    sasppu_windows_t *self = MP_OBJ_TO_PTR(obj_in);
    return self->window_1_state | (self->window_2_state << 4);
}

static mp_obj_t sasppu_windows_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in)
{
    sasppu_windows_t *self = mp_obj_malloc(sasppu_windows_t, type);

    if (n_args == 0)
    {
        sasppu_windows_default(MP_OBJ_FROM_PTR(self));
    }
    else if (n_args == 1)
    {
        mp_int_t int_in = mp_obj_get_int(args_in[0]);
        sasppu_windows_from_int(MP_OBJ_FROM_PTR(self), int_in);
    }
    else if (n_args == 2)
    {
        mp_int_t int_in_1 = mp_obj_get_int(args_in[0]);
        if (int_in_1 < 0 || int_in_1 > 0x0F)
        {
            mp_raise_ValueError(MP_ERROR_TEXT("Window 1 value out of bounds"));
            return MP_OBJ_NULL;
        }
        mp_int_t int_in_2 = mp_obj_get_int(args_in[1]);
        if (int_in_2 < 0 || int_in_2 > 0x0F)
        {
            mp_raise_ValueError(MP_ERROR_TEXT("Window 2 value out of bounds"));
            return MP_OBJ_NULL;
        }
        self->window_1_state = int_in_1 & 0x0F;
        self->window_2_state = int_in_2 & 0x0F;
    }

    return MP_OBJ_FROM_PTR(self);
}

static mp_obj_t sasppu_windows_subscr(mp_obj_t self_in, mp_obj_t index_in, mp_obj_t value)
{
    sasppu_windows_t *self = MP_OBJ_TO_PTR(self_in);
    mp_int_t index = mp_obj_get_int(index_in);
    if ((index < 0) || (index > 1))
    {
        mp_raise_type(&mp_type_IndexError);
        return MP_OBJ_NULL;
    }
    if (value == MP_OBJ_NULL)
    {
        // delete
        mp_raise_ValueError(MP_ERROR_TEXT("Cannot delete window"));
        return MP_OBJ_NULL;
    }
    else if (value == MP_OBJ_SENTINEL)
    {
        // load
        if (index == 0)
        {
            return MP_OBJ_NEW_SMALL_INT(self->window_1_state);
        }
        else
        {
            return MP_OBJ_NEW_SMALL_INT(self->window_2_state);
        }
    }
    else
    {
        // store
        mp_int_t val = mp_obj_get_int(value);
        if ((val < 0) || (val > 0x0F))
        {
            mp_raise_ValueError(MP_ERROR_TEXT("Window value out of bounds"));
            return MP_OBJ_NULL;
        }
        else
        {
            if (index == 0)
            {
                self->window_1_state = val;
            }
            else
            {
                self->window_2_state = val;
            }
        }
    }
    return mp_const_none;
}

static mp_obj_t sasppu_windows_create_from_int(mp_int_t int_in)
{
    if (int_in < 0 || int_in > 255)
    {
        mp_raise_ValueError(MP_ERROR_TEXT("Window value out of bounds"));
        return MP_OBJ_NULL;
    }
    sasppu_windows_t *self = MP_OBJ_TO_PTR(sasppu_windows_make_new(&sasppu_type_windows, 0, 0, NULL));
    self->window_1_state = int_in & 0x0F;
    self->window_2_state = (int_in >> 4) & 0x0F;
    return MP_OBJ_FROM_PTR(self);
}

static mp_obj_t sasppu_windows_unary_op(mp_unary_op_t op, mp_obj_t self_in)
{
    sasppu_windows_t *self = MP_OBJ_TO_PTR(self_in);
    switch (op)
    {
    case MP_UNARY_OP_INVERT:
    {
        sasppu_windows_t *other = MP_OBJ_TO_PTR(sasppu_windows_make_new(&sasppu_type_windows, 0, 0, NULL));
        other->window_1_state = (!self->window_1_state) & 0x0F;
        other->window_2_state = (!self->window_2_state) & 0x0F;
        return MP_OBJ_FROM_PTR(self);
    }
    case MP_UNARY_OP_BOOL:
    {
        bool non_zero = (self->window_1_state != 0) || (self->window_2_state != 0);
        return non_zero ? mp_const_true : mp_const_false;
    }
    // case MP_UNARY_OP_HASH:
    case MP_UNARY_OP_INT_MAYBE:
    {
        return MP_OBJ_NEW_SMALL_INT(sasppu_windows_to_int(self_in));
    }
        // case MP_UNARY_OP_SIZEOF:
        //{
        //     return MP_OBJ_NEW_SMALL_INT(sizeof(sasppu_windows_t));
        // }

    default:
        return MP_OBJ_NULL; // op not supported
    }
}

static mp_obj_t sasppu_windows_binary_op(mp_binary_op_t op, mp_obj_t lhs_in, mp_obj_t rhs_in)
{
    sasppu_windows_t *self = MP_OBJ_TO_PTR(lhs_in);
    mp_int_t other_int = mp_obj_get_int(rhs_in);
    sasppu_windows_t *other = MP_OBJ_TO_PTR(sasppu_windows_create_from_int(other_int));
    switch (op)
    {
    case MP_BINARY_OP_EQUAL:
    {
        bool same = true;
        same &= (self->window_1_state == other->window_1_state);
        same &= (self->window_2_state == other->window_2_state);
        return same ? mp_const_true : mp_const_false;
    }

    case MP_BINARY_OP_INPLACE_OR:
    {
        self->window_1_state |= other->window_1_state;
        self->window_2_state |= other->window_2_state;
        return lhs_in;
    }
    case MP_BINARY_OP_INPLACE_XOR:
    {
        self->window_1_state ^= other->window_1_state;
        self->window_2_state ^= other->window_2_state;
        return lhs_in;
    }
    case MP_BINARY_OP_INPLACE_AND:
    {
        self->window_1_state &= other->window_1_state;
        self->window_2_state &= other->window_2_state;
        return lhs_in;
    }

    case MP_BINARY_OP_OR:
    {
        other->window_1_state |= self->window_1_state;
        other->window_2_state |= self->window_2_state;
        return MP_OBJ_FROM_PTR(other);
    }
    case MP_BINARY_OP_XOR:
    {
        other->window_1_state ^= self->window_1_state;
        other->window_2_state ^= self->window_2_state;
        return MP_OBJ_FROM_PTR(other);
    }
    case MP_BINARY_OP_AND:
    {
        other->window_1_state &= self->window_1_state;
        other->window_2_state &= self->window_2_state;
        return MP_OBJ_FROM_PTR(other);
    }

    default:
        // op not supported
        return MP_OBJ_NULL;
    }
}

static void sasppu_windows_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind)
{
    sasppu_windows_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "Windows(%u, %u)", self->window_1_state, self->window_2_state);
}

static void sasppu_windows_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest)
{
    sasppu_windows_t *self = MP_OBJ_TO_PTR(self_in);
    if (dest[0] == MP_OBJ_NULL)
    {
        // Load attribute.
        if (attr == MP_QSTR_window_1_state)
        {
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->window_1_state);
        }
        else if (attr == MP_QSTR_window_2_state)
        {
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->window_2_state);
        }
        else
        {
            // Continue lookup in locals_dict.
            dest[1] = MP_OBJ_SENTINEL;
        }
    }
    else if (dest[1] != MP_OBJ_NULL)
    {
        // Store attribute.
        mp_int_t val = mp_obj_get_int(dest[1]);
        if ((val < 0) || (val > 0x0F))
        {
            mp_raise_ValueError(MP_ERROR_TEXT("Window value out of bounds"));
        }
        else if (attr == MP_QSTR_window_1_state)
        {
            self->window_1_state = val;
            dest[0] = MP_OBJ_NULL;
        }
        else if (attr == MP_QSTR_window_2_state)
        {
            self->window_2_state = val;
            dest[0] = MP_OBJ_NULL;
        }
    }
}

static const mp_rom_map_elem_t sasppu_windows_locals_dict_table[] = {
    {MP_ROM_QSTR(MP_QSTR_A), MP_ROM_INT(WINDOW_A)},
    {MP_ROM_QSTR(MP_QSTR_B), MP_ROM_INT(WINDOW_B)},
    {MP_ROM_QSTR(MP_QSTR_AB), MP_ROM_INT(WINDOW_AB)},
    {MP_ROM_QSTR(MP_QSTR_X), MP_ROM_INT(WINDOW_X)},
    {MP_ROM_QSTR(MP_QSTR_ALL), MP_ROM_INT(WINDOW_ALL)},
};
static MP_DEFINE_CONST_DICT(sasppu_windows_locals_dict, sasppu_windows_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    sasppu_type_windows,
    MP_QSTR_Windows,
    MP_TYPE_FLAG_NONE,
    make_new, sasppu_windows_make_new,
    locals_dict, &sasppu_windows_locals_dict,
    subscr, sasppu_windows_subscr,
    unary_op, sasppu_windows_unary_op,
    binary_op, sasppu_windows_binary_op,
    print, sasppu_windows_print,
    attr, sasppu_windows_attr);