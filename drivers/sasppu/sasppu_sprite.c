// Include the header file to get access to the MicroPython API
#include "sasppu_sprite.h"
#include <string.h>

mp_obj_t sasppu_sprite_default(mp_obj_t self_in)
{
    sasppu_sprite_t *self = MP_OBJ_TO_PTR(self_in);
    memset(&self->dat, 0, sizeof(Sprite));
    self->dat.x = 0;
    self->dat.y = 0;
    self->dat.width = 32;
    self->dat.height = 32;
    self->dat.graphics_x = 0;
    self->dat.graphics_y = 0;
    self->dat.flags = 0;
    self->dat.windows = 0xFF;
    self->bound = -1;
    return mp_const_none;
}

static mp_obj_t sasppu_sprite_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in)
{
    sasppu_sprite_t *o = mp_obj_malloc(sasppu_sprite_t, type);

    sasppu_sprite_default(MP_OBJ_FROM_PTR(o));

    return MP_OBJ_FROM_PTR(o);
}

static mp_obj_t sasppu_sprite_binary_op(mp_binary_op_t op, mp_obj_t lhs_in, mp_obj_t rhs_in)
{
    sasppu_sprite_t *self = MP_OBJ_TO_PTR(lhs_in);
    if (self->bound >= 0)
    {
        self->dat = SASPPU_oam[self->bound];
    }
    sasppu_sprite_t *other = MP_OBJ_TO_PTR(rhs_in);
    if (other->bound >= 0)
    {
        other->dat = SASPPU_oam[other->bound];
    }
    switch (op)
    {
    case MP_BINARY_OP_EQUAL:
    {
        return memcmp(&self->dat, &other->dat, sizeof(Sprite)) ? mp_const_true : mp_const_false;
    }
    default:
        // op not supported
        return MP_OBJ_NULL;
    }
}

static void sasppu_sprite_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest)
{
    sasppu_sprite_t *self = MP_OBJ_TO_PTR(self_in);
    if (self->bound >= 0)
    {
        self->dat = SASPPU_oam[self->bound];
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
        case MP_QSTR_width:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->dat.width);
            break;
        case MP_QSTR_height:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->dat.height);
            break;
        case MP_QSTR_graphics_x:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->dat.graphics_x);
            break;
        case MP_QSTR_graphics_y:
            dest[0] = MP_OBJ_NEW_SMALL_INT(self->dat.graphics_y);
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
        case MP_QSTR_width:
            if ((val < 0) || (val > 0xFF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Dimension out of bounds"));
            }
            else if ((val & 0x7) > 0)
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Width must be multiple of 8"));
            }
            else
            {
                self->dat.width = val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_height:
            if ((val < 0) || (val > 0xFF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Dimension out of bounds"));
            }
            else
            {
                self->dat.height = val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_graphics_x:
            if ((val < 0) || (val > 0xFF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Graphics position out of bounds"));
            }
            else if ((val & 0x7) > 0)
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Graphics x position must be multiple of 8"));
            }
            else
            {
                self->dat.graphics_x = val;
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_graphics_y:
            if ((val < 0) || (val > 0xFF))
            {
                mp_raise_ValueError(MP_ERROR_TEXT("Graphics position out of bounds"));
            }
            else
            {
                self->dat.graphics_y = val;
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
                self->dat.windows |= val << 4;
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
    if (self->bound >= 0)
    {
        SASPPU_oam[self->bound] = self->dat;
    }
}

static mp_obj_t sasppu_sprite_unbind(mp_obj_t self_in)
{
    sasppu_sprite_t *self = MP_OBJ_TO_PTR(self_in);
    if (self->bound >= 0)
    {
        self->dat = SASPPU_oam[self->bound];
    }
    self->bound = -1;
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(sasppu_sprite_unbind_obj, sasppu_sprite_unbind);

static mp_obj_t sasppu_sprite_bind(size_t n_args, const mp_obj_t *args)
{
    bool flush = true;
    if (n_args == 3)
    {
        flush = mp_obj_is_true(args[2]);
    }
    sasppu_sprite_t *self = MP_OBJ_TO_PTR(args[0]);
    mp_int_t bind_point = mp_obj_get_int(args[1]);
    if (bind_point < 0)
    {
        sasppu_sprite_unbind(args[0]);
        return mp_const_none;
    }
    if (bind_point > 0xFF)
    {
        mp_raise_ValueError(MP_ERROR_TEXT("Bind point out of bounds"));
        return mp_const_none;
    }
    if (self->bound >= 0)
    {
        sasppu_sprite_unbind(args[0]);
    }
    self->bound = bind_point;
    if (flush)
    {
        SASPPU_oam[bind_point] = self->dat;
    }
    else
    {
        self->dat = SASPPU_oam[bind_point];
    }
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_sprite_bind_obj, 2, 3, sasppu_sprite_bind);

static mp_obj_t sasppu_sprite_get_bind_point(mp_obj_t self_in)
{
    sasppu_sprite_t *self = MP_OBJ_TO_PTR(self_in);
    if (self->bound < 0)
    {
        return mp_const_none;
    }
    return MP_OBJ_NEW_SMALL_INT(self->bound);
}
static MP_DEFINE_CONST_FUN_OBJ_1(sasppu_sprite_get_bind_point_obj, sasppu_sprite_get_bind_point);

static const mp_rom_map_elem_t sasppu_sprite_locals_dict_table[] = {
    {MP_ROM_QSTR(MP_QSTR_bind), MP_ROM_PTR(&sasppu_sprite_bind_obj)},
    {MP_ROM_QSTR(MP_QSTR_unbind), MP_ROM_PTR(&sasppu_sprite_unbind_obj)},
    {MP_ROM_QSTR(MP_QSTR_get_bind_point), MP_ROM_PTR(&sasppu_sprite_get_bind_point_obj)},

    {MP_ROM_QSTR(MP_QSTR_WIDTH_POWER), MP_ROM_INT(SPR_WIDTH_POWER)},
    {MP_ROM_QSTR(MP_QSTR_HEIGHT_POWER), MP_ROM_INT(SPR_HEIGHT_POWER)},
    {MP_ROM_QSTR(MP_QSTR_WIDTH), MP_ROM_INT(SPR_WIDTH)},
    {MP_ROM_QSTR(MP_QSTR_HEIGHT), MP_ROM_INT(SPR_HEIGHT)},
    {MP_ROM_QSTR(MP_QSTR_ENABLED), MP_ROM_INT(SPR_ENABLED)},
    {MP_ROM_QSTR(MP_QSTR_PRIORITY), MP_ROM_INT(SPR_PRIORITY)},
    {MP_ROM_QSTR(MP_QSTR_FLIP_X), MP_ROM_INT(SPR_FLIP_X)},
    {MP_ROM_QSTR(MP_QSTR_FLIP_Y), MP_ROM_INT(SPR_FLIP_Y)},
    {MP_ROM_QSTR(MP_QSTR_C_MATH), MP_ROM_INT(SPR_C_MATH)},
    {MP_ROM_QSTR(MP_QSTR_DOUBLE), MP_ROM_INT(SPR_DOUBLE)},
};
static MP_DEFINE_CONST_DICT(sasppu_sprite_locals_dict, sasppu_sprite_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    sasppu_type_sprite,
    MP_QSTR_Sprite,
    MP_TYPE_FLAG_NONE,
    make_new, sasppu_sprite_make_new,
    locals_dict, &sasppu_sprite_locals_dict,
    binary_op, sasppu_sprite_binary_op,
    attr, sasppu_sprite_attr);