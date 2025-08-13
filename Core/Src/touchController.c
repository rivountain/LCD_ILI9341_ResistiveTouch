#include "touchController.h"
#include "lvgl.h"
#include "touchDriver.h"

// LVGL读取触摸板的回调函数
static void touchpad_read(lv_indev_drv_t * drv, lv_indev_data_t * data)
{
    uint16_t touchX, touchY;

    if (Touch_GetCoordinates(&touchX, &touchY)) {
        data->state = LV_INDEV_STATE_PR; // press state
        data->point.x = touchX;
        data->point.y = touchY;
    } else {
        data->state = LV_INDEV_STATE_REL; // release state
    }
}

void lv_port_indev_init(void)
{
    Touch_Init();

    // 注册一个触摸板输入设备
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    lv_indev_drv_register(&indev_drv);
}
