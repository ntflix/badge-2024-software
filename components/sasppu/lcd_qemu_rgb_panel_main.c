/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_qemu_rgb.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "hal/cpu_hal.h"
#include "sasppu.h"
#include "sasppu_help.h"
static const
#include "kodim05.png.h"
    static const
#include "sprites.png.h"

    static const char *TAG = "example";

#define CURRENT_COLOR_DEPTH RGB_QEMU_BPP_16

// The pixel number in horizontal and vertical
#define EXAMPLE_LCD_H_RES 240
#define EXAMPLE_LCD_V_RES 240

void app_main(void)
{
    ESP_LOGI(TAG, "Install RGB LCD panel driver");
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_rgb_qemu_config_t panel_config = {
        .width = EXAMPLE_LCD_H_RES,
        .height = EXAMPLE_LCD_V_RES,
        .bpp = CURRENT_COLOR_DEPTH,
    };
    ESP_ERROR_CHECK(esp_lcd_new_rgb_qemu(&panel_config, &panel_handle));

    ESP_LOGI(TAG, "Initialize RGB LCD panel");
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

    void *buf1;

    ESP_ERROR_CHECK(esp_lcd_rgb_qemu_get_frame_buffer(panel_handle, &buf1));

    SASPPU_main_state_mainscreen_colour = 0b1111110000000000;
    SASPPU_main_state_subscreen_colour = 0b0000001111100000;
    SASPPU_main_state_flags = MAIN_CMATH_ENABLE | MAIN_BGCOL_WINDOW_ENABLE | MAIN_BG0_ENABLE | MAIN_SPR0_ENABLE;
    SASPPU_cmath_state_flags = CMATH_CMATH_ENABLE | CMATH_ADD_SUB_SCREEN | CMATH_FADE_ENABLE;
    SASPPU_main_state_bgcol_windows = WINDOW_A | WINDOW_B;

    SASPPU_main_state_window_1_left = 10;
    SASPPU_main_state_window_1_right = 200;
    SASPPU_main_state_window_2_left = 180;
    SASPPU_main_state_window_2_right = 230;

    memcpy(SASPPU_background, kodim05_small_png_bgraw, kodim05_small_png_bgraw_len);
    memcpy(SASPPU_sprites, sprites_png_bgraw, sprites_png_bgraw_len);

    for (int i = 0; i < 64 * 64; i++)
    {
        if ((i % 64) >= 32)
        {
            if (((i / 64) % 64) >= 32)
            {
                SASPPU_bg0[i] = ((31 - (i % 32)) * 8 + ((31 - ((i / 64) % 32)) * 256 * 8)) | 0b11;
            }
            else
            {
                SASPPU_bg0[i] = ((31 - (i % 32)) * 8 + (((i / 64) % 32) * 256 * 8)) | 0b01;
            }
        }
        else
        {
            if (((i / 64) % 64) >= 32)
            {
                SASPPU_bg0[i] = ((i % 32) * 8 + ((31 - ((i / 64) % 32)) * 256 * 8)) | 0b10;
            }
            else
            {
                SASPPU_bg0[i] = ((i % 32) * 8 + (((i / 64) % 32) * 256 * 8)) | 0b00;
            }
        }
    }

    for (int i = 0; i < 32; i++)
    {
        SASPPU_oam[i].width = 32;
        SASPPU_oam[i].height = 32;
        SASPPU_oam[i].graphics_x = (i % 8) * 32;
        SASPPU_oam[i].graphics_y = 0;
        SASPPU_oam[i].windows = 0x0F;
        SASPPU_oam[i].flags = SPR_ENABLED | SPR_DOUBLE | SPR_FLIP_Y | SPR_FLIP_X;
    }

    SASPPU_bg0_state.windows = 0x0F;
    SASPPU_bg1_state.windows = 0x0F;

    for (uint32_t i = 0;; i++)
    {
        SASPPU_cmath_state_screen_fade = (i * 8) % 256;
        if (i >= (256 / 8))
        {
            SASPPU_cmath_state_flags = CMATH_CMATH_ENABLE | CMATH_ADD_SUB_SCREEN;
        }

        // SASPPU_oam[0].x = (i / 4) % 240;
        // SASPPU_oam[0].y = (i / 3) % 240;

        for (int spr = 0; spr < 32; spr++)
        {
            SASPPU_oam[spr].x = (uint8_t)((sinf((float)(i) / (10.0f + (float)spr)) * ((240 - 16) / 2.0)) + 120.0);
            SASPPU_oam[spr].y = (uint8_t)((cosf((float)(i) / (13.0f + (float)spr)) * ((240 - 16) / 2.0)) + 120.0);
        }

        SASPPU_bg0_state.x = i;
        SASPPU_bg0_state.y = i / 2;

        ESP_LOGI(TAG, "Render frame %lu", i);
        uint64_t prev = cpu_hal_get_cycle_count();
        SASPPU_render();
        uint64_t next = cpu_hal_get_cycle_count();
        ESP_LOGI(TAG, "Frame cycle count: %llu", next - prev);
        memcpy(buf1, &SASPPU_frame_buffer, sizeof(SASPPU_frame_buffer));
        esp_lcd_rgb_qemu_refresh(panel_handle);
    }
}
