#include "sasppu_mpy_help.h"

static mp_obj_t sasppu_copy_sprite(size_t n_args, const mp_obj_t *args)
{
    mp_int_t dst_x = mp_obj_get_int(args[0]);
    mp_int_t dst_y = mp_obj_get_int(args[1]);
    mp_int_t width = mp_obj_get_int(args[2]);
    mp_int_t height = mp_obj_get_int(args[3]);
    mp_int_t src_x = mp_obj_get_int(args[4]);
    mp_int_t src_y = mp_obj_get_int(args[5]);
    bool double_size = false;
    if (n_args == 7)
    {
        double_size = mp_obj_is_true(args[6]);
    }
    SASPPUImageCode res = SASPPU_copy_sprite(dst_x, dst_y, width, height, src_x, src_y, double_size);
    return MP_OBJ_NEW_SMALL_INT(res);
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_copy_sprite_obj, 6, 7, sasppu_copy_sprite);

static mp_obj_t sasppu_copy_sprite_transparent(size_t n_args, const mp_obj_t *args)
{
    mp_int_t dst_x = mp_obj_get_int(args[0]);
    mp_int_t dst_y = mp_obj_get_int(args[1]);
    mp_int_t width = mp_obj_get_int(args[2]);
    mp_int_t height = mp_obj_get_int(args[3]);
    mp_int_t src_x = mp_obj_get_int(args[4]);
    mp_int_t src_y = mp_obj_get_int(args[5]);
    bool double_size = false;
    if (n_args == 7)
    {
        double_size = mp_obj_is_true(args[6]);
    }
    SASPPUImageCode res = SASPPU_copy_sprite_transparent(dst_x, dst_y, width, height, src_x, src_y, double_size);
    return MP_OBJ_NEW_SMALL_INT(res);
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_copy_sprite_transparent_obj, 6, 7, sasppu_copy_sprite_transparent);

static mp_obj_t sasppu_fill_sprite(size_t n_args, const mp_obj_t *args)
{
    mp_int_t x = mp_obj_get_int(args[0]);
    mp_int_t y = mp_obj_get_int(args[1]);
    mp_int_t width = mp_obj_get_int(args[2]);
    mp_int_t height = mp_obj_get_int(args[3]);
    mp_int_t colour = mp_obj_get_int(args[4]);
    SASPPUImageCode res = SASPPU_fill_sprite(x, y, width, height, colour);
    return MP_OBJ_NEW_SMALL_INT(res);
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_fill_sprite_obj, 5, 5, sasppu_fill_sprite);

static mp_obj_t sasppu_draw_text_sprite(size_t n_args, const mp_obj_t *args)
{
    mp_int_t x = mp_obj_get_int(args[0]);
    mp_int_t y = mp_obj_get_int(args[1]);
    mp_int_t colour = mp_obj_get_int(args[2]);
    mp_int_t line_width = mp_obj_get_int(args[3]);
    mp_check_self(mp_obj_is_str_or_bytes(args[4]));
    GET_STR_DATA_LEN(args[4], text, text_len);
    mp_int_t newline_height = 10;
    bool double_size = false;
    if (n_args == 6)
    {
        double_size = mp_obj_is_true(args[5]);
    }
    if (n_args == 7)
    {
        newline_height = mp_obj_get_int(args[6]);
    }
    SASPPUImageCode res = SASPPU_draw_text_sprite(x, y, colour, line_width, newline_height, double_size, (const char *)text);
    return MP_OBJ_NEW_SMALL_INT(res);
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_draw_text_sprite_obj, 5, 7, sasppu_draw_text_sprite);

static mp_obj_t sasppu_copy_background(size_t n_args, const mp_obj_t *args)
{
    mp_int_t dst_x = mp_obj_get_int(args[0]);
    mp_int_t dst_y = mp_obj_get_int(args[1]);
    mp_int_t width = mp_obj_get_int(args[2]);
    mp_int_t height = mp_obj_get_int(args[3]);
    mp_int_t src_x = mp_obj_get_int(args[4]);
    mp_int_t src_y = mp_obj_get_int(args[5]);
    bool double_size = false;
    if (n_args == 7)
    {
        double_size = mp_obj_is_true(args[6]);
    }
    SASPPUImageCode res = SASPPU_copy_background(dst_x, dst_y, width, height, src_x, src_y, double_size);
    return MP_OBJ_NEW_SMALL_INT(res);
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_copy_background_obj, 6, 7, sasppu_copy_background);

static mp_obj_t sasppu_copy_background_transparent(size_t n_args, const mp_obj_t *args)
{
    mp_int_t dst_x = mp_obj_get_int(args[0]);
    mp_int_t dst_y = mp_obj_get_int(args[1]);
    mp_int_t width = mp_obj_get_int(args[2]);
    mp_int_t height = mp_obj_get_int(args[3]);
    mp_int_t src_x = mp_obj_get_int(args[4]);
    mp_int_t src_y = mp_obj_get_int(args[5]);
    bool double_size = false;
    if (n_args == 7)
    {
        double_size = mp_obj_is_true(args[6]);
    }
    SASPPUImageCode res = SASPPU_copy_background_transparent(dst_x, dst_y, width, height, src_x, src_y, double_size);
    return MP_OBJ_NEW_SMALL_INT(res);
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_copy_background_transparent_obj, 6, 7, sasppu_copy_background_transparent);

static mp_obj_t sasppu_fill_background(size_t n_args, const mp_obj_t *args)
{
    mp_int_t x = mp_obj_get_int(args[0]);
    mp_int_t y = mp_obj_get_int(args[1]);
    mp_int_t width = mp_obj_get_int(args[2]);
    mp_int_t height = mp_obj_get_int(args[3]);
    mp_int_t colour = mp_obj_get_int(args[4]);
    SASPPUImageCode res = SASPPU_fill_background(x, y, width, height, colour);
    return MP_OBJ_NEW_SMALL_INT(res);
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_fill_background_obj, 5, 5, sasppu_fill_background);

static mp_obj_t sasppu_draw_text_background(size_t n_args, const mp_obj_t *args)
{
    mp_int_t x = mp_obj_get_int(args[0]);
    mp_int_t y = mp_obj_get_int(args[1]);
    mp_int_t colour = mp_obj_get_int(args[2]);
    mp_int_t line_width = mp_obj_get_int(args[3]);
    mp_check_self(mp_obj_is_str_or_bytes(args[4]));
    GET_STR_DATA_LEN(args[4], text, text_len);
    mp_int_t newline_height = 10;
    bool double_size = false;
    if (n_args == 6)
    {
        double_size = mp_obj_is_true(args[5]);
    }
    if (n_args == 7)
    {
        newline_height = mp_obj_get_int(args[6]);
    }
    SASPPUImageCode res = SASPPU_draw_text_background(x, y, colour, line_width, newline_height, double_size, (const char *)text);
    return MP_OBJ_NEW_SMALL_INT(res);
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(sasppu_draw_text_background_obj, 5, 7, sasppu_draw_text_background);