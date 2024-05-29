/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdlib.h>
#include <dirent.h>
#include "esp_log.h"
#include "esp_err.h"
#include "esp_spiffs.h"
#include "bsp/esp-bsp.h"
#include "thorvg_capi.h"

static const char *TAG = "example";

static void capi_shape_example(bsp_lcd_handles_t *lcd_panel);
static void capi_lottie_example(bsp_lcd_handles_t *lcd_panel);
static void argb888_to_rgb565(const uint32_t *argb888, uint16_t *rgb565, size_t size);

#define LOTTIE_SIZE_HOR     300
#define LOTTIE_SIZE_VER     300

void app_main(void)
{
    bsp_lcd_handles_t lcd_panel;

    bsp_spiffs_mount();

    /* Initialize display */
    bsp_display_new_with_handles(NULL, &lcd_panel);

    bsp_display_backlight_on();

    capi_shape_example(&lcd_panel);

    vTaskDelay(pdMS_TO_TICKS(5000));

    capi_lottie_example(&lcd_panel);
}

static void capi_shape_example(bsp_lcd_handles_t *lcd_panel)
{
    ESP_LOGI(TAG, "capiShapes start");

    uint32_t *cavas_buf_888 = NULL;
    uint16_t *cavas_buf_565 = NULL;

    cavas_buf_888 = heap_caps_malloc(LOTTIE_SIZE_HOR * LOTTIE_SIZE_VER * sizeof(uint32_t), MALLOC_CAP_SPIRAM);
    assert(cavas_buf_888);

    cavas_buf_565 = heap_caps_malloc(LOTTIE_SIZE_HOR * LOTTIE_SIZE_VER * sizeof(uint16_t), MALLOC_CAP_SPIRAM);
    assert(cavas_buf_565);

    assert(tvg_engine_init(TVG_ENGINE_SW, 0) == TVG_RESULT_SUCCESS);

    Tvg_Canvas *canvas = tvg_swcanvas_create();
    assert(canvas);

    assert(tvg_swcanvas_set_target(canvas, cavas_buf_888, LOTTIE_SIZE_HOR, LOTTIE_SIZE_HOR, LOTTIE_SIZE_VER, TVG_COLORSPACE_ARGB8888) == TVG_RESULT_SUCCESS);

    Tvg_Paint *paint = tvg_shape_new();
    assert(paint);

    assert(tvg_shape_append_rect(paint, 0, 0, LOTTIE_SIZE_HOR, LOTTIE_SIZE_HOR, 0, 0) == TVG_RESULT_SUCCESS);
    assert(tvg_shape_set_fill_color(paint, 255, 0, 0, 255) == TVG_RESULT_SUCCESS);

    assert(tvg_canvas_push(canvas, paint) == TVG_RESULT_SUCCESS);

    assert(tvg_canvas_draw(canvas) == TVG_RESULT_SUCCESS);
    assert(tvg_canvas_sync(canvas) == TVG_RESULT_SUCCESS);

    argb888_to_rgb565(cavas_buf_888, cavas_buf_565, LOTTIE_SIZE_HOR * LOTTIE_SIZE_VER);
    esp_lcd_panel_draw_bitmap(lcd_panel->panel, 0, 0, LOTTIE_SIZE_HOR, LOTTIE_SIZE_VER, cavas_buf_565);

    assert(tvg_canvas_destroy(canvas) == TVG_RESULT_SUCCESS);

    assert(tvg_engine_term(TVG_ENGINE_SW) == TVG_RESULT_SUCCESS);

    if (cavas_buf_888) {
        free(cavas_buf_888);
    }

    if (cavas_buf_565) {
        free(cavas_buf_565);
    }
}

static void capi_lottie_example(bsp_lcd_handles_t *lcd_panel)
{
    ESP_LOGI(TAG, "capiLottie start");

    char *src = "/spiffs/lolo_walk.json";
    uint32_t *cavas_buf_888 = NULL;
    uint16_t *cavas_buf_565 = NULL;

    cavas_buf_888 = heap_caps_malloc(LOTTIE_SIZE_HOR * LOTTIE_SIZE_VER * sizeof(uint32_t), MALLOC_CAP_SPIRAM);
    assert(cavas_buf_888);

    cavas_buf_565 = heap_caps_malloc(LOTTIE_SIZE_HOR * LOTTIE_SIZE_VER * sizeof(uint16_t), MALLOC_CAP_SPIRAM);
    assert(cavas_buf_565);

    assert(tvg_engine_init(TVG_ENGINE_SW, 0) == TVG_RESULT_SUCCESS);

    Tvg_Animation *animation = tvg_animation_new();
    assert(animation);

    Tvg_Paint *picture = tvg_animation_get_picture(animation);
    assert(picture);

    assert(tvg_picture_load(picture, src) == TVG_RESULT_SUCCESS);

    assert(tvg_picture_set_size(picture, LOTTIE_SIZE_HOR, LOTTIE_SIZE_VER) == TVG_RESULT_SUCCESS);

    Tvg_Canvas *canvas = tvg_swcanvas_create();
    assert(canvas);

    assert(tvg_swcanvas_set_target(canvas, cavas_buf_888, LOTTIE_SIZE_HOR, LOTTIE_SIZE_HOR, LOTTIE_SIZE_VER, TVG_COLORSPACE_ARGB8888) == TVG_RESULT_SUCCESS);
    assert(tvg_canvas_push(canvas, picture) == TVG_RESULT_SUCCESS);

    float f_total;
    float f = 0;
    assert(tvg_animation_get_total_frame(animation, &f_total) == TVG_RESULT_SUCCESS);
    assert(f_total != 0.0f);

    while (f < f_total) {
        assert(tvg_animation_get_frame(animation, &f) == TVG_RESULT_SUCCESS);
        f++;
        ESP_LOGI(TAG, "set %f / %f", f, f_total);
        assert(tvg_animation_set_frame(animation, f) == TVG_RESULT_SUCCESS);
        assert(tvg_canvas_update(canvas) == TVG_RESULT_SUCCESS);

        assert(tvg_canvas_draw(canvas) == TVG_RESULT_SUCCESS);
        assert(tvg_canvas_sync(canvas) == TVG_RESULT_SUCCESS);

        argb888_to_rgb565(cavas_buf_888, cavas_buf_565, LOTTIE_SIZE_HOR * LOTTIE_SIZE_VER);
        esp_lcd_panel_draw_bitmap(lcd_panel->panel, 0, 0, LOTTIE_SIZE_HOR, LOTTIE_SIZE_VER, cavas_buf_565);

        vTaskDelay(pdMS_TO_TICKS(1));
    }

    assert(tvg_animation_del(animation) == TVG_RESULT_SUCCESS);

    assert(tvg_canvas_destroy(canvas) == TVG_RESULT_SUCCESS);

    assert(tvg_engine_term(TVG_ENGINE_SW) == TVG_RESULT_SUCCESS);

    if (cavas_buf_888) {
        free(cavas_buf_888);
    }

    if (cavas_buf_565) {
        free(cavas_buf_565);
    }
}

static void argb888_to_rgb565(const uint32_t *argb888, uint16_t *rgb565, size_t size)
{
    for (size_t i = 0; i < size; ++i) {
        uint32_t color = argb888[i];
        uint8_t r = (color >> 16) & 0xFF;
        uint8_t g = (color >> 8) & 0xFF;
        uint8_t b = color & 0xFF;

        uint16_t r_565 = (r >> 3) << 11;
        uint16_t g_565 = (g >> 2) << 5;
        uint16_t b_565 = b >> 3;

        rgb565[i] = r_565 | g_565 | b_565;
    }
}
