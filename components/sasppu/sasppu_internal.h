/**
 * @file sasppu_internal.h
 * @author john hunter <moliveofscratch@gmail.com>
 * @brief Internal header to SASPPU. Don't include.
 * @version 0.1
 * @date 2025-04-05
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef SASPPU_INTERNAL_H_
#define SASPPU_INTERNAL_H_

#include "sasppu.h"
#include "stddef.h"
#include "assert.h"
#include "esp_log.h"

void SASPPU_assert_fail();

extern uint16x8_t SASPPU_subscreen_scanline[240 / 8];
extern mask16x8_t SASPPU_window_cache[(240 / 8) * 2];

#define USE_INLINE_ASM 1
#define VERIFY_INLINE_ASM 0
#define USE_GCC_SIMD ((!USE_INLINE_ASM) | VERIFY_INLINE_ASM)

#define QEMU_EMULATOR 0

#define SIMD_ASSERT(index, cpu_val, simd_val)                                                                      \
    if (cpu_val[index] == simd_val[index])                                                                         \
    {                                                                                                              \
    }                                                                                                              \
    else                                                                                                           \
    {                                                                                                              \
        ESP_LOGI("SIMD_ASRT", "cpu_val[%u]: %u, simd_val[%u]: %u", index, cpu_val[index], index, simd_val[index]); \
        assert(cpu_val[index] == simd_val[index]);                                                                 \
    }

#define CHECK_SIMD(cpu_val, asm_segment)           \
    {                                              \
        volatile uint16x8_t check = VBROADCAST(0); \
        asm_segment;                               \
        SIMD_ASSERT(0, cpu_val, check);            \
        SIMD_ASSERT(1, cpu_val, check);            \
        SIMD_ASSERT(2, cpu_val, check);            \
        SIMD_ASSERT(3, cpu_val, check);            \
        SIMD_ASSERT(4, cpu_val, check);            \
        SIMD_ASSERT(5, cpu_val, check);            \
        SIMD_ASSERT(6, cpu_val, check);            \
        SIMD_ASSERT(7, cpu_val, check);            \
    }

#define CHECK_SIMD_Q0(cpu_val) CHECK_SIMD(cpu_val, asm volatile inline("st.qr q0, %[check], 0" : : [check] "r"(&check)));
#define CHECK_SIMD_Q1(cpu_val) CHECK_SIMD(cpu_val, asm volatile inline("st.qr q1, %[check], 0" : : [check] "r"(&check)));
#define CHECK_SIMD_Q2(cpu_val) CHECK_SIMD(cpu_val, asm volatile inline("st.qr q2, %[check], 0" : : [check] "r"(&check)));
#define CHECK_SIMD_Q3(cpu_val) CHECK_SIMD(cpu_val, asm volatile inline("st.qr q3, %[check], 0" : : [check] "r"(&check)));
#define CHECK_SIMD_Q4(cpu_val) CHECK_SIMD(cpu_val, asm volatile inline("st.qr q4, %[check], 0" : : [check] "r"(&check)));
#define CHECK_SIMD_Q5(cpu_val) CHECK_SIMD(cpu_val, asm volatile inline("st.qr q5, %[check], 0" : : [check] "r"(&check)));
#define CHECK_SIMD_Q6(cpu_val) CHECK_SIMD(cpu_val, asm volatile inline("st.qr q6, %[check], 0" : : [check] "r"(&check)));
#define CHECK_SIMD_Q7(cpu_val) CHECK_SIMD(cpu_val, asm volatile inline("st.qr q7, %[check], 0" : : [check] "r"(&check)));

#if USE_INLINE_ASM
typedef void (*HandleWindowType)(uint16x8_t *const scanline, const uint16_t x);
#else
typedef void (*HandleWindowType)(uint16x8_t *const scanline, const uint16_t x, const uint16x8_t col);
#endif

typedef void (*HandleSpriteType)(uint16x8_t *const scanline, const int16_t y, Sprite *const sprite);
typedef void (*HandleCMathType)(uint16x8_t *const scanline);
typedef void (*HandleScanlineType)(uint16x8_t *const scanline, const int16_t y);
typedef void (*HandleBackgroundType)(uint16x8_t *const scanline, const int16_t y);

#define SCANLINE_END (240 - 8)
#define VBROADCAST(val) {val, val, val, val, val, val, val, val}
extern const uint16x8_t VECTOR_INCREMENTS;
extern const uint16x8_t VECTOR_SCANLINE_END;
extern const uint16x8_t VECTOR_INCREMENTS_END;

extern const uint16_t SASPPU_EIGHT;
extern const uint16_t CMATH_BIT;
extern const uint16_t CMATH_MASK_LOW;
extern const uint16_t CMATH_MASK_GREEN;
extern const uint16_t CMATH_MASK_SPLIT;
extern const uint16_t CMATH_ONE;
extern const uint16_t CMATH_TWO_FOUR;
extern const uint16_t CMATH_TWO_FIVE;
extern const uint16_t CMATH_TWO_EIGHT;
extern const uint16_t CMATH_TWO_NINE;
extern const uint16_t CMATH_TWO_TEN;

extern const HandleSpriteType HANDLE_SPRITE_LOOKUP[16];
extern const HandleScanlineType HANDLE_SCANLINE_LOOKUP[64];
extern const HandleCMathType HANDLE_CMATH_LOOKUP[256];
extern const HandleWindowType HANDLE_WINDOW_LOOKUP[256];

void handle_bg0(uint16x8_t *const scanline, const int16_t y);
void handle_bg1(uint16x8_t *const scanline, const int16_t y);

#if USE_GCC_SIMD
extern const uint16x8_t REVERSE_MASK;
extern const uint16x8_t INTERLEAVE_MASK_LOW;
extern const uint16x8_t INTERLEAVE_MASK_HIGH;
extern const uint16x8_t VECTOR_SHUFFLES[9];
#endif

#endif // SASPPU_INTERNAL_H_