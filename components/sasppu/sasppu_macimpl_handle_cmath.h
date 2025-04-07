#include "sasppu.h"
#include "sasppu_internal.h"

// SASPPU_HANDLE_CMATH(IDENT, HALF_MAIN_SCREEN, DOUBLE_MAIN_SCREEN, HALF_SUB_SCREEN, DOUBLE_SUB_SCREEN, ADD_SUB_SCREEN, SUB_SUB_SCREEN, FADE_ENABLE, CMATH_ENABLE)
#ifndef IDENT
#define IDENT handle_cmath_0_0_0_0_0_0_0_0
#endif

#ifndef HALF_MAIN_SCREEN
#define HALF_MAIN_SCREEN 1
#endif
#ifndef DOUBLE_MAIN_SCREEN
#define DOUBLE_MAIN_SCREEN 1
#endif
#ifndef HALF_SUB_SCREEN
#define HALF_SUB_SCREEN 1
#endif
#ifndef DOUBLE_SUB_SCREEN
#define DOUBLE_SUB_SCREEN 1
#endif
#ifndef ADD_SUB_SCREEN
#define ADD_SUB_SCREEN 1
#endif
#ifndef SUB_SUB_SCREEN
#define SUB_SUB_SCREEN 1
#endif
#ifndef FADE_ENABLE
#define FADE_ENABLE 1
#endif
#ifndef CMATH_ENABLE
#define CMATH_ENABLE 1
#endif

static void IDENT(uint16x8_t *const scanline)
{
#if USE_INLINE_ASM
    asm volatile inline(".include \"sasppu_handle_cmath.i\"");
#endif

    uint16x8_t *maincol __attribute__((__unused__)) = &scanline[(240 / 8) - 1];
#if CMATH_ENABLE
    uint16x8_t *subcol __attribute__((__unused__)) = &SASPPU_subscreen_scanline[(240 / 8) - 1];
#endif

    for (ssize_t x = (240 / 8) - 1; x >= 0; x--)
    {

#if USE_INLINE_ASM
#if CMATH_ENABLE && FADE_ENABLE
        uint16x8_t spill_area[4];
        asm volatile inline("                                   \n\t \
            ld.qr q0, %[maincol], 0             \n\t \
            ee.vld.128.ip q1, %[subcol], -16    \n\t \
            " : [subcol] "+r"(subcol) : [maincol] "r"(maincol));

        asm volatile("handle_cmath_and_fade %[sub_ss], %[add_ss], %[ss_double], %[ss_half], %[ms_double], %[ms_half], %[five], %[ten], %[mask_split], %[one], %[four], %[fade], %[spill_area]" : :
                     [sub_ss] "i"(SUB_SUB_SCREEN),
                     [add_ss] "i"(ADD_SUB_SCREEN),
                     [ss_double] "i"(DOUBLE_SUB_SCREEN),
                     [ss_half] "i"(HALF_SUB_SCREEN),
                     [ms_double] "i"(DOUBLE_MAIN_SCREEN),
                     [ms_half] "i"(HALF_MAIN_SCREEN),
                     [five] "r"(&CMATH_TWO_FIVE),
                     [ten] "r"(&CMATH_TWO_TEN),
                     [mask_split] "r"(&CMATH_MASK_SPLIT),
                     [one] "r"(&CMATH_ONE),
                     [four] "r"(&CMATH_TWO_FOUR),
                     [fade] "r"(&SASPPU_cmath_state.screen_fade),
                     [spill_area] "r"(&spill_area) : "a15");
#elif CMATH_ENABLE && !FADE_ENABLE
        uint16x8_t spill_area[4];
        asm volatile inline("                                   \n\t \
            ld.qr q0, %[maincol], 0             \n\t \
            ee.vld.128.ip q1, %[subcol], -16    \n\t \
            " : [subcol] "+r"(subcol) : [maincol] "r"(maincol));

        asm volatile("handle_cmath_only %[sub_ss], %[add_ss], %[ss_double], %[ss_half], %[ms_double], %[ms_half], %[five], %[ten], %[mask_split], %[one], %[four], %[spill_area]" : :
                     [sub_ss] "i"(SUB_SUB_SCREEN),
                     [add_ss] "i"(ADD_SUB_SCREEN),
                     [ss_double] "i"(DOUBLE_SUB_SCREEN),
                     [ss_half] "i"(HALF_SUB_SCREEN),
                     [ms_double] "i"(DOUBLE_MAIN_SCREEN),
                     [ms_half] "i"(HALF_MAIN_SCREEN),
                     [five] "r"(&CMATH_TWO_FIVE),
                     [ten] "r"(&CMATH_TWO_TEN),
                     [mask_split] "r"(&CMATH_MASK_SPLIT),
                     [one] "r"(&CMATH_ONE),
                     [four] "r"(&CMATH_TWO_FOUR),
                     [spill_area] "r"(&spill_area) : "a15");
#elif !CMATH_ENABLE && FADE_ENABLE
        asm volatile inline("                       \n\t \
            ld.qr q0, %[maincol], 0 \n\t \
            " : : [maincol] "r"(maincol));

        asm volatile("handle_fade_only %[fade], %[one], %[four], %[five], %[ten], %[mask_split]" : :
                     [fade] "r"(&SASPPU_cmath_state.screen_fade),
                     [one] "r"(&CMATH_ONE),
                     [four] "r"(&CMATH_TWO_FOUR),
                     [five] "r"(&CMATH_TWO_FIVE),
                     [ten] "r"(&CMATH_TWO_TEN),
                     [mask_split] "r"(&CMATH_MASK_SPLIT) : "a15");
#elif !CMATH_ENABLE && !FADE_ENABLE
        asm volatile inline("                       \n\t \
            ld.qr q0, %[maincol], 0 \n\t \
            " : : [maincol] "r"(maincol));

        asm volatile("no_cmath_colour_shift %[one], %[eight], %[nine], %[mask_low], %[mask_green]" : :
                     [one] "r"(&CMATH_ONE),
                     [eight] "r"(&CMATH_TWO_EIGHT),
                     [nine] "r"(&CMATH_TWO_NINE),
                     [mask_low] "r"(&CMATH_MASK_LOW),
                     [mask_green] "r"(&CMATH_MASK_GREEN) : "a15");
#endif
        asm volatile inline("ee.vst.128.ip q0, %[maincol], -16" : [maincol] "+r"(maincol));
#endif
    }
}

#undef IDENT
#undef HALF_MAIN_SCREEN
#undef DOUBLE_MAIN_SCREEN
#undef HALF_SUB_SCREEN
#undef DOUBLE_SUB_SCREEN
#undef ADD_SUB_SCREEN
#undef SUB_SUB_SCREEN
#undef FADE_ENABLE
#undef CMATH_ENABLE
