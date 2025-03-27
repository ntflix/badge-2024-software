// Include the header file to get access to the MicroPython API
#include "sasppu_tile.h"

mp_obj_t sasppu_tile_default(mp_obj_t self_in)
{
    sasppu_tile_t *self = MP_OBJ_TO_PTR(self_in);
    self->flip_x = mp_const_false;
    self->flip_y = mp_const_false;
    self->tile = 0;
    return mp_const_none;
}

mp_obj_t sasppu_tile_from_int(mp_obj_t self_in, mp_int_t int_in)
{
    sasppu_tile_t *self = MP_OBJ_TO_PTR(self_in);
    if (int_in < 0 || int_in > 0xFFFF)
    {
        mp_raise_ValueError(MP_ERROR_TEXT("Tile value out of bounds"));
        return MP_OBJ_NULL;
    }
    self->flip_x = int_in & 0b001 ? mp_const_true : mp_const_false;
    self->flip_y = int_in & 0b010 ? mp_const_true : mp_const_false;
    self->tile = int_in & !0x07;
    return mp_const_none;
}

mp_int_t sasppu_tile_to_int(mp_obj_t self_in)
{
    sasppu_tile_t *self = MP_OBJ_TO_PTR(self_in);
    return self->tile | (mp_obj_is_true(self->flip_x) ? 0b001 : 0) | (mp_obj_is_true(self->flip_y) ? 0b010 : 0);
}

static mp_obj_t sasppu_tile_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in)
{
    sasppu_tile_t *o = mp_obj_malloc(sasppu_tile_t, type);

    sasppu_tile_default(MP_OBJ_FROM_PTR(o));

    return MP_OBJ_FROM_PTR(o);
}

static mp_obj_t sasppu_tile_unary_op(mp_unary_op_t op, mp_obj_t self_in)
{
    switch (op)
    {
    // case MP_UNARY_OP_HASH:
    case MP_UNARY_OP_INT_MAYBE:
    {
        return MP_OBJ_NEW_SMALL_INT(sasppu_tile_to_int(self_in));
    }
        // case MP_UNARY_OP_SIZEOF:
        //{
        //     return MP_OBJ_NEW_SMALL_INT(sizeof(sasppu_tile_t));
        // }

    default:
        return MP_OBJ_NULL; // op not supported
    }
}

static mp_obj_t sasppu_tile_binary_op(mp_binary_op_t op, mp_obj_t lhs_in, mp_obj_t rhs_in)
{
    sasppu_tile_t *self = MP_OBJ_TO_PTR(lhs_in);
    sasppu_tile_t *other = MP_OBJ_TO_PTR(rhs_in);
    switch (op)
    {
    case MP_BINARY_OP_EQUAL:
    {
        bool same = true;
        same &= (self->tile == other->tile);
        same &= (self->flip_x == other->flip_x);
        same &= (self->flip_y == other->flip_y);
        return same ? mp_const_true : mp_const_false;
    }
    default:
        // op not supported
        return MP_OBJ_NULL;
    }
}

static void sasppu_tile_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest)
{
    sasppu_tile_t *self = MP_OBJ_TO_PTR(self_in);
    if (dest[0] == MP_OBJ_NULL)
    {
        // Load attribute.
        switch (attr)
        {
        case MP_QSTR_tile:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->tile);
            break;
        case MP_QSTR_flip_x:
            dest[0] = self->flip_x;
            break;
        case MP_QSTR_flip_y:
            dest[0] = self->flip_y;
            break;
        default:
            dest[1] = MP_OBJ_SENTINEL;
            break;
        }
    }
    else if (dest[1] != MP_OBJ_NULL)
    {
        // Store attribute.
        switch (attr)
        {
        case MP_QSTR_tile:
        {
            mp_int_t val = mp_obj_get_int(dest[1]);
            if ((val < 0) || (val > 0xFFFF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Tile index out of bounds"));
            }
            else if ((val & 0x7) > 0)
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Tile index must be multiple of 8"));
            }
            else
            {
                self->tile = val;
                dest[0] = MP_OBJ_NULL;
            }
        }
        break;
        case MP_QSTR_flip_x:
        {
            bool val = mp_obj_is_true(dest[1]);
            self->flip_x = val ? mp_const_true : mp_const_false;
            dest[0] = MP_OBJ_NULL;
        }
        break;
        case MP_QSTR_flip_y:
        {
            bool val = mp_obj_is_true(dest[1]);
            self->flip_y = val ? mp_const_true : mp_const_false;
            dest[0] = MP_OBJ_NULL;
        }
        break;
        default:
            break;
        }
    }
}

static const mp_rom_map_elem_t sasppu_tile_locals_dict_table[] = {
    {MP_ROM_QSTR(MP_QSTR_FLIP_X), MP_ROM_INT(0b001)},
    {MP_ROM_QSTR(MP_QSTR_FLIP_Y), MP_ROM_INT(0b010)},
};
static MP_DEFINE_CONST_DICT(sasppu_tile_locals_dict, sasppu_tile_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    sasppu_type_tile,
    MP_QSTR_Tile,
    MP_TYPE_FLAG_NONE,
    make_new, sasppu_tile_make_new,
    locals_dict, &sasppu_tile_locals_dict,
    unary_op, sasppu_tile_unary_op,
    binary_op, sasppu_tile_binary_op,
    attr, sasppu_tile_attr);