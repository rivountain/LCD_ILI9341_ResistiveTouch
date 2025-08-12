#include "lvglController.h"
#include "ili9341.h"

// --- LVGL 显示缓冲区 ---
// LVGL需要一小块RAM来渲染图像，然后再发送到屏幕
// 缓冲区大小可以根据您的RAM余量调整，通常是屏幕宽度的10-20倍
#define LV_DISP_BUF_SIZE (ILI9341_WIDTH * 20)

static lv_disp_draw_buf_t disp_buf;
static lv_color_t buf_1[LV_DISP_BUF_SIZE];

void lv_port_disp_init(void)
{
    // 1. 初始化底层硬件
    ILI9341_Init();

    // 2. 初始化LVGL的显示缓冲区
    lv_disp_draw_buf_init(&disp_buf, buf_1, NULL, LV_DISP_BUF_SIZE);

    // 3. 初始化LVGL的显示驱动
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);

    // 4. 关联我们的刷新回调函数
    disp_drv.flush_cb = ILI9341_Flush_Callback;

    // 5. 关联我们的显示缓冲区
    disp_drv.draw_buf = &disp_buf;

    // 6. 设置屏幕分辨率
    disp_drv.hor_res = ILI9341_WIDTH;
    disp_drv.ver_res = ILI9341_HEIGHT;

    // 7. 最终注册驱动
    lv_disp_drv_register(&disp_drv);
}


