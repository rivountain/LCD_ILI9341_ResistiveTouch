# LCD_ILI9341_ResistiveTouch
Play with Adafruit 2.8'' TFT LCD with Touchscreen Breakout Board
> STM32CubeIDE 1.19.0
<br> STM32U575

## TODO:
- [x] ILI9341 Driver
  - [x] Update: DMA instead of Blocking CPU
  - [x]	Add Font_5x7
  - [x]	Can Adjust Font Size
  - [x] Test
- [x] LVGL
  - [ ] SquareLine Studio for UI Design
  - [ ] Test
- [ ] Touch Driver
  - [ ] Test

## Port Configuration
| TFT LCD | NUCLEO-U575ZI-Q | Note |
| ------- | --------------- | ---- |
| GND | GND | - |
| Vin | 3V3 | - |
| CLK	| PA_5 | SPI_1_SCK |
| MOSI | PA_7 | SPI_1_MOSI |
| CS | PD_14 | GPIO_Output |
| D/C | PD_15 | GPIO_Output |
| RST | PF_12 | GPIO_Output |
| Lite | 3V3 | - |
| Y+ | PC2 | ADC_1_IN3 |
| X+ | PC1 | ADC_1_IN2 |
| Y- | PA2 | ADC_1_IN7 |
| X- | PA3 | ADC_1_IN8 |

## GPDMA_1
- DMA Request:                     SPI1_TX
- Direction:                       Memory to Peripheral
- Source Increment Address:        Enable
- Destination Increment Address:   Disable
- NVIC:                            Checked Global Interrupt

## Clock Configuration
- HCLK 160 MHz
- SPI1 Clock Parameter
  - Prescaler 16
  - > Baud Rate 10 MBits/s, min serial clock cycle 100 ns => max freq 10 MHz
  - Clock Polarity (CPOL) Low
  - Clock Phase (CPHA) 1 Edge
