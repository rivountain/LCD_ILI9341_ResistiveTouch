#ifndef INC_ILI9341_H_
#define INC_ILI9341_H_

#include "stm32u5xx_hal.h"
#include "main.h"
#include "lvgl.h"

// SPI接口句柄，CubeMX中会自动生成 hspi1
extern SPI_HandleTypeDef hspi1;
#define ILI9341_SPI_HANDLE  &hspi1

// CS (Chip Select)
#define ILI9341_CS_PORT     TFT_CS_GPIO_Port
#define ILI9341_CS_PIN      TFT_CS_Pin

// DC (Data/Command)
#define ILI9341_DC_PORT     TFT_DC_GPIO_Port
#define ILI9341_DC_PIN      TFT_DC_Pin

// RST (Reset)
#define ILI9341_RST_PORT    TFT_RST_GPIO_Port
#define ILI9341_RST_PIN     TFT_RST_Pin

//#define ILI9341_WIDTH       240
//#define ILI9341_HEIGHT      320
// landscape
#define ILI9341_WIDTH       320
#define ILI9341_HEIGHT      240

// 16-bit RGB565
#define ILI9341_BLACK       0x0000
#define ILI9341_BLUE        0x001F
#define ILI9341_RED         0xF800
#define ILI9341_GREEN       0x07E0
#define ILI9341_CYAN        0x07FF
#define ILI9341_MAGENTA     0xF81F
#define ILI9341_YELLOW      0xFFE0
#define ILI9341_WHITE       0xFFFF

void ILI9341_Init(void);
void ILI9341_Flush_Callback(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);

#endif /* INC_ILI9341_H_ */
