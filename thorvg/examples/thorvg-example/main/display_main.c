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
#include "thorvg_capi.h"

static const char *TAG = "example";

static void init_filesystem(void);
static void capi_shape_example();
static void capi_lottie_example();

#define LOTTIE_SIZE_HOR     300
#define LOTTIE_SIZE_VER     300

void app_main(void)
{
    init_filesystem();

    capi_shape_example();

    capi_lottie_example();
}

static void init_filesystem(void)
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/lottie",
        .partition_label = "lottie",
        .max_files = 1,
    };

    ESP_ERROR_CHECK(esp_vfs_spiffs_register(&conf));

    /* Get file name in storage */
    struct dirent *p_dirent = NULL;
    DIR *p_dir_stream = opendir("/lottie");

    /* Scan files in storage */
    while (true) {
        p_dirent = readdir(p_dir_stream);
        if (NULL != p_dirent) {
            ESP_LOGI(TAG, "d_name:%s", p_dirent->d_name);
        } else {
            closedir(p_dir_stream);
            break;
        }
    }
}

static void capi_shape_example()
{
    ESP_LOGI(TAG, "capiShapes start");

    uint32_t *lottile_cavas_888 = NULL;

    lottile_cavas_888 = heap_caps_malloc(LOTTIE_SIZE_HOR * LOTTIE_SIZE_VER * sizeof(uint32_t), MALLOC_CAP_SPIRAM);
    assert(lottile_cavas_888);

    assert(tvg_engine_init(TVG_ENGINE_SW, 0) == TVG_RESULT_SUCCESS);

    Tvg_Canvas *canvas = tvg_swcanvas_create();
    assert(canvas);

    assert(tvg_swcanvas_set_target(canvas, lottile_cavas_888, LOTTIE_SIZE_HOR, LOTTIE_SIZE_HOR, LOTTIE_SIZE_VER, TVG_COLORSPACE_ARGB8888) == TVG_RESULT_SUCCESS);

    Tvg_Paint *paint = tvg_shape_new();
    assert(paint);

    assert(tvg_shape_append_rect(paint, 0, 0, LOTTIE_SIZE_HOR, LOTTIE_SIZE_HOR, 0, 0) == TVG_RESULT_SUCCESS);
    assert(tvg_shape_set_fill_color(paint, 255, 0, 0, 255) == TVG_RESULT_SUCCESS);

    assert(tvg_canvas_push(canvas, paint) == TVG_RESULT_SUCCESS);

    assert(tvg_canvas_draw(canvas) == TVG_RESULT_SUCCESS);
    assert(tvg_canvas_sync(canvas) == TVG_RESULT_SUCCESS);

    assert(tvg_canvas_destroy(canvas) == TVG_RESULT_SUCCESS);

    assert(tvg_engine_term(TVG_ENGINE_SW) == TVG_RESULT_SUCCESS);

    if (lottile_cavas_888) {
        free(lottile_cavas_888);
    }
}

static void capi_lottie_example()
{
    ESP_LOGI(TAG, "capiLottie start");

    char *src = "/lottie/lolo_walk.json";
    uint32_t *lottile_cavas_888 = NULL;

    lottile_cavas_888 = heap_caps_malloc(LOTTIE_SIZE_HOR * LOTTIE_SIZE_VER * sizeof(uint32_t), MALLOC_CAP_SPIRAM);
    assert(lottile_cavas_888);

    assert(tvg_engine_init(TVG_ENGINE_SW, 0) == TVG_RESULT_SUCCESS);

    Tvg_Animation *animation = tvg_animation_new();
    assert(animation);

    Tvg_Paint *picture = tvg_animation_get_picture(animation);
    assert(picture);

    assert(tvg_picture_load(picture, src) == TVG_RESULT_SUCCESS);

    assert(tvg_picture_set_size(picture, LOTTIE_SIZE_HOR, LOTTIE_SIZE_VER) == TVG_RESULT_SUCCESS);

    Tvg_Canvas *canvas = tvg_swcanvas_create();
    assert(canvas);

    assert(tvg_swcanvas_set_target(canvas, lottile_cavas_888, LOTTIE_SIZE_HOR, LOTTIE_SIZE_HOR, LOTTIE_SIZE_VER, TVG_COLORSPACE_ARGB8888) == TVG_RESULT_SUCCESS);
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

        vTaskDelay(pdMS_TO_TICKS(1));
    }

    assert(tvg_animation_del(animation) == TVG_RESULT_SUCCESS);

    assert(tvg_canvas_destroy(canvas) == TVG_RESULT_SUCCESS);

    assert(tvg_engine_term(TVG_ENGINE_SW) == TVG_RESULT_SUCCESS);

    if (lottile_cavas_888) {
        free(lottile_cavas_888);
    }
}
