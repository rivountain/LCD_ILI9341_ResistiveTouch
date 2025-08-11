#include "ili9341.h"

// 定义一个静态变量作为DMA传输完成的标志
// volatile 关键字是必须的，因为它可能在中断中被改变
static volatile uint8_t spi_dma_tx_completed = 1;

// --- 底层硬件操作函数 ---

// SPI发送单个字节
static void ILI9341_SPI_Tx(uint8_t data) {
    HAL_SPI_Transmit(ILI9341_SPI_HANDLE, &data, 1, HAL_MAX_DELAY);
}

//// SPI发送多个字节
//static void ILI9341_SPI_TxBuffer(uint8_t* buffer, uint16_t len) {
//    HAL_SPI_Transmit(ILI9341_SPI_HANDLE, buffer, len, HAL_MAX_DELAY);
//}
static void ILI9341_SPI_Wait_DMA_Completed(void) {
    while (!spi_dma_tx_completed);
}

//static void ILI9341_SPI_TxBuffer_DMA_Start(uint8_t* buffer, uint16_t len) {
//	// 等待上一次DMA传输完成
//	ILI9341_SPI_Wait_DMA_Completed();
//
//	// 开始新的传输前，将标志位置为“未完成”
//	spi_dma_tx_completed = 0;
//
//	// 启动DMA传输
//    HAL_SPI_Transmit_DMA(ILI9341_SPI_HANDLE, buffer, len);
//}

// 发送命令
static void ILI9341_WriteCommand(uint8_t cmd) {
	ILI9341_SPI_Wait_DMA_Completed();
    HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_RESET); // DC拉低，表示命令
    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_RESET); // CS拉低，选中芯片
    ILI9341_SPI_Tx(cmd);
    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_SET);   // CS拉高，取消选中
}

// 发送数据
static void ILI9341_WriteData(uint8_t data) {
	ILI9341_SPI_Wait_DMA_Completed();
    HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_SET);   // DC拉高，表示数据
    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_RESET); // CS拉低，选中芯片
    ILI9341_SPI_Tx(data);
    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_SET);   // CS拉高，取消选中
}

// 硬件复位
static void ILI9341_Reset(void) {
    HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(100);
}

// 设置绘图窗口
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


// --- 高层绘图函数 ---

// 初始化序列
void ILI9341_Init(void) {
    ILI9341_Reset();

    ILI9341_WriteCommand(0xCF); // Power control B
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0xC1);
    ILI9341_WriteData(0x30);

    ILI9341_WriteCommand(0xED); // Power on sequence control
    ILI9341_WriteData(0x64);
    ILI9341_WriteData(0x03);
    ILI9341_WriteData(0x12);
    ILI9341_WriteData(0x81);

    ILI9341_WriteCommand(0xE8); // Driver timing control A
    ILI9341_WriteData(0x85);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x78);

    ILI9341_WriteCommand(0xCB); // Power control A
    ILI9341_WriteData(0x39);
    ILI9341_WriteData(0x2C);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x34);
    ILI9341_WriteData(0x02);

    ILI9341_WriteCommand(0xF7); // Pump ratio control
    ILI9341_WriteData(0x20);

    ILI9341_WriteCommand(0xEA); // Driver timing control B
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x00);

    ILI9341_WriteCommand(0xC0); // Power Control 1
    ILI9341_WriteData(0x23);

    ILI9341_WriteCommand(0xC1); // Power Control 2
    ILI9341_WriteData(0x10);

    ILI9341_WriteCommand(0xC5); // VCM control 1
    ILI9341_WriteData(0x3E);
    ILI9341_WriteData(0x28);

    ILI9341_WriteCommand(0xC7); // VCM control 2
    ILI9341_WriteData(0x86);

    ILI9341_WriteCommand(0x36); // Memory Access Control (MADCTL): MX, MY, RGB order
//    ILI9341_WriteData(0x48);
    ILI9341_WriteData(0x28); 	// landscape
//    ILI9341_WriteData(0x28);	// landscape flipped

    ILI9341_WriteCommand(0x3A); // Pixel Format Set
    ILI9341_WriteData(0x55);    // 16 bits per pixel

    ILI9341_WriteCommand(0xB1); // Frame Rate Control
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x18);

    ILI9341_WriteCommand(0xB6); // Display Function Control
    ILI9341_WriteData(0x08);
    ILI9341_WriteData(0x82);
    ILI9341_WriteData(0x27);

    ILI9341_WriteCommand(0xF2); // 3Gamma Function Disable
    ILI9341_WriteData(0x00);

    ILI9341_WriteCommand(0x26); // Gamma curve selected
    ILI9341_WriteData(0x01);

    ILI9341_WriteCommand(0xE0); // Set Gamma
    ILI9341_WriteData(0x0F);
    ILI9341_WriteData(0x31);
    ILI9341_WriteData(0x2B);
    ILI9341_WriteData(0x0C);
    ILI9341_WriteData(0x0E);
    ILI9341_WriteData(0x08);
    ILI9341_WriteData(0x4E);
    ILI9341_WriteData(0xF1);
    ILI9341_WriteData(0x37);
    ILI9341_WriteData(0x07);
    ILI9341_WriteData(0x10);
    ILI9341_WriteData(0x03);
    ILI9341_WriteData(0x0E);
    ILI9341_WriteData(0x09);
    ILI9341_WriteData(0x00);

    ILI9341_WriteCommand(0xE1); // Set Gamma
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x0E);
    ILI9341_WriteData(0x14);
    ILI9341_WriteData(0x03);
    ILI9341_WriteData(0x11);
    ILI9341_WriteData(0x07);
    ILI9341_WriteData(0x31);
    ILI9341_WriteData(0xC1);
    ILI9341_WriteData(0x48);
    ILI9341_WriteData(0x08);
    ILI9341_WriteData(0x0F);
    ILI9341_WriteData(0x0C);
    ILI9341_WriteData(0x31);
    ILI9341_WriteData(0x36);
    ILI9341_WriteData(0x0F);

    ILI9341_WriteCommand(0x11); // Exit Sleep
    HAL_Delay(120);

    ILI9341_WriteCommand(0x29); // Display on
}


void ILI9341_FillScreen(uint16_t color) {
	ILI9341_FillRectangle_DMA(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, color);
}

void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if ((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT)) return;

    ILI9341_SetAddressWindow(x, y, x, y);
    uint8_t data[] = {color >> 8, color & 0xFF};

    // 等待DMA空闲
    ILI9341_SPI_Wait_DMA_Completed();
    HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_RESET);
    // 只发送一个像素，使用阻塞模式更简单高效
    HAL_SPI_Transmit(ILI9341_SPI_HANDLE, data, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_SET);
}

//void ILI9341_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
//    if ((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT)) return;
//    if ((x + w - 1) >= ILI9341_WIDTH) w = ILI9341_WIDTH - x;
//    if ((y + h - 1) >= ILI9341_HEIGHT) h = ILI9341_HEIGHT - y;
//
//    ILI9341_SetAddressWindow(x, y, x + w - 1, y + h - 1);
//
//    HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_SET);
//    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_RESET);
//
//    uint8_t data[] = {color >> 8, color & 0xFF};
//    for (y = h; y > 0; y--) {
//        for (x = w; x > 0; x--) {
//            HAL_SPI_Transmit(ILI9341_SPI_HANDLE, data, 2, HAL_MAX_DELAY);
//        }
//    }
//
//    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_SET);
//}
// ** 优化后的填充矩形函数 **
void ILI9341_FillRectangle_DMA(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if ((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT)) return;
    if ((x + w - 1) >= ILI9341_WIDTH) w = ILI9341_WIDTH - x;
    if ((y + h - 1) >= ILI9341_HEIGHT) h = ILI9341_HEIGHT - y;

    ILI9341_SetAddressWindow(x, y, x + w - 1, y + h - 1);

    HAL_GPIO_WritePin(ILI9341_DC_PORT, ILI9341_DC_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_RESET);

    // 使用最大可能的行缓冲区
	uint8_t row_buffer[ILI9341_WIDTH * 2];
	uint16_t row_size_bytes = w * 2;

	// 填充缓冲区为指定颜色
	for (uint16_t i = 0; i < row_size_bytes; i += 2) {
		row_buffer[i] = color >> 8;
		row_buffer[i+1] = color & 0xFF;
	}

	// 逐行发送，每次发送都等待上一次完成
	for (uint16_t i = 0; i < h; i++) {
		while (!spi_dma_tx_completed); // 等待
		spi_dma_tx_completed = 0;      // 清零
		HAL_SPI_Transmit_DMA(ILI9341_SPI_HANDLE, row_buffer, row_size_bytes); // 启动
	}


	// 等待最后一次DMA传输完成
	ILI9341_SPI_Wait_DMA_Completed();
    HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_SET);
}

// *** 新增：一个简单的、基于DrawPixel的矩形填充函数 ***
// 这个函数专门给DrawChar用于字符放大，我们不把它在.h中声明，作为内部函数
static void ILI9341_FillRectangle_Slow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    for (uint16_t j = 0; j < h; j++) {
        for (uint16_t i = 0; i < w; i++) {
            ILI9341_DrawPixel(x + i, y + j, color);
        }
    }
}

// *** 基于Adafruit GFX逻辑的字符绘制函数 ***
// 这个函数现在会调用我们内部的慢速填充函数
void ILI9341_DrawChar(uint16_t x, uint16_t y, char c, sFont* font, uint16_t color, uint16_t bg, uint8_t size) {
    if (size == 0) size = 1;
    if ((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT) ||
        ((x + font->Width * size - 1) < 0) || ((y + font->Height * size - 1) < 0))
        return;

    if (c < ' ' || c > '~') c = ' ';

    for (int8_t i = 0; i < font->Width; i++) {
        uint8_t line = font->table[(c - ' ') * font->Width + i];
        for (int8_t j = 0; j < font->Height; j++, line >>= 1) {
            if (line & 0x01) {
                if (size == 1)
                    ILI9341_DrawPixel(x + i, y + j, color);
                else
                    // *** 调用慢速版 ***
                    ILI9341_FillRectangle_Slow(x + i * size, y + j * size, size, size, color);
            } else if (bg != color) {
                if (size == 1)
                    ILI9341_DrawPixel(x + i, y + j, bg);
                else
                    // *** 调用慢速版 ***
                    ILI9341_FillRectangle_Slow(x + i * size, y + j * size, size, size, bg);
            }
        }
    }
}

void ILI9341_DrawString(uint16_t x, uint16_t y, const char* str, sFont* font, uint16_t color, uint16_t bg, uint8_t size) {
    while (*str) {
    	uint16_t char_width = (font->Width + 1) * size;
        if (x + char_width >= ILI9341_WIDTH) {
            x = 0;
            y += font->Height * size;
            if (y + font->Height * size >= ILI9341_HEIGHT) break;
        }
        ILI9341_DrawChar(x, y, *str, font, color, bg, size);
        x += char_width;
        str++;
    }
}

// --- SPI DMA传输完成回调函数 ---
// 当DMA完成SPI发送后，硬件会自动调用这个函数
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    // 检查是否是我们正在使用的SPI1
    if (hspi->Instance == SPI1) {
        // 设置标志位，表示传输已完成
        spi_dma_tx_completed = 1;
    }
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    // 在这里设置第二个断点 (断点2)
    // 如果程序停在这里，就说明DMA出错了！
    __NOP(); // 这是一条空指令，方便设置断点
}
