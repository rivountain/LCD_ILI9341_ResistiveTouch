#include "touchDriver.h"
#include "ili9341.h"

extern ADC_HandleTypeDef hadc1;

static void Set_Pin_Output(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

static void Set_Pin_Analog(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

static uint16_t Read_ADC_Channel(uint32_t channel) {
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = channel;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_68CYCLES;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) return 0;

    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK) return 0;
    uint16_t result = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    return result;
}

void Touch_Init(void) {
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
}

bool Touch_GetRaw_ADC(uint16_t* adc_x, uint16_t* adc_y)
{
// --- 读取物理Y轴 (对应屏幕的X轴) ---
	Set_Pin_Output(TOUCH_Y_PLUS_GPIO_Port, TOUCH_Y_PLUS_Pin);
	Set_Pin_Output(TOUCH_Y_MINUS_GPIO_Port, TOUCH_Y_MINUS_Pin);
	HAL_GPIO_WritePin(TOUCH_Y_PLUS_GPIO_Port, TOUCH_Y_PLUS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TOUCH_Y_MINUS_GPIO_Port, TOUCH_Y_MINUS_Pin, GPIO_PIN_RESET);
	Set_Pin_Analog(TOUCH_X_PLUS_GPIO_Port, TOUCH_X_PLUS_Pin);
	Set_Pin_Analog(TOUCH_X_MINUS_GPIO_Port, TOUCH_X_MINUS_Pin);
	HAL_Delay(1);
	*adc_x = Read_ADC_Channel(ADC_CHANNEL_7);

	// --- 读取物理X轴 (对应屏幕的Y轴) ---
	Set_Pin_Output(TOUCH_X_PLUS_GPIO_Port, TOUCH_X_PLUS_Pin);
	Set_Pin_Output(TOUCH_X_MINUS_GPIO_Port, TOUCH_X_MINUS_Pin);
	HAL_GPIO_WritePin(TOUCH_X_PLUS_GPIO_Port, TOUCH_X_PLUS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TOUCH_X_MINUS_GPIO_Port, TOUCH_X_MINUS_Pin, GPIO_PIN_RESET);
	Set_Pin_Analog(TOUCH_Y_PLUS_GPIO_Port, TOUCH_Y_PLUS_Pin);
	Set_Pin_Analog(TOUCH_Y_MINUS_GPIO_Port, TOUCH_Y_MINUS_Pin);
	HAL_Delay(1);
	*adc_y = Read_ADC_Channel(ADC_CHANNEL_8);

	if (*adc_x < 50 || *adc_y < 50) {
		return false;
	}
	return true;
}

bool Touch_GetCoordinates(uint16_t* x, uint16_t* y) {
    uint16_t adc_x, adc_y;

    if (!Touch_GetRaw_ADC(&adc_x, &adc_y)) {
        return false;
    }

    if (adc_x < PRESSURE_THRESHOLD || adc_y < PRESSURE_THRESHOLD) {
    	return false;
    }

    // 边界检查
    if (adc_x < ADC_MIN_X) adc_x = ADC_MIN_X;
    if (adc_x > ADC_MAX_X) adc_x = ADC_MAX_X;
    if (adc_y < ADC_MIN_Y) adc_y = ADC_MIN_Y;
    if (adc_y > ADC_MAX_Y) adc_y = ADC_MAX_Y;

    *x = ILI9341_WIDTH - (uint16_t)(((long)(adc_x - ADC_MIN_X) * ILI9341_WIDTH) / (ADC_MAX_X - ADC_MIN_X));
    *y = (uint16_t)(((long)(adc_y - ADC_MIN_Y) * ILI9341_HEIGHT) / (ADC_MAX_Y - ADC_MIN_Y));

    return true;
}
