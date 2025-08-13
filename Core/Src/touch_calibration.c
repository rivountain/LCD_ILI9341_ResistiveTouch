// 文件: Core/Src/touch_calibration.c (完全LVGL版)

#include "touch_calibration.h"
#include "lvgl.h"
#include "touchDriver.h"
#include "ili9341.h"
#include <stdio.h>
#include <stdbool.h>

// --- 校准状态机 ---
typedef enum {
    CAL_STATE_INIT,
    CAL_STATE_WAIT_PRESS_TL,  // 等待触摸左上角
    CAL_STATE_SAMPLING_TL,    // 正在采样左上角
    CAL_STATE_WAIT_RELEASE_TL,// 等待释放左上角
    CAL_STATE_WAIT_PRESS_BR,  // 等待触摸右下角
    CAL_STATE_SAMPLING_BR,    // 正在采样右下角
    CAL_STATE_WAIT_RELEASE_BR,// 等待释放右下角
    CAL_STATE_SHOW_RESULTS    // 显示结果
} calibration_state_t;

// --- 全局变量 ---
static calibration_state_t cal_state = CAL_STATE_INIT;
static lv_obj_t * main_label;
static lv_obj_t * point_label;
static lv_obj_t * adc_x_label;
static lv_obj_t * adc_y_label;
static lv_obj_t * target_obj;
static uint16_t tl_x, tl_y, br_x, br_y; // 存储校准结果

// --- LVGL定时器回调函数 (校准流程的核心) ---
static void calibration_timer_cb(lv_timer_t * timer)
{
    uint16_t raw_x, raw_y;
    bool is_touched = Touch_GetRaw_ADC(&raw_x, &raw_y);
    char buf[50];

    // 实时更新ADC读数
	if (is_touched) {
		sprintf(buf, "ADC X: %d", raw_x);
		lv_label_set_text(adc_x_label, buf);
		sprintf(buf, "ADC Y: %d", raw_y);
		lv_label_set_text(adc_y_label, buf);
	} else {
		lv_label_set_text(adc_x_label, "ADC X: ----");
		lv_label_set_text(adc_y_label, "ADC Y: ----");
	}

	// 校准状态机
	switch (cal_state)
	{
		case CAL_STATE_INIT:
			lv_label_set_text(main_label, "Calibration started.\nPlease press and HOLD the target.");
			lv_obj_align(target_obj, LV_ALIGN_TOP_LEFT, 0, 0);
			lv_obj_clear_flag(target_obj, LV_OBJ_FLAG_HIDDEN);
			cal_state = CAL_STATE_WAIT_PRESS_TL;
			break;

		case CAL_STATE_WAIT_PRESS_TL:
			if (is_touched) cal_state = CAL_STATE_SAMPLING_TL;
			break;

		case CAL_STATE_SAMPLING_TL:
		{
			#define NUM_SAMPLES 16 // 定义采样次数
			uint32_t total_x = 0, total_y = 0;
			uint8_t samples_count = 0;
			for(int i = 0; i < NUM_SAMPLES; i++) {
				// 确保在采样期间一直有触摸
				if (Touch_GetRaw_ADC(&raw_x, &raw_y)) {
					total_x += raw_x;
					total_y += raw_y;
					samples_count++;
				} else {
					// 如果中途松手，则重新开始等待触摸
					lv_label_set_text(main_label, "Touch lost! Please press\nand HOLD the target again.");
					lv_label_set_text(point_label, "");
					cal_state = CAL_STATE_WAIT_PRESS_TL;
					return; // 提前退出本次回调
				}
				HAL_Delay(10); // 每次采样之间稍作延时
			}
			// 计算平均值并存储
			if (samples_count > 0) {
				tl_x = total_x / samples_count;
				tl_y = total_y / samples_count;
			}
			lv_label_set_text(point_label, "OK. Now RELEASE the screen.");
			cal_state = CAL_STATE_WAIT_RELEASE_TL;
			break;
		}

		case CAL_STATE_WAIT_RELEASE_TL:
			if (!is_touched) {
				HAL_Delay(200); // 延时消抖
				cal_state = CAL_STATE_WAIT_PRESS_BR;
				lv_label_set_text(main_label, "Good. Now press and HOLD\nthe BOTTOM-RIGHT target.");
				lv_label_set_text(point_label, "");
				lv_obj_align(target_obj, LV_ALIGN_BOTTOM_RIGHT, -0, -0);
			}
			break;

		case CAL_STATE_WAIT_PRESS_BR:
			if (is_touched) cal_state = CAL_STATE_SAMPLING_BR;
			break;

		case CAL_STATE_SAMPLING_BR:
		{
			#define NUM_SAMPLES 16
            uint32_t total_x = 0, total_y = 0;
            uint8_t samples_count = 0;

            for(int i = 0; i < NUM_SAMPLES; i++) {
                if (Touch_GetRaw_ADC(&raw_x, &raw_y)) {
                    total_x += raw_x;
                    total_y += raw_y;
                    samples_count++;
                } else {
                    lv_label_set_text(main_label, "Touch lost! Please press\nand HOLD the target again.");
                    lv_label_set_text(point_label, "");
                    cal_state = CAL_STATE_WAIT_PRESS_BR;
                    return;
                }
                HAL_Delay(10);
            }

            if (samples_count > 0) {
                br_x = total_x / samples_count;
                br_y = total_y / samples_count;
            }
			lv_label_set_text(point_label, "OK. Now RELEASE the screen.");
			cal_state = CAL_STATE_WAIT_RELEASE_BR;
			break;
		}

		case CAL_STATE_WAIT_RELEASE_BR:
			if (!is_touched) {
				HAL_Delay(200);
				cal_state = CAL_STATE_SHOW_RESULTS;
				lv_obj_add_flag(target_obj, LV_OBJ_FLAG_HIDDEN); // 隐藏目标点
			}
			break;

		case CAL_STATE_SHOW_RESULTS:
		{
			uint16_t x_min = (tl_x < br_x) ? tl_x : br_x;
			uint16_t x_max = (tl_x > br_x) ? tl_x : br_x;
			uint16_t y_min = (tl_y < br_y) ? tl_y : br_y;
			uint16_t y_max = (tl_y > br_y) ? tl_y : br_y;

			lv_label_set_text(main_label, "Calibration Done!");
			lv_obj_set_style_text_color(main_label, lv_color_hex(0x00FFFF), 0); // Cyan

			lv_label_set_text(point_label, "Update touch_adc.h with:");

			sprintf(buf, "#define ADC_MIN_X %d", x_min);
			lv_label_set_text(adc_x_label, buf);

			sprintf(buf, "#define ADC_MAX_X %d", x_max);
			// 我们复用adc_y_label来显示第二行结果
			lv_obj_t* x_max_label = lv_label_create(lv_scr_act());
			lv_obj_align_to(x_max_label, adc_x_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
			lv_label_set_text(x_max_label, buf);

			sprintf(buf, "#define ADC_MIN_Y %d", y_min);
			lv_obj_t* y_min_label = lv_label_create(lv_scr_act());
			lv_obj_align_to(y_min_label, x_max_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
			lv_label_set_text(y_min_label, buf);

			sprintf(buf, "#define ADC_MAX_Y %d", y_max);
			lv_obj_t* y_max_label = lv_label_create(lv_scr_act());
			lv_obj_align_to(y_max_label, y_min_label, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 5);
			lv_label_set_text(y_max_label, buf);

			lv_label_set_text(adc_y_label, ""); // 清空原来的标签

			lv_timer_del(timer); // 删除定时器，停止流程
			break;
		}
	}
}

// --- 公开函数 ---
void Touch_Calibrate(void)
{
    // 清空当前屏幕
    lv_obj_clean(lv_scr_act());

    // 创建主提示标签
    main_label = lv_label_create(lv_scr_act());
    lv_obj_align(main_label, LV_ALIGN_TOP_MID, 0, 20);
    lv_label_set_long_mode(main_label, LV_LABEL_LONG_WRAP);
    lv_obj_set_width(main_label, ILI9341_WIDTH - 20);
    lv_obj_set_style_text_align(main_label, LV_TEXT_ALIGN_CENTER, 0);

    // 创建第二个提示标签
	point_label = lv_label_create(lv_scr_act());
	lv_obj_align(point_label, LV_ALIGN_TOP_MID, 0, 80);

	// 创建实时ADC读数标签
	adc_x_label = lv_label_create(lv_scr_act());
	lv_obj_align(adc_x_label, LV_ALIGN_CENTER, 0, 20);
	adc_y_label = lv_label_create(lv_scr_act());
	lv_obj_align(adc_y_label, LV_ALIGN_CENTER, 0, 40);

	// 创建一个红色的目标点
	target_obj = lv_obj_create(lv_scr_act());
	lv_obj_set_size(target_obj, 10, 10);
	lv_obj_set_style_bg_color(target_obj, lv_color_hex(0xFF0000), 0);
	lv_obj_set_style_border_width(target_obj, 0, 0);
	lv_obj_add_flag(target_obj, LV_OBJ_FLAG_HIDDEN); // 初始隐藏

	// 创建一个周期性定时器来驱动校准流程
	lv_timer_create(calibration_timer_cb, 50, NULL); // 每50ms检查一次
}
