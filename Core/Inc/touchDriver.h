#ifndef INC_TOUCHDRIVER_H_
#define INC_TOUCHDRIVER_H_

#include "stm32u5xx_hal.h"
#include <stdbool.h>

// --- 触摸屏校准值 ---
// 我们将通过校准程序来获得这些精确值
#define ADC_MIN_X      555
#define ADC_MAX_X      3834
#define ADC_MIN_Y      573
#define ADC_MAX_Y      3667
#define PRESSURE_THRESHOLD 650

// --- 驱动函数声明 ---
void Touch_Init(void);

// 获取经过校准的坐标
bool Touch_GetCoordinates(uint16_t* x, uint16_t* y);

// 获取原始ADC读数，供校准程序使用
bool Touch_GetRaw_ADC(uint16_t* adc_x, uint16_t* adc_y);

#endif /* INC_TOUCHDRIVER_H_ */
