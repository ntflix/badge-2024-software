#ifndef SASPPU_HELP_H_
#define SASPPU_HELP_H_

#include "sasppu.h"

void SASPPU_copy_sprite(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t *data);
void SASPPU_copy_sprite_transparent(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t *data);
void SASPPU_fill_sprite(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t colour);

void SASPPU_copy_background(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t *data);
void SASPPU_copy_background_transparent(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t *data);
void SASPPU_fill_background(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t colour);

void SASPPU_gfx_reset();

#endif // SASPPU_HELP_H_
