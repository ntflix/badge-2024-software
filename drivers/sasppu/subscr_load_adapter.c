#include "subscr_load_adapter.h"

// TODO: subscr_load_adaptor & subscr_getiter convenience functions
// should be moved to common location for reuse.
static mp_obj_t subscr_load_adaptor(mp_obj_t self_in, mp_obj_t index_in)
{
    return mp_obj_subscr(self_in, index_in, MP_OBJ_SENTINEL);
}
static MP_DEFINE_CONST_FUN_OBJ_2(subscr_load_adaptor_obj, subscr_load_adaptor);

// .getiter special method which returns iterator which works in terms
// of object subscription.
mp_obj_t subscr_getiter(mp_obj_t self_in, mp_obj_iter_buf_t *iter_buf)
{
    mp_obj_t dest[2] = {MP_OBJ_FROM_PTR(&subscr_load_adaptor_obj), self_in};
    return mp_obj_new_getitem_iter(dest, iter_buf);
}