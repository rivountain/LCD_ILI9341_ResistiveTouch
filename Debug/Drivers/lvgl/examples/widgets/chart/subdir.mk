################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/lvgl/examples/widgets/chart/lv_example_chart_1.c \
../Drivers/lvgl/examples/widgets/chart/lv_example_chart_2.c \
../Drivers/lvgl/examples/widgets/chart/lv_example_chart_3.c \
../Drivers/lvgl/examples/widgets/chart/lv_example_chart_4.c \
../Drivers/lvgl/examples/widgets/chart/lv_example_chart_5.c \
../Drivers/lvgl/examples/widgets/chart/lv_example_chart_6.c \
../Drivers/lvgl/examples/widgets/chart/lv_example_chart_7.c \
../Drivers/lvgl/examples/widgets/chart/lv_example_chart_8.c \
../Drivers/lvgl/examples/widgets/chart/lv_example_chart_9.c 

OBJS += \
./Drivers/lvgl/examples/widgets/chart/lv_example_chart_1.o \
./Drivers/lvgl/examples/widgets/chart/lv_example_chart_2.o \
./Drivers/lvgl/examples/widgets/chart/lv_example_chart_3.o \
./Drivers/lvgl/examples/widgets/chart/lv_example_chart_4.o \
./Drivers/lvgl/examples/widgets/chart/lv_example_chart_5.o \
./Drivers/lvgl/examples/widgets/chart/lv_example_chart_6.o \
./Drivers/lvgl/examples/widgets/chart/lv_example_chart_7.o \
./Drivers/lvgl/examples/widgets/chart/lv_example_chart_8.o \
./Drivers/lvgl/examples/widgets/chart/lv_example_chart_9.o 

C_DEPS += \
./Drivers/lvgl/examples/widgets/chart/lv_example_chart_1.d \
./Drivers/lvgl/examples/widgets/chart/lv_example_chart_2.d \
./Drivers/lvgl/examples/widgets/chart/lv_example_chart_3.d \
./Drivers/lvgl/examples/widgets/chart/lv_example_chart_4.d \
./Drivers/lvgl/examples/widgets/chart/lv_example_chart_5.d \
./Drivers/lvgl/examples/widgets/chart/lv_example_chart_6.d \
./Drivers/lvgl/examples/widgets/chart/lv_example_chart_7.d \
./Drivers/lvgl/examples/widgets/chart/lv_example_chart_8.d \
./Drivers/lvgl/examples/widgets/chart/lv_example_chart_9.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/lvgl/examples/widgets/chart/%.o Drivers/lvgl/examples/widgets/chart/%.su Drivers/lvgl/examples/widgets/chart/%.cyclo: ../Drivers/lvgl/examples/widgets/chart/%.c Drivers/lvgl/examples/widgets/chart/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32U575xx -c -I../Core/Inc -I"D:/github_repo/LCD_ILI9341_ResistiveTouch/Drivers/lvgl/src" -I"D:/github_repo/LCD_ILI9341_ResistiveTouch/Drivers/lvgl" -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32U5xx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-lvgl-2f-examples-2f-widgets-2f-chart

clean-Drivers-2f-lvgl-2f-examples-2f-widgets-2f-chart:
	-$(RM) ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_1.cyclo ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_1.d ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_1.o ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_1.su ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_2.cyclo ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_2.d ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_2.o ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_2.su ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_3.cyclo ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_3.d ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_3.o ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_3.su ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_4.cyclo ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_4.d ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_4.o ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_4.su ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_5.cyclo ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_5.d ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_5.o ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_5.su ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_6.cyclo ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_6.d ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_6.o ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_6.su ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_7.cyclo ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_7.d ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_7.o ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_7.su ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_8.cyclo ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_8.d ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_8.o ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_8.su ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_9.cyclo ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_9.d ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_9.o ./Drivers/lvgl/examples/widgets/chart/lv_example_chart_9.su

.PHONY: clean-Drivers-2f-lvgl-2f-examples-2f-widgets-2f-chart

