#include "sasppu_map.h"
#include "subscr_load_adapter.h"

static mp_obj_t map_subscr(mp_obj_t self_in, mp_obj_t index_in, mp_obj_t value)
{
    sasppu_map_t *self = MP_OBJ_TO_PTR(self_in);
    mp_int_t index = mp_obj_get_int(index_in);
    if ((index < 0) || (index > (MAP_HEIGHT * MAP_WIDTH)))
    {
        mp_raise_msg(&mp_type_IndexError, MP_ERROR_TEXT("Index out of map bounds"));
        return mp_const_none;
    }

    if (value == MP_OBJ_NULL)
    {
        if (self->bg == 0) {
            SASPPU_bg0[index] = 0;
        } else if (self->bg == 1) {
            SASPPU_bg1[index] = 0;
        }
        return mp_const_none;
    }
    else if (value == MP_OBJ_SENTINEL)
    {
        if (self->bg == 0) {
            return MP_OBJ_NEW_SMALL_INT(SASPPU_bg0[index]);
        } else if (self->bg == 1) {
            return MP_OBJ_NEW_SMALL_INT(SASPPU_bg1[index]);
        }
        return MP_OBJ_NULL;
    }
    else
    {
        mp_int_t tile = mp_obj_get_int(value);
        if ((tile < 0) || (tile >  0xFFFF))
        {
            mp_raise_ValueError(MP_ERROR_TEXT("Invalid Background tile"));
            return mp_const_none;
        }
        if (self->bg == 0) {
            SASPPU_bg0[index] = tile;
        } else if (self->bg == 1) {
            SASPPU_bg1[index] = tile;
        }
        return mp_const_none;
    }

    return MP_OBJ_NULL;
}

static mp_obj_t map_unary_op(mp_unary_op_t op, mp_obj_t self_in) {
    switch (op) {
        case MP_UNARY_OP_LEN: {
            return MP_OBJ_NEW_SMALL_INT(MAP_HEIGHT * MAP_WIDTH);
        }
        default:
            return MP_OBJ_NULL; // op not supported
    }
}

MP_DEFINE_CONST_OBJ_TYPE(
    sasppu_type_map,
    MP_QSTR_MAP,
    MP_TYPE_FLAG_ITER_IS_GETITER,
    unary_op, map_unary_op,
    subscr, map_subscr,
    iter, subscr_getiter);