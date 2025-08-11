/*
 * ili9341.h
 *
 *  Created on: Aug 10, 2025
 *      Author: ryan
 */

#ifndef INC_ILI9341_H_
#define INC_ILI9341_H_

#include "stm32u5xx_hal.h"
#include "main.h"
#include "fonts.h"

// --- 主要配置区 ---
// 在这里配置您的SPI接口和引脚
// 确保这里的定义与您在CubeMX中配置的别名(User Label)一致

// SPI接口句柄，CubeMX中会自动生成 hspi1
extern SPI_HandleTypeDef hspi1;
#define ILI9341_SPI_HANDLE  &hspi1

// CS (Chip Select) 引脚
#define ILI9341_CS_PORT     TFT_CS_GPIO_Port
#define ILI9341_CS_PIN      TFT_CS_Pin

// DC (Data/Command) 引脚
#define ILI9341_DC_PORT     TFT_DC_GPIO_Port
#define ILI9341_DC_PIN      TFT_DC_Pin

// RST (Reset) 引脚
#define ILI9341_RST_PORT    TFT_RST_GPIO_Port
#define ILI9341_RST_PIN     TFT_RST_Pin

// --- 显示屏基本属性 ---
#define ILI9341_WIDTH       240
#define ILI9341_HEIGHT      320

// --- 常用颜色定义 (16-bit, RGB565) ---
#define ILI9341_BLACK       0x0000
#define ILI9341_BLUE        0x001F
#define ILI9341_RED         0xF800
#define ILI9341_GREEN       0x07E0
#define ILI9341_CYAN        0x07FF
#define ILI9341_MAGENTA     0xF81F
#define ILI9341_YELLOW      0xFFE0
#define ILI9341_WHITE       0xFFFF

// --- 驱动函数声明 ---

// 初始化函数
void ILI9341_Init(void);

// 填充整个屏幕
void ILI9341_FillScreen(uint16_t color);

// 在指定坐标画一个点
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color);

// 绘制一个填充矩形
void ILI9341_FillRectangle_DMA(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

// 字符和字符串绘制函数
void ILI9341_DrawChar(uint16_t x, uint16_t y, char c, sFont* font, uint16_t color, uint16_t bgcolor, uint8_t size);
void ILI9341_DrawString(uint16_t x, uint16_t y, const char* str, sFont* font, uint16_t color, uint16_t bgcolor, uint8_t size);

#endif /* INC_ILI9341_H_ */
