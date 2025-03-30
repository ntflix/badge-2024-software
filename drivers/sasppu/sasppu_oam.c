#include "sasppu_oam.h"
#include "sasppu_sprite.h"
#include "subscr_load_adapter.h"

static mp_obj_t oam_subscr(mp_obj_t self_in, mp_obj_t index_in, mp_obj_t value)
{
    // sasppu_oam_t *self = MP_OBJ_TO_PTR(self_in);
    mp_int_t index = mp_obj_get_int(index_in);
    if ((index < 0) || (index > SPRITE_COUNT))
    {
        mp_raise_msg(&mp_type_IndexError, MP_ERROR_TEXT("Index out of OAM bounds"));
        return mp_const_none;
    }

    if (value == MP_OBJ_NULL)
    {
        Sprite *spr = &SASPPU_oam[index];
        spr->x = 0;
        spr->y = 0;
        spr->width = 32;
        spr->height = 32;
        spr->graphics_x = 0;
        spr->graphics_y = 0;
        spr->flags = 0;
        spr->windows = 0xFF;
        return mp_const_none;
    }
    else if (value == MP_OBJ_SENTINEL)
    {
        sasppu_sprite_t *spr = mp_obj_malloc(sasppu_sprite_t, &sasppu_type_sprite);
        spr->bound = index;
        spr->dat = SASPPU_oam[index];
        return MP_OBJ_FROM_PTR(spr);
    }
    else
    {
        sasppu_sprite_t *spr = MP_OBJ_TO_PTR(value);
        SASPPU_oam[index] = spr->dat;
        return mp_const_none;
    }

    return MP_OBJ_NULL;
}

static mp_obj_t oam_unary_op(mp_unary_op_t op, mp_obj_t self_in)
{
    switch (op)
    {
    case MP_UNARY_OP_LEN:
    {
        return MP_OBJ_NEW_SMALL_INT(SPRITE_COUNT);
    }
    default:
        return MP_OBJ_NULL; // op not supported
    }
}

MP_DEFINE_CONST_OBJ_TYPE(
    sasppu_type_oam,
    MP_QSTR_OAM,
    MP_TYPE_FLAG_ITER_IS_GETITER,
    unary_op, oam_unary_op,
    subscr, oam_subscr,
    iter, subscr_getiter);