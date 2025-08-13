################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/lvgl/src/extra/widgets/chart/lv_chart.c 

OBJS += \
./Drivers/lvgl/src/extra/widgets/chart/lv_chart.o 

C_DEPS += \
./Drivers/lvgl/src/extra/widgets/chart/lv_chart.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/lvgl/src/extra/widgets/chart/%.o Drivers/lvgl/src/extra/widgets/chart/%.su Drivers/lvgl/src/extra/widgets/chart/%.cyclo: ../Drivers/lvgl/src/extra/widgets/chart/%.c Drivers/lvgl/src/extra/widgets/chart/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32U575xx -c -I../Core/Inc -I"D:/github_repo/LCD_ILI9341_ResistiveTouch/Drivers/lvgl/src" -I"D:/github_repo/LCD_ILI9341_ResistiveTouch/Drivers/lvgl" -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32U5xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-lvgl-2f-src-2f-extra-2f-widgets-2f-chart

clean-Drivers-2f-lvgl-2f-src-2f-extra-2f-widgets-2f-chart:
	-$(RM) ./Drivers/lvgl/src/extra/widgets/chart/lv_chart.cyclo ./Drivers/lvgl/src/extra/widgets/chart/lv_chart.d ./Drivers/lvgl/src/extra/widgets/chart/lv_chart.o ./Drivers/lvgl/src/extra/widgets/chart/lv_chart.su

.PHONY: clean-Drivers-2f-lvgl-2f-src-2f-extra-2f-widgets-2f-chart

