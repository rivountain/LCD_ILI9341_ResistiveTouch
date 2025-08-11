#ifndef FONTS_H
#define FONTS_H

#include <stdint.h>

// 定义一个字体的数据结构
typedef struct {
    const uint8_t *table; // 指向字体数据的指针
    uint16_t Width;       // 每个字符的宽度（像素）
    uint16_t Height;      // 每个字符的高度（像素）
} sFont;

// 声明我们将要使用的5x7像素字体
extern sFont Font_5x7;

#endif // FONTS_H
