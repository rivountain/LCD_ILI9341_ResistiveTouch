#include "ili9341.h"

// Send Single Byte
static void ILI9341_SPI_Tx(uint8_t data) {
    HAL_SPI_Transmit(ILI9341_SPI_HANDLE, &data, 1, HAL_MAX_DELAY);
}

// Write Command
static void ILI9341_WriteCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_RESET); // DC LOW for CMD
    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_RESET); // CS LOW
    ILI9341_SPI_Tx(cmd);
    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_SET);   // CS HIGH
}

// Write Data
static void ILI9341_WriteData(uint8_t data) {
    HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_SET);   // DC HIGH for DATA
    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_RESET); // CS LOW
    ILI9341_SPI_Tx(data);
    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_SET);   // CS HIGH
}

// Reset
static void ILI9341_Reset(void) {
    HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(100);
}

// Set Address Window
void ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // Column Address Set
    ILI9341_WriteCommand(0x2A);
    ILI9341_WriteData(x0 >> 8);
    ILI9341_WriteData(x0 & 0xFF);
    ILI9341_WriteData(x1 >> 8);
    ILI9341_WriteData(x1 & 0xFF);

    // Page Address Set
    ILI9341_WriteCommand(0x2B);
    ILI9341_WriteData(y0 >> 8);
    ILI9341_WriteData(y0 & 0xFF);
    ILI9341_WriteData(y1 >> 8);
    ILI9341_WriteData(y1 & 0xFF);

    // Memory Write
    ILI9341_WriteCommand(0x2C);
}

void ILI9341_Init(void) {
	ILI9341_Reset();

	    ILI9341_WriteCommand(0x01); // Software Reset
	    HAL_Delay(120);

	    ILI9341_WriteCommand(0xEF);
	    ILI9341_WriteData(0x03);
	    ILI9341_WriteData(0x80);
	    ILI9341_WriteData(0x02);

	    ILI9341_WriteCommand(0xCF);
	    ILI9341_WriteData(0x00);
	    ILI9341_WriteData(0xC1);
	    ILI9341_WriteData(0x30);

	    ILI9341_WriteCommand(0xED);
	    ILI9341_WriteData(0x64);
	    ILI9341_WriteData(0x03);
	    ILI9341_WriteData(0x12);
	    ILI9341_WriteData(0x81);

	    ILI9341_WriteCommand(0xE8);
	    ILI9341_WriteData(0x85);
	    ILI9341_WriteData(0x00);
	    ILI9341_WriteData(0x78);

	    ILI9341_WriteCommand(0xCB);
	    ILI9341_WriteData(0x39);
	    ILI9341_WriteData(0x2C);
	    ILI9341_WriteData(0x00);
	    ILI9341_WriteData(0x34);
	    ILI9341_WriteData(0x02);

	    ILI9341_WriteCommand(0xF7);
	    ILI9341_WriteData(0x20);

	    ILI9341_WriteCommand(0xEA);
	    ILI9341_WriteData(0x00);
	    ILI9341_WriteData(0x00);

	    ILI9341_WriteCommand(0xC0);    //Power control
	    ILI9341_WriteData(0x23);   //VRH[5:0]

	    ILI9341_WriteCommand(0xC1);    //Power control
	    ILI9341_WriteData(0x10);   //SAP[2:0];BT[3:0]

	    ILI9341_WriteCommand(0xC5);    //VCM control
	    ILI9341_WriteData(0x3e);
	    ILI9341_WriteData(0x28);

	    ILI9341_WriteCommand(0xC7);    //VCM control2
	    ILI9341_WriteData(0x86);

	    ILI9341_WriteCommand(0x36);    // Memory Access Control
	    ILI9341_WriteData(0x28);

	    ILI9341_WriteCommand(0x3A);    // Pixel Format Set
	    ILI9341_WriteData(0x55);

	    ILI9341_WriteCommand(0xB1);    // Frame Rate Control
	    ILI9341_WriteData(0x00);
	    ILI9341_WriteData(0x18);

	    ILI9341_WriteCommand(0xB6);    // Display Function Control
	    ILI9341_WriteData(0x08);
	    ILI9341_WriteData(0x82);
	    ILI9341_WriteData(0x27);

	    ILI9341_WriteCommand(0xF2);    // 3Gamma Function Disable
	    ILI9341_WriteData(0x00);

	    ILI9341_WriteCommand(0x26);    //Gamma curve selected
	    ILI9341_WriteData(0x01);

	    // *** 新增的、关键的修复代码 ***
	    ILI9341_WriteCommand(0x20); // Display Inversion OFF

	    ILI9341_WriteCommand(0x11); // Exit Sleep
	    HAL_Delay(120);

	    ILI9341_WriteCommand(0x29); // Display ON
}

// 用于在中断回调中通知LVGL的显示驱动指针
static lv_disp_drv_t *g_lvgl_disp_drv;

void ILI9341_Flush_Callback(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    // 1. 保存驱动指针，以便在DMA中断回调中使用
    g_lvgl_disp_drv = disp_drv;

    // 2. 设置硬件绘图窗口
    ILI9341_SetAddressWindow(area->x1, area->y1, area->x2, area->y2);

    // 3. 计算需要传输的数据总字节数
    uint32_t size = lv_area_get_width(area) * lv_area_get_height(area) * sizeof(lv_color_t);

    // 4. 准备开始数据传输
    HAL_GPIO_WritePin(TFT_DC_GPIO_Port, TFT_DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_RESET);

    // 5. 启动DMA传输，然后函数立即返回，不会等待！
    HAL_SPI_Transmit_DMA(&hspi1, (uint8_t*)color_p, size);
}

// --- SPI DMA传输完成回调函数 ---
// 当DMA完成SPI发送后，硬件会自动调用这个函数
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    // 检查是否是我们正在使用的SPI1
    if (hspi->Instance == SPI1) {
    	// 传输完成后，拉高CS引脚，结束本次SPI通信
		HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, GPIO_PIN_SET);

		// **关键步骤**：通知LVGL刷新已完成
		if (g_lvgl_disp_drv != NULL) {
			lv_disp_flush_ready(g_lvgl_disp_drv);
		}
    }
}
