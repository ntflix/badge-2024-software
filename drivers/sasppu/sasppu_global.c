// Include the header file to get access to the MicroPython API
#include "sasppu_global.h"

#include "sasppu_windows.h"
#include "sasppu_background.h"
#include "sasppu_cmath_state.h"
#include "sasppu_main_state.h"
#include "sasppu_sprite.h"
#include "sasppu_tile.h"

static mp_obj_t sasppu_global_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in)
{
    sasppu_global_t *o = mp_obj_malloc(sasppu_global_t, type);

    return MP_OBJ_FROM_PTR(o);
}

static void sasppu_global_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest)
{
    if (dest[0] == MP_OBJ_NULL)
    {
        // Load attribute.
        switch (attr)
        {
        case MP_QSTR_main_state:
            dest[0] = mp_obj_malloc(sasppu_main_state_t, &sasppu_type_main_state);
            sasppu_main_state_from_struct(dest[0], SASPPU_raise_main_state());
            break;
        case MP_QSTR_cmath_state:
            dest[0] = mp_obj_malloc(sasppu_cmath_state_t, &sasppu_type_cmath_state);
            sasppu_cmath_state_from_struct(dest[0], SASPPU_raise_cmath_state());
            break;
        case MP_QSTR_bg0_state:
            dest[0] = mp_obj_malloc(sasppu_background_t, &sasppu_type_background);
            sasppu_background_from_struct(dest[0], SASPPU_bg0_state);
            break;
        case MP_QSTR_bg1_state:
            dest[0] = mp_obj_malloc(sasppu_background_t, &sasppu_type_background);
            sasppu_background_from_struct(dest[0], SASPPU_bg1_state);
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
        case MP_QSTR_main_state:
            if (!mp_obj_is_type(dest[1], &sasppu_type_main_state))
            {
                mp_raise_TypeError(MP_ERROR_TEXT("MainState needed"));
            }
            else
            {
                SASPPU_lower_main_state(sasppu_main_state_to_struct(dest[1]));
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_cmath_state:
            if (!mp_obj_is_type(dest[1], &sasppu_type_cmath_state))
            {
                mp_raise_TypeError(MP_ERROR_TEXT("CMathState needed"));
            }
            else
            {
                SASPPU_lower_cmath_state(sasppu_cmath_state_to_struct(dest[1]));
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_bg0_state:
            if (!mp_obj_is_type(dest[1], &sasppu_type_background))
            {
                mp_raise_TypeError(MP_ERROR_TEXT("Background needed"));
            }
            else
            {
                SASPPU_bg0_state = (sasppu_background_to_struct(dest[1]));
                dest[0] = MP_OBJ_NULL;
            }
            break;
        case MP_QSTR_bg1_state:
            if (!mp_obj_is_type(dest[1], &sasppu_type_background))
            {
                mp_raise_TypeError(MP_ERROR_TEXT("Background needed"));
            }
            else
            {
                SASPPU_bg1_state = (sasppu_background_to_struct(dest[1]));
                dest[0] = MP_OBJ_NULL;
            }
            break;
        default:
            break;
        }
    }
}

static const mp_rom_map_elem_t sasppu_global_locals_dict_table[] = {};
static MP_DEFINE_CONST_DICT(sasppu_global_locals_dict, sasppu_global_locals_dict_table);

MP_DEFINE_CONST_OBJ_TYPE(
    sasppu_type_global,
    MP_QSTR_Global,
    MP_TYPE_FLAG_NONE,
    make_new, sasppu_global_make_new,
    locals_dict, &sasppu_global_locals_dict,
    attr, sasppu_global_attr);