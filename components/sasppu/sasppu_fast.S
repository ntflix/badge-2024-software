//
// ESP32-S3 ASM SIMD code
// Part of SASPPU
// Written by Molive
//
// This code is a disaster. Every block is trying to do about 3 things at once, with the code interleaved to avoid data hazards.
// Good luck.
//

// Sprite
// size = 10, align = 2
  .struct 0
Sprite:
  //.space 0
Sprite.x:
  .space 2
Sprite.y:
  .space 2
Sprite.width:
  .space 1
Sprite.height:
  .space 1
Sprite.graphics.x:
  .space 1
Sprite.graphics.y:
  .space 1
Sprite.windows:
  .space 1
Sprite.flags:
  .space 1
size_of_Sprite:

// size = 6, align = 2
  .struct 0
Background:
  //.space 0
Background.scroll.x:
  .space 2
Background.scroll.y:
  .space 2
Background.windows:
  .space 1
Background.flags:
  .space 1
size_of_Background:

.set BG_WIDTH_POWER, (8)
.set BG_HEIGHT_POWER, (8)
.set BG_WIDTH, (1 << BG_WIDTH_POWER)
.set BG_HEIGHT, (1 << BG_HEIGHT_POWER)

.set SPRITE_COUNT, (256)
.set SPRITE_CACHE, (16)

.set SPR_WIDTH_POWER, (8)
.set SPR_HEIGHT_POWER, (8)
.set SPR_WIDTH, (1 << SPR_WIDTH_POWER)
.set SPR_HEIGHT, (1 << SPR_HEIGHT_POWER)

.set MAP_WIDTH_POWER, (6)
.set MAP_HEIGHT_POWER, (6)
.set MAP_WIDTH, (1 << MAP_WIDTH_POWER)
.set MAP_HEIGHT, (1 << MAP_HEIGHT_POWER)

.set SPR_ENABLED, (1 << 0)
.set SPR_PRIORITY, (1 << 1)
.set SPR_FLIP_X, (1 << 2)
.set SPR_FLIP_Y, (1 << 3)
.set SPR_C_MATH, (1 << 4)
.set SPR_DOUBLE, (1 << 5)

.set BG_C_MATH, (1 << 0)

.set CMATH_HALF_MAIN_SCREEN, (1 << 0)
.set CMATH_DOUBLE_MAIN_SCREEN, (1 << 1)
.set CMATH_HALF_SUB_SCREEN, (1 << 2)
.set CMATH_DOUBLE_SUB_SCREEN, (1 << 3)
.set CMATH_ADD_SUB_SCREEN, (1 << 4)
.set CMATH_SUB_SUB_SCREEN, (1 << 5)
.set CMATH_FADE_ENABLE, (1 << 6)
.set CMATH_CMATH_ENABLE, (1 << 7)

.set WINDOW_A, (0b0001)
.set WINDOW_B, (0b0010)
.set WINDOW_AB, (0b0100)
.set WINDOW_X, (0b1000)

  .section .bss
  .align 2
  .global SASPPU_main_state_mainscreen_colour
  .type   SASPPU_main_state_mainscreen_colour, @object
SASPPU_main_state_mainscreen_colour:
  .zero 2
  .global SASPPU_main_state_subscreen_colour
  .type   SASPPU_main_state_subscreen_colour, @object
SASPPU_main_state_subscreen_colour:
  .zero 2
  .global SASPPU_main_state_window_1_left
  .type   SASPPU_main_state_window_1_left, @object
SASPPU_main_state_window_1_left:
  .zero 2
  .global SASPPU_main_state_window_1_right
  .type   SASPPU_main_state_window_1_right, @object
SASPPU_main_state_window_1_right:
  .zero 2
  .global SASPPU_main_state_window_2_left
  .type   SASPPU_main_state_window_2_left, @object
SASPPU_main_state_window_2_left:
  .zero 2
  .global SASPPU_main_state_window_2_right
  .type   SASPPU_main_state_window_2_right, @object
SASPPU_main_state_window_2_right:
  .zero 2
  .global SASPPU_main_state_bgcol_windows
  .type   SASPPU_main_state_bgcol_windows, @object
SASPPU_main_state_bgcol_windows:
  .zero 1
  .global SASPPU_main_state_flags
  .type   SASPPU_main_state_flags, @object
SASPPU_main_state_flags:
  .zero 1
  .align 2
  .global SASPPU_bg0_state
  .type   SASPPU_bg0_state, @object
SASPPU_bg0_state:
  .zero size_of_Background
  .global SASPPU_bg1_state
  .type   SASPPU_bg1_state, @object
  .align 2
SASPPU_bg1_state:
  .zero size_of_Background
  .global SASPPU_cmath_state_screen_fade
  .type   SASPPU_cmath_state_screen_fade, @object
  .align 2
SASPPU_cmath_state_screen_fade:
  .zero 2
  .global SASPPU_cmath_state_flags
  .type   SASPPU_cmath_state_flags, @object
SASPPU_cmath_state_flags:
  .zero 1

  .align 4
  .global SASPPU_sprite_cache
  .type   SASPPU_sprite_cache, @object
SASPPU_sprite_cache:
  .zero (SPRITE_CACHE * 4 * 2)

  .section .ext_ram.bss
  .align 2
  .global SASPPU_oam
  .type   SASPPU_oam, @object
SASPPU_oam:
  .zero (size_of_Sprite * SPRITE_COUNT)
  
  .align 2
  .global SASPPU_bg0
  .type   SASPPU_bg0, @object
SASPPU_bg0:
  .zero (MAP_WIDTH * MAP_HEIGHT * 2)
  .global SASPPU_bg1
  .type   SASPPU_bg1, @object
SASPPU_bg1:
  .zero (MAP_WIDTH * MAP_HEIGHT * 2)

// Constants
.section .rodata
.align 16
vector_increments:
  .short 0
  .short 1
  .short 2
  .short 3
  .short 4
  .short 5
  .short 6
  .short 7
cmath_bit:
  .short 0x8000
cmath_mask_low:
  .short 0b0000000000011111
cmath_mask_high:
  .short 0b0111111111100000
cmath_mask_split:
  .short 0b1111100000000000
cmath_one:
  .short 1
cmath_two:
  .short 2
cmath_six:
  .short (1 << 6)
cmath_eleven:
  .short (1 << 11)

.macro window_macro_q6 wincase
.if \wincase == 14
  ee.orq q5, q2, q3
  ee.notq q6, q6
  ee.andq q5, q5, q6
  ee.notq q6, q5
.endif
.if \wincase == 13
  ee.andq q5, q2, q3
  ee.orq q6, q5, q6
  ee.notq q5, q6
.endif
.if \wincase == 12
  ee.xorq q5, q2, q3
  ee.notq q6, q6
  ee.andq q5, q5, q6
  ee.notq q6, q5
.endif
.if \wincase == 11
  ee.notq q5, q3
  ee.notq q6, q6
  ee.orq q5, q2, q5
  ee.andq q5, q5, q6
  ee.notq q6, q5
.endif
.if \wincase == 10
  ee.notq q6, q6
  ee.andq q5, q2, q6
  ee.notq q6, q5
.endif
.if \wincase == 9
  ee.orq q6, q3, q6
  ee.notq q5, q6
.endif
.if \wincase == 8
  ee.notq q5, q3
  ee.andq q5, q2, q5
  ee.notq q6, q6
  ee.andq q5, q5, q6
  ee.notq q6, q5
.endif
.if \wincase == 7
  ee.notq q5, q2
  ee.orq q5, q3, q5
  ee.notq q6, q6
  ee.andq q5, q5, q6
  ee.notq q6, q5
.endif
.if \wincase == 6
  ee.notq q6, q6
  ee.andq q5, q3, q6
  ee.notq q6, q5
.endif
.if \wincase == 5
  ee.orq q6, q2, q6
  ee.notq q5, q6
.endif
.if \wincase == 4
  ee.notq q5, q2
  ee.andq q5, q3, q5
  ee.notq q6, q6
  ee.andq q5, q5, q6
  ee.notq q6, q5
.endif
.if \wincase == 3
  ee.xorq q5, q2, q3
  ee.orq q6, q5, q6
  ee.notq q5, q6
.endif
.if \wincase == 2
  ee.andq q5, q2, q3
  ee.notq q6, q6
  ee.andq q5, q5, q6
  ee.notq q6, q5
.endif
.if \wincase == 1
  ee.orq q5, q2, q3
  ee.orq q6, q5, q6
  ee.notq q5, q6
.endif
.if \wincase == 15
  ee.notq q5, q6
.endif
.endm

.macro window_macro_q7 wincase
.if \wincase == 15
  mv.qr q6, q7
  ee.notq q5, q6
.endif
.if \wincase == 14
  ee.orq q5, q2, q3
  ee.notq q6, q7
  ee.andq q5, q5, q6
  ee.notq q6, q5
.endif
.if \wincase == 13
  ee.andq q5, q2, q3
  ee.orq q6, q5, q7
  ee.notq q5, q6
.endif
.if \wincase == 12
  ee.xorq q5, q2, q3
  ee.notq q6, q7
  ee.andq q5, q5, q6
  ee.notq q6, q5
.endif
.if \wincase == 11
  ee.notq q5, q3
  ee.orq q5, q2, q5
  ee.notq q6, q7
  ee.andq q5, q5, q6
  ee.notq q6, q5
.endif
.if \wincase == 10
  ee.notq q6, q7
  ee.andq q5, q2, q6
  ee.notq q6, q5
.endif
.if \wincase == 9
  ee.orq q6, q3, q7
  ee.notq q5, q6
.endif
.if \wincase == 8
  ee.notq q5, q3
  ee.andq q5, q2, q5
  ee.notq q6, q7
  ee.andq q5, q5, q6
  ee.notq q6, q5
.endif
.if \wincase == 7
  ee.notq q5, q2
  ee.orq q5, q3, q5
  ee.notq q6, q7
  ee.andq q5, q5, q6
  ee.notq q6, q5
.endif
.if \wincase == 6
  ee.notq q6, q7
  ee.andq q5, q3, q6
  ee.notq q6, q5
.endif
.if \wincase == 5
  ee.orq q6, q2, q7
  ee.notq q5, q6
.endif
.if \wincase == 4
  ee.notq q5, q2
  ee.andq q5, q3, q5
  ee.notq q6, q7
  ee.andq q5, q5, q6
  ee.notq q6, q5
.endif
.if \wincase == 3
  ee.xorq q5, q2, q3
  ee.orq q6, q5, q7
  ee.notq q5, q6
.endif
.if \wincase == 2
  ee.andq q5, q2, q3
  ee.notq q6, q7
  ee.andq q5, q5, q6
  ee.notq q6, q5
.endif
.if \wincase == 1
  ee.orq q5, q2, q3
  ee.orq q6, q5, q7
  ee.notq q5, q6
.endif
.endm

// both window settings are the same.
.macro window_macro_both wincase
.if \wincase == 0
.exitm
.endif

  ee.zero.q q6
  ee.vcmp.eq.s16 q6, q4, q6
  ee.notq q6, q6
  window_macro_q6 \wincase
  ee.andq q0, q0, q5
  ee.andq q1, q1, q5
  ee.andq q4, q4 ,q6
  ee.orq q0, q0, q4
  ee.orq q1, q1, q4
.endm

// just the main window
.macro window_macro_main wincase
.if \wincase == 0
.exitm
.endif

  window_macro_q7 \wincase
  ee.andq q0, q0, q5
  ee.andq q6, q4 ,q6
  ee.orq q0, q0, q6
.endm

// just the sub window
.macro window_macro_sub wincase
.if \wincase == 0
.exitm
.endif

  window_macro_q7 \wincase
  ee.andq q1, q1, q5
  ee.andq q6, q4 ,q6
  ee.orq q1, q1, q6
.endm

// Calculates the windows
.macro window_macro mainwincase, subwincase
.if \mainwincase == \subwincase
  window_macro_both \mainwincase
.else

.if \subwincase == 0
  ee.zero.q q6
  ee.vcmp.eq.s16 q6, q4, q6
  ee.notq q6, q6
  window_macro_q6 \mainwincase
  ee.andq q0, q0, q5
  ee.andq q4, q4 ,q6
  ee.orq q0, q0, q4
.else
.if \mainwincase == 0
  ee.zero.q q6
  ee.vcmp.eq.s16 q6, q4, q6
  ee.notq q6, q6
  window_macro_q6 \subwincase
  ee.andq q1, q1, q5
  ee.andq q4, q4 ,q6
  ee.orq q1, q1, q4
.else
  ee.zero.q q7
  ee.vcmp.eq.s16 q7, q4, q7
  ee.notq q7, q7
  window_macro_main \mainwincase
  window_macro_sub \subwincase
.endif
.endif

.endif
.endm

.macro window_wrapper subwincase, mainwincase
  entry sp, 0+64
  window_macro \mainwincase, \subwincase
  retw.n
.endm

// void handle_bg( //a0 return, a1 stack
  // uint8_t x, // a2
  // uint8_t y, // a3
  // SpriteCaches *sprite_caches, // a4
  // HandleBgType *handle_bg0, // a5
  // HandleBgType *handle_bg1, // a6
  // HandleCMathType *handle_cmath, // a7
  // vector_increments, // a8
  //);
.macro handle_bg bgn, unique
.if \bgn == 0
  l32r  a9,  .LCbg0_state
  l32r  a10, .LCbg0
.else
  l32r  a9,  .LCbg1_state
  l32r  a10, .LCbg1
.endif
  l16si  a11, a9, Background.scroll.x // load scroll.x
  l16si  a12, a9, Background.scroll.y // load scroll.y
  add.n  a11, a11, a2 // x + scroll.x
  add.n  a12, a12, a3 // y + scroll.y
  extui  a13, a11, 3, (MAP_WIDTH_POWER) // x_pos_1
  extui  a14, a12, 3, (MAP_HEIGHT_POWER) // y_pos
  addi.n a15, a13, 1 // x_pos_2
  slli  a14, a14, (MAP_WIDTH_POWER)
  extui  a15, a15, 0, (MAP_WIDTH_POWER) // x_pos_2
  addx2  a10, a14, a10
  movi a14, 7
  addx2  a15, a15, a10
  and a11, a11, a14 // offset_x ((x + scroll.x) & 7)
  slli a11, a11, 1
  wur.sar_byte a11
  addx2  a13, a13, a10
  and a12, a12, a14 // offset_y ((y + scroll.y) & 7)
  l16ui a15, a15, 0 // bg0_2_map
  l16ui a13, a13, 0 // bg0_1_map

  l32r a9, .LCbackground
.Ly_flip_1_\unique:
  mov.n a10, a12
  bbci.l a13, 1, .Lno_y_flip_1_\unique
  sub a10, a14, a10
.Lno_y_flip_1_\unique:
  slli a10, a10, (BG_WIDTH_POWER)
  add.n a10, a13, a10
  addx2 a10, a10, a9
  ld.qr q4, a10, 0 // load bg0_1 (offset_y flip)
.Ly_flip_2_\unique:
  mov.n a10, a12
  bbci.l a15, 1, .Lno_y_flip_2_\unique
  sub a10, a14, a10
.Lno_y_flip_2_\unique:
  slli a10, a10, (BG_WIDTH_POWER)
  add.n a10, a15, a10
  addx2 a10, a10, a9
  ld.qr q5, a10, 0 // load bg0_1 (offset_y flip)

  bbci.l a13, 0, .Lno_x_flip_1_\unique
  bbci.l a15, 0, .Lno_x_flip_2_1_\unique
  // flip both
  ee.vzip.16 q4, q5
  ee.vzip.16 q5, q4
  ee.vzip.16 q4, q5
  ee.vzip.16 q5, q4
  ee.src.q.ld.ip q7, a9, 0, q5, q4 // swimzleoo
  mv.qr q4, q5
  j .Lflip_end_\unique
.Lno_x_flip_2_1_\unique:
  // flip just 1
  ee.vzip.16 q4, q6
  ee.vzip.16 q6, q4
  ee.vzip.16 q4, q6
  ee.vzip.16 q6, q4
  ee.src.q.ld.ip q7, a9, 0, q6, q5 // swimzleoo
  mv.qr q4, q6
  j .Lflip_end_\unique
.Lno_x_flip_1_\unique:
  bbci.l a15, 0, .Lno_x_flip_2_2_\unique
  // flip just 2
  ee.vzip.16 q5, q6
  ee.vzip.16 q6, q5
  ee.vzip.16 q5, q6
  ee.vzip.16 q6, q5
  ee.src.q.ld.ip q7, a9, 0, q4, q6 // swimzleoo
  j .Lflip_end_\unique
.Lno_x_flip_2_2_\unique:
  ee.src.q.ld.ip q7, a9, 0, q4, q5 // swimzleoo
.Lflip_end_\unique:

.if \bgn == 0
  l32r  a9,  .LCbg0_state
.else
  l32r  a9,  .LCbg1_state
.endif
  l8ui  a15, a9, Background.flags
  bbci.l a15, 0, .Lno_cmath_\unique
  l32r a14, .LCcmath_bit
  ee.vldbc.16.ip q5, a14, 0
  ee.orq q4, q4, q5 
.Lno_cmath_\unique:
.if \bgn == 0
  callx8 a5
.else
  callx8 a6
.endif
  //mv.qr q0, q4
.endm

// void handle_sprite( //a0 return, a1 stack
  // uint8_t x, // a2
  // uint8_t y, // a3
  // Sprite *sprite, // a4
  //);
.macro handle_sprite unique, double, c_math, flip_y, flip_x
  entry sp, 0+64
  l8ui a10, a4, Sprite.width
  l16si a13, a4, Sprite.x
.if \double
  slli a10, a10, 1
.endif
  sub a11, a2, a13 // offset_x = x - sprite.x;
  movi a12, -7
  blt a11, a12, .Lhandle_sprite_exit_\unique
  bge a11, a10, .Lhandle_sprite_exit_\unique
  l16si a12, a4, Sprite.y
.if \flip_x
  sub a11, a10, a11
  addi a11, a11, -1
.endif
  sub a12, a3, a12 // offset_y = y - sprite.y;
.if \flip_y
  l8ui a10, a4, Sprite.height
.if \double
  slli a10, a10, 1
.endif
  sub a12, a10, a12
  addi a12, a12, -1
.endif
.if \double
  srai a12, a12, 1 // offset_y
  srai a11, a11, 1
.endif
.if \double
  movi a10, -4
.else
  movi a10, -8
.endif
  and a10, a11, a10 // x_pos_1
.if \flip_x
  addi a11, a10, -8 // x_pos_2
.else
  addi a11, a10, 8 // x_pos_2
.endif

  addi.n a13, a13, -1
  extui a13, a13, 0, 3
  movi a14, 7
  sub a13, a14, a13 //offset
  slli a13, a13, 1
  wur.sar_byte a13

  slli a12, a12, SPR_WIDTH_POWER

  l8ui a13, a4, Sprite.width
  ee.zero.q q4
  bge a10, a13, .Lspr_1_end_\unique
  bltz a10, .Lspr_1_end_\unique

  l16ui a15, a4, Sprite.graphics.x
  add a14, a12, a15
  add a14, a10, a14
  l32r a15, .LCsprites
  addx2 a14, a14, a15
  ld.qr q4, a14, 0

.Lspr_1_end_\unique:

  ee.zero.q q5
  bge a11, a13, .Lspr_2_end_\unique
  bltz a11, .Lspr_2_end_\unique

  l16ui a15, a4, Sprite.graphics.x
  add a14, a12, a15
  add a14, a11, a14
  l32r a15, .LCsprites
  addx2 a14, a14, a15
  ld.qr q5, a14, 0

.Lspr_2_end_\unique:

.if \double
  movi a11, 0x4
  bany a10, a11, .Ldouble_fix_not_zero_\unique
.if \flip_x
  mv.qr q6, q4
  ee.vzip.16 q4, q6
  mv.qr q6, q5
  ee.vzip.16 q6, q5
.else
  mv.qr q5, q4
  ee.vzip.16 q4, q5
.endif
  j .Ldouble_fix_exit_\unique
.Ldouble_fix_not_zero_\unique:
.if \flip_x
  mv.qr q5, q4
  ee.vzip.16 q5, q4
.else
  mv.qr q6, q4
  ee.vzip.16 q6, q4
  mv.qr q6, q5
  ee.vzip.16 q5, q6
.endif
.Ldouble_fix_exit_\unique:
.endif

.if \flip_x
  ee.vzip.16 q4, q5
  ee.vzip.16 q5, q4
  ee.vzip.16 q4, q5
  ee.vzip.16 q5, q4
  ee.src.q.ld.ip q7, a4, 0, q5, q4 // swimzleoo
  mv.qr q4, q5
.else
  ee.src.q.ld.ip q7, a4, 0, q4, q5 // swimzleoo
.endif

.if \c_math
  l32r a14, .LCcmath_bit
  ee.vldbc.16.ip q5, a14, 0
  ee.orq q4, q4, q5 
.endif

  l8ui a15, a4, Sprite.windows
  l32r a14, .LCwindow_jump_table
  addx4 a14, a15, a14
  l32i.n a14, a14, 0
  addi.n a14, a14, 3 // skip the entry call
  jx a14
  
.Lhandle_sprite_exit_\unique:
  retw.n
.endm

.macro handle_spr_wrapper layer, unique
.if \layer == 0
  l32r a4, .LCsprite_cache_1
.else
  l32r a4, .LCsprite_cache_2
.endif
  mov.n a10, a2
  mov.n a11, a3
.Lspr_loop_\unique:
  l32i.n a12, a4, 0
  beqz a12, .Lspr_next_\unique
  l8ui a14, a12, Sprite.flags
  movi a15, -4
  l32r a13, .LCsprite_jump_table
  and a14, a15, a14
  add a15, a14, a13
  l32i.n a15, a15, 0
  callx8 a15
.Lspr_next_\unique:
  addi a4, a4, 4
.if \layer == 0
  l32r a8, .LCsprite_cache_2
.else
  l32r a8, .LCsprite_cache_end
.endif
  bltu a4, a8, .Lspr_loop_\unique
.endm

.macro colour_split_main
  movi a15, 0
  wsr.sar a15
  l32r a12, .LCcmath_six
  l32r a13, .LCcmath_eleven
  l32r a14, .LCcmath_two
  l32r a11, .LCcmath_mask_split
  ee.vldbc.16.ip q2, a12, 0 // six
  ee.vldbc.16.ip q3, a13, 0 // eleven
  ee.vldbc.16.ip q4, a11, 0 // mask
  ee.vldbc.16.ip q5, a14, 0 // two
  ee.vmul.u16 q7, q0, q3 // blue
  ee.vmul.u16 q6, q0, q2 // green
  ee.vmul.u16 q5, q0, q5 // red
  ee.andq q6, q6, q4
  ee.andq q5, q5, q4
.endm

.macro colour_split_sub
  ee.vldbc.16.ip q0, a14, 0 // two
  ee.vmul.u16 q3, q1, q3 // blue
  ee.vmul.u16 q2, q1, q2 // green
  ee.vmul.u16 q1, q1, q0 // red
  ee.andq q2, q2, q4
  ee.andq q1, q1, q4
.endm

.macro colour_split_both
  colour_split_main
  colour_split_sub
.endm

.macro no_cmath_colour_shift
  l32r a12, .LCcmath_two
  l32r a11, .LCcmath_mask_high
  l32r a10, .LCcmath_mask_low
  ee.vldbc.16.ip q3, a12, 0
  ee.vldbc.16.ip q2, a11, 0
  ee.vldbc.16.ip q1, a10, 0
  movi a12, 0
  ee.andq q7, q0, q2
  wsr.sar a12
  ee.andq q6, q0, q1
  ee.vmul.u16 q5, q7, q3 //shl 1
  ee.orq q0, q5, q6
.endm

.macro recombine_colour // assumes cmath_one in q0
  movi a12, 5
  wsr.sar a12
  movi a12, 11
  ee.vmul.u16 q6, q6, q0
  wsr.sar a12
  ee.vmul.u16 q7, q7, q0
  ee.orq q0, q6, q5
  ee.orq q0, q0, q7
.endm

.macro double_screen colr, colg, colb
  ee.vadds.s16 \colr, \colr, \colr //shl 1
  ee.vadds.s16 \colg, \colg, \colg //shl 1
  ee.vadds.s16 \colb, \colb, \colb //shl 1
.endm

.macro halve_screen colr, colg, colb // assumes cmath_one in q0, 1 in SAR
  ee.vmul.u16 \colr, \colr, q0 //shr 1
  ee.vmul.u16 \colg, \colg, q0 //shr 1
  ee.vmul.u16 \colb, \colb, q0 //shr 1
.endm

.macro add_screens
  ee.vadds.s16 q5, q5, q1
  ee.vadds.s16 q6, q6, q2
  ee.vadds.s16 q7, q7, q3
.endm

.macro sub_screens
  ee.vsubs.s16 q5, q5, q1
  ee.vsubs.s16 q6, q6, q2
  ee.vsubs.s16 q7, q7, q3
.endm

.macro handle_fade_only
  colour_split_main
  movi a12, 8
  wsr.sar a12
  l32r a13, .LCcmath_fade
  l32r a11, .LCcmath_one
  ee.vldbc.16.ip q3, a13, 0
  ee.vldbc.16.ip q0, a11, 0
  ee.vmul.u16 q5, q5, q3
  ee.vmul.u16 q6, q6, q3
  ee.vmul.u16 q7, q7, q3
  ee.andq q5, q5, q4
  ee.andq q6, q6, q4
  ee.andq q7, q7, q4
  recombine_colour
.endm

.macro handle_cmath_body sub_ss, add_ss, ss_double, ss_half, ms_double, ms_half
  l32r a11, .LCcmath_one
  ee.vldbc.16.ip q0, a11, 0
.if \ms_half || \ss_half // preload the halving constants
  movi a12, 1
  wsr.sar a12
.endif

.if \ms_double
  double_screen q5, q6, q7
.endif

.if \ms_half
  halve_screen q5, q6, q7
.endif

.if \ss_double
  double_screen q1, q2, q3
.endif

.if \ss_half
  halve_screen q1, q2, q3
.endif

.if \add_ss
  add_screens
.endif

.if \sub_ss
  sub_screens
.endif
.endm

.macro handle_cmath_only sub_ss, add_ss, ss_double, ss_half, ms_double, ms_half
  ee.zero.q q7
  ee.vcmp.lt.s16 q4, q0, q7 // less than zero means bit 15 set
  st.qr q4, sp, 0 // Ive run out of registers, so this is going to be sent to the stack for its crimes.
  colour_split_both
  st.qr q5, sp, 16 // main_r_bak
  st.qr q6, sp, 32 // main_g_bak
  st.qr q7, sp, 48 // main_b_bak

  handle_cmath_body \sub_ss, \add_ss, \ss_double, \ss_half, \ms_double, \ms_half

  ld.qr q3, sp, 0 // use_cmath
  ld.qr q0, sp, 16 // main_r_bak
  ld.qr q1, sp, 32 // main_g_bak
  ld.qr q2, sp, 48 // main_b_bak
  ee.andq q5, q5, q3
  ee.andq q6, q6, q3
  ee.andq q7, q7, q3
  ee.notq q3, q3
  ee.andq q0, q0, q3
  ee.andq q1, q1, q3
  ee.andq q2, q2, q3
  ee.orq q5, q5, q0
  ee.orq q6, q6, q1
  ee.orq q7, q7, q2
  
  ee.vldbc.16.ip q0, a11, 0
  recombine_colour
.endm

.macro handle_cmath_and_fade sub_ss, add_ss, ss_double, ss_half, ms_double, ms_half
  ee.zero.q q7
  ee.vcmp.lt.s16 q4, q0, q7 // less than zero means bit 15 set
  st.qr q4, sp, 0 // Ive run out of registers, so this is going to be sent to the stack for its crimes.
  colour_split_both
  st.qr q5, sp, 16 // main_r_bak
  st.qr q6, sp, 32 // main_g_bak
  st.qr q7, sp, 48 // main_b_bak

  handle_cmath_body \sub_ss, \add_ss, \ss_double, \ss_half, \ms_double, \ms_half

  ld.qr q3, sp, 0 // use_cmath
  ld.qr q0, sp, 16 // main_r_bak
  ld.qr q1, sp, 32 // main_g_bak
  ld.qr q2, sp, 48 // main_b_bak
  ee.andq q5, q5, q3
  ee.andq q6, q6, q3
  ee.andq q7, q7, q3
  ee.notq q3, q3
  ee.andq q0, q0, q3
  ee.andq q1, q1, q3
  ee.andq q2, q2, q3
  ee.orq q5, q5, q0
  ee.orq q6, q6, q1
  ee.orq q7, q7, q2
  
  movi a12, 8
  wsr.sar a12
  l32r a13, .LCcmath_fade
  ee.vldbc.16.ip q3, a13, 0
  ee.vldbc.16.ip q0, a11, 0
  ee.vmul.u16 q5, q5, q3
  ee.vmul.u16 q6, q6, q3
  ee.vmul.u16 q7, q7, q3
  ee.andq q5, q5, q4
  ee.andq q6, q6, q4
  ee.andq q7, q7, q4

  recombine_colour
.endm

// void handle_cmath( //a0 return, a1 stack
  // uint8_t x, // a2
  // uint8_t y, // a3
  // SpriteCaches *sprite_caches, // a4
  // HandleBgType *handle_bg0, // a5
  // HandleBgType *handle_bg1, // a6
  // HandleCMathType *handle_cmath, // a7
  // vector_increments, // a8
  // Sprite *sprite, // a9
  //);
.macro handle_cmath unique, cmath_enable, fade_enable, sub_ss, add_ss, ss_double, ss_half, ms_double, ms_half
.if \fade_enable && !\cmath_enable
  handle_fade_only
  .exitm
.endif
.if !\fade_enable && \cmath_enable
  handle_cmath_only \sub_ss, \add_ss, \ss_double, \ss_half, \ms_double, \ms_half
  .exitm
.endif
.if \fade_enable && \cmath_enable
  handle_cmath_and_fade \sub_ss, \add_ss, \ss_double, \ss_half, \ms_double, \ms_half
  .exitm
.endif
.if !\fade_enable && !\cmath_enable
  no_cmath_colour_shift
  .exitm
.endif
.endm

.macro handle_cmath_wrapper unique, cmath_enable, fade_enable, sub_ss, add_ss, ss_double, ss_half, ms_double, ms_half
  handle_cmath \unique, \cmath_enable, \fade_enable, \sub_ss, \add_ss, \ss_double, \ss_half, \ms_double, \ms_half
  retw.n
.endm

// void per_pixel( //a0 return, a1 stack
  // uint8_t x, // a2
  // uint8_t y, // a3
  // SpriteCaches *handle_bgcol_window, // a4
  // HandleBgType *handle_bg0, // a5
  // HandleBgType *handle_bg1, // a6
  // HandleCMathType *handle_cmath, // a7
//);
.macro per_pixel_macro unique, bgcol_window_enable, cmath_enable, bg1_enable, bg0_enable, spr1_enable, spr0_enable
  entry sp, 64+64
  l32r a8, .LCvector_increments
  l32r a9, .LCmainscreen_colour
  l32r a10, .LCsubscreen_colour
  l32r a11, .LCwindow_1_left
  l32r a12, .LCwindow_1_right
  l32r a13, .LCwindow_2_left
  l32r a14, .LCwindow_2_right

  s32i.n a2, sp, 0
  ld.qr q0, a8, 0 // load increments
  ee.vldbc.16.ip q1, sp, 0 // broadcast x 
  ee.vadds.s16 q0, q0, q1 // x_window
  ee.vldbc.16.ip q2, a11, 0 // load window_1_left
  ee.vldbc.16.ip q3, a12, 0 // load window_1_right
  ee.vcmp.eq.s16 q4, q0, q2
  ee.vcmp.gt.s16 q5, q0, q2
  ee.vcmp.eq.s16 q6, q0, q3
  ee.orq q4, q4, q5
  ee.vcmp.lt.s16 q5, q0, q3
  ee.vldbc.16.ip q3, a13, 0 // load window_2_left
  ee.orq q5, q5, q6
  ee.vldbc.16.ip q7, a14, 0 // load window_2_right
  ee.andq q2, q4, q5
  ee.vcmp.eq.s16 q4, q0, q3
  ee.vcmp.gt.s16 q5, q0, q3
  ee.vcmp.eq.s16 q6, q0, q7
  ee.orq q4, q4, q5
  ee.vcmp.lt.s16 q5, q0, q7
  ee.vcmp.lt.s16 q5, q0, q7
.if \bgcol_window_enable
  ee.zero.q q0 // zero main_col
  ee.orq q5, q5, q6
  ee.vldbc.16.ip q1, a10, 0 // load sub_col
  ee.andq q3, q4, q5
  ee.vldbc.16.ip q4, a9, 0 // load main_col

  callx8 a4 // window calc
.else
  ee.vldbc.16.ip q0, a9, 0 // load main_col
  ee.orq q5, q5, q6
  ee.vldbc.16.ip q1, a10, 0 // load sub_col
  ee.andq q3, q4, q5
.endif

.if \bg0_enable
  // handle bg0
  handle_bg 0, bg0_\unique
.endif

.if \spr0_enable
  // handle spr0
  handle_spr_wrapper 0, spr0_\unique
.endif

.if \bg1_enable
  // handle bg1
  handle_bg 1, bg1_\unique
.endif

.if \spr1_enable
  // handle spr1
  handle_spr_wrapper 1, spr1_\unique
.endif

.if \cmath_enable
  // handle cmath
  jx  a7
.else
  no_cmath_colour_shift
  retw.n
.endif
.endm

  .text
  .literal_position
  .literal .LCmainscreen_colour, SASPPU_main_state_mainscreen_colour
  .literal .LCsubscreen_colour, SASPPU_main_state_subscreen_colour
  .literal .LCwindow_1_left, SASPPU_main_state_window_1_left
  .literal .LCwindow_1_right, SASPPU_main_state_window_1_right
  .literal .LCwindow_2_left, SASPPU_main_state_window_2_left
  .literal .LCwindow_2_right, SASPPU_main_state_window_2_right
  .literal .LCbgcol_windows, SASPPU_main_state_bgcol_windows
  .literal .LCmain_state_flags, SASPPU_main_state_flags
  .literal .LCcmath_fade, SASPPU_cmath_state_screen_fade
  .literal .LCcmath_flags, SASPPU_cmath_state_flags
  .literal .LCbg0_state, SASPPU_bg0_state
  .literal .LCbg1_state, SASPPU_bg1_state
  .literal .LCbg0, SASPPU_bg0
  .literal .LCbg1, SASPPU_bg1
  .literal .LCbackground, SASPPU_background
  .literal .LCsprites, SASPPU_sprites
  .literal .LCsprite_cache_1, SASPPU_sprite_cache 
  .literal .LCsprite_cache_2, SASPPU_sprite_cache + (SPRITE_CACHE * 4)
  .literal .LCsprite_cache_end, SASPPU_sprite_cache + (SPRITE_CACHE * 4 * 2)
  .literal .LCframe_buffer, tildagon_fb
  .literal .LCvector_increments, vector_increments
  .literal .LCcmath_bit, cmath_bit
  .literal .LCcmath_mask_low, cmath_mask_low
  .literal .LCcmath_mask_high, cmath_mask_high
  .literal .LCcmath_mask_split, cmath_mask_split
  .literal .LCcmath_one, cmath_one
  .literal .LCcmath_two, cmath_two
  .literal .LCcmath_six, cmath_six
  .literal .LCcmath_eleven, cmath_eleven

  .align 4
.include "sasppu_gen.inc"

  .text

  .literal .LCwindow_jump_table, window_jump_table
  .literal .LCsprite_jump_table, sprite_jump_table
  .literal .LCcmath_jump_table, cmath_jump_table
  .literal .LCper_pixel_jump_table, per_pixel_jump_table
  .literal .LCper_scanline, SASPPU_per_scanline

  .align 4
  .global SASPPU_render_scanline
  .type   SASPPU_render_scanline, @function
// void render_scanline( a0 return, a1 stack
  // uint8_t x, // a2
  // uint8_t y, // a3
//);
SASPPU_render_scanline:
  entry sp, 0+64

  l32r a4, .LCwindow_jump_table

  l32r a5, .LCbg0_state
  l8ui a5, a5, Background.windows
  addx4 a5, a5, a4
  l32i.n a13, a5, 0

  l32r a5, .LCbg1_state
  l8ui a5, a5, Background.windows
  addx4 a5, a5, a4
  l32i.n a14, a5, 0

  l32r a5, .LCbgcol_windows
  l8ui a5, a5, 0
  addx4 a5, a5, a4
  l32i.n a6, a5, 0

  l32r a4, .LCcmath_jump_table

  l32r a5, .LCcmath_flags
  l8ui a5, a5, 0
  addx4 a5, a5, a4
  l32i.n a15, a5, 0

  l32r a4, .LCper_pixel_jump_table

  l32r a5, .LCmain_state_flags
  l8ui a5, a5, 0
  addx4 a5, a5, a4
  l32i.n a7, a5, 0

  mov.n a11, a3

  movi a10, (240 - 8)
.LCper_pixel_loop:
  mov.n a12, a6
  callx8 a7
  
	slli  a4, a11, 4
	sub	  a4, a4, a11
	slli	a4, a4, 4
	l32r	a5, .LCframe_buffer
	add.n	a4, a4, a10
	addx2	a4, a4, a5
  st.qr q0, a4, 0

  addi.n a10, a10, -8
  bgez a10, .LCper_pixel_loop
  
  retw.n

	add.n	a4, a4, a10
	addx2	a4, a4, a5
  st.qr q0, a4, 0

  addi.n a10, a10, -8
  bgez a10, .LCper_pixel_loop
  
  retw.n
