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
#include "esp_check.h"
#include "esp_lcd_panel_ops.h"
#include "bsp/esp-bsp.h"
#include "driver/ppa.h"
#include "thorvg_capi.h"

static const char *TAG = "example";

static void capi_loop_task(void *arg);
static esp_err_t capi_create_lottie(bsp_lcd_handles_t *lcd_panel);
static esp_err_t argb888_to_rgb565_ppa(ppa_client_handle_t ppa_handle, const uint32_t *in, uint16_t *out);

#define LOTTIE_SIZE_HOR     320
#define LOTTIE_SIZE_VER     320

void app_main(void)
{
    static bsp_lcd_handles_t lcd_panel;

    bsp_spiffs_mount();

    /* Initialize display */
    bsp_display_new_with_handles(NULL, &lcd_panel);

    bsp_display_backlight_on();

    BaseType_t res = xTaskCreate(capi_loop_task, "thorvg task", 60 * 1024, (void *)&lcd_panel, 5, NULL);
    if (res != pdPASS) {
        ESP_LOGE(TAG, "Create thorvg task fail!");
    }
}

void capi_loop_task(void *arg)
{
    bsp_lcd_handles_t *lcd_panel = (bsp_lcd_handles_t *)arg;

    while (1) {
        capi_create_lottie(lcd_panel);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }

    vTaskDelete(NULL);
}

static esp_err_t capi_create_lottie(bsp_lcd_handles_t *lcd_panel)
{
    esp_err_t ret = ESP_OK;
    Tvg_Result tvg_res = TVG_RESULT_SUCCESS;
    Tvg_Result tvg_engine = TVG_RESULT_INSUFFICIENT_CONDITION;

    static uint32_t reac_color = 0x00;

    char *src = "/spiffs/lolo_walk.json";

    uint32_t *canvas_buf_888 = NULL;
    uint16_t *canvas_buf_565 = NULL;
    ppa_client_handle_t ppa_handle = NULL;

    Tvg_Animation *animation = NULL;
    Tvg_Canvas *canvas = NULL;

    canvas_buf_888 = heap_caps_aligned_calloc(64, LOTTIE_SIZE_HOR * LOTTIE_SIZE_VER * sizeof(uint32_t), sizeof(uint8_t), MALLOC_CAP_SPIRAM);
    ESP_GOTO_ON_FALSE(canvas_buf_888, ESP_ERR_NO_MEM, err, TAG, "Error malloc canvas buffer");

    canvas_buf_565 = heap_caps_aligned_calloc(64, LOTTIE_SIZE_HOR * LOTTIE_SIZE_VER * sizeof(uint16_t), sizeof(uint8_t), MALLOC_CAP_SPIRAM);
    ESP_GOTO_ON_FALSE(canvas_buf_565, ESP_ERR_NO_MEM, err, TAG, "Error malloc canvas buffer");

    ppa_client_config_t ppa_client_config = {
        .oper_type = PPA_OPERATION_SRM,
    };
    ESP_GOTO_ON_FALSE((ESP_OK == ppa_register_client(&ppa_client_config, &ppa_handle)), ESP_ERR_INVALID_STATE, err, TAG, "ppa_register_client failed");

    tvg_engine = tvg_engine_init(TVG_ENGINE_SW, 0);
    ESP_GOTO_ON_FALSE(tvg_engine == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_engine_init failed");

    canvas = tvg_swcanvas_create();
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_engine_init failed");

    tvg_res = tvg_swcanvas_set_target(canvas, canvas_buf_888, LOTTIE_SIZE_HOR, LOTTIE_SIZE_HOR, LOTTIE_SIZE_VER, TVG_COLORSPACE_ARGB8888);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_engine_init failed");

    /* shape rect */
    Tvg_Paint *paint = tvg_shape_new();
    ESP_GOTO_ON_FALSE(paint, ESP_ERR_INVALID_STATE, err, TAG, "tvg_shape_new failed");

    tvg_res = tvg_shape_append_rect(paint, 0, 0, LOTTIE_SIZE_HOR, LOTTIE_SIZE_HOR, 0, 0);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_shape_append_rect failed");

    reac_color++;
    if (reac_color % 2) {
        tvg_res = tvg_shape_set_fill_color(paint, 255, 0, 0, 255);
    } else {
        tvg_res = tvg_shape_set_fill_color(paint, 0, 255, 0, 255);
    }
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_shape_set_fill_color failed");

    tvg_res = tvg_canvas_push(canvas, paint);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_canvas_push failed");

    tvg_res = tvg_canvas_draw(canvas);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_canvas_draw failed");

    tvg_res = tvg_canvas_sync(canvas);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_canvas_sync failed");

    argb888_to_rgb565_ppa(ppa_handle, canvas_buf_888, canvas_buf_565);
    esp_lcd_panel_draw_bitmap(lcd_panel->panel, 0, 0, LOTTIE_SIZE_HOR, LOTTIE_SIZE_VER, canvas_buf_565);

    /* tvg Lottie */
    animation = tvg_animation_new();
    ESP_GOTO_ON_FALSE(animation, ESP_ERR_INVALID_STATE, err, TAG, "tvg_engine_init failed");

    Tvg_Paint *picture = tvg_animation_get_picture(animation);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_engine_init failed");

    tvg_res = tvg_picture_load(picture, src);
    ESP_GOTO_ON_FALSE(picture, ESP_ERR_INVALID_STATE, err, TAG, "tvg_engine_init failed");

    tvg_res = tvg_picture_set_size(picture, LOTTIE_SIZE_HOR, LOTTIE_SIZE_VER);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_engine_init failed");

    tvg_res = tvg_canvas_push(canvas, picture);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_engine_init failed");

    float f_total;
    float f = 0;
    tvg_res = tvg_animation_get_total_frame(animation, &f_total);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_engine_init failed");
    ESP_GOTO_ON_FALSE((f_total != 0.0f), ESP_ERR_INVALID_STATE, err, TAG, "tvg_engine_init failed");

    while (f < f_total) {
        tvg_res = tvg_animation_get_frame(animation, &f);
        f++;
        ESP_LOGI(TAG, "set %f / %f", f, f_total);
        tvg_res = tvg_animation_set_frame(animation, f);
        ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_animation_set_frame failed");

        tvg_res = tvg_canvas_update(canvas);
        ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_canvas_update failed");

        tvg_res = tvg_canvas_draw(canvas);
        ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_canvas_draw failed");

        tvg_res = tvg_canvas_sync(canvas);
        ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_canvas_sync failed");

        argb888_to_rgb565_ppa(ppa_handle, canvas_buf_888, canvas_buf_565);
        esp_lcd_panel_draw_bitmap(lcd_panel->panel, 0, 0, LOTTIE_SIZE_HOR, LOTTIE_SIZE_VER, canvas_buf_565);

        vTaskDelay(pdMS_TO_TICKS(1));
    }

err:
    if (animation) {
        tvg_animation_del(animation);
    }
    if (canvas) {
        tvg_canvas_destroy(canvas);
    }
    if (TVG_RESULT_SUCCESS == tvg_engine) {
        tvg_engine_term(TVG_ENGINE_SW);
    }

    if (canvas_buf_888) {
        free(canvas_buf_888);
    }
    if (canvas_buf_565) {
        free(canvas_buf_565);
    }
    if (ppa_handle) {
        ppa_unregister_client(ppa_handle);
    }

    return ret;
}

static esp_err_t argb888_to_rgb565_ppa(ppa_client_handle_t ppa_handle, const uint32_t *in, uint16_t *out)
{
    ppa_srm_oper_config_t oper_config = {
        .in.buffer = in,
        .in.pic_w = LOTTIE_SIZE_HOR,
        .in.pic_h = LOTTIE_SIZE_VER,
        .in.block_w = LOTTIE_SIZE_HOR,
        .in.block_h = LOTTIE_SIZE_VER,
        .in.block_offset_x = 0,
        .in.block_offset_y = 0,
        .in.srm_cm = PPA_SRM_COLOR_MODE_ARGB8888,

        .out.buffer = out,
        .out.buffer_size = LOTTIE_SIZE_HOR * LOTTIE_SIZE_VER * sizeof(uint16_t),
            .out.pic_w = LOTTIE_SIZE_HOR,
            .out.pic_h = LOTTIE_SIZE_VER,
            .out.block_offset_x = 0,
            .out.block_offset_y = 0,
            .out.srm_cm = PPA_SRM_COLOR_MODE_RGB565,

            .rotation_angle = PPA_SRM_ROTATION_ANGLE_0,
            .scale_x = 1.0,
            .scale_y = 1.0,

            .rgb_swap = 0,
            .byte_swap = 0,
            .mode = PPA_TRANS_MODE_BLOCKING,
    };

    ESP_ERROR_CHECK(ppa_do_scale_rotate_mirror(ppa_handle, &oper_config));

    return ESP_OK;
}
