################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User_Libs/DS3231.c \
../User_Libs/Keypad.c \
../User_Libs/Lora.c \
../User_Libs/TFT.c \
../User_Libs/W25Q.c 

OBJS += \
./User_Libs/DS3231.o \
./User_Libs/Keypad.o \
./User_Libs/Lora.o \
./User_Libs/TFT.o \
./User_Libs/W25Q.o 

C_DEPS += \
./User_Libs/DS3231.d \
./User_Libs/Keypad.d \
./User_Libs/Lora.d \
./User_Libs/TFT.d \
./User_Libs/W25Q.d 


# Each subdirectory must supply rules for building sources it contributes
User_Libs/%.o User_Libs/%.su: ../User_Libs/%.c User_Libs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../User_Libs -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-User_Libs

clean-User_Libs:
	-$(RM) ./User_Libs/DS3231.d ./User_Libs/DS3231.o ./User_Libs/DS3231.su ./User_Libs/Keypad.d ./User_Libs/Keypad.o ./User_Libs/Keypad.su ./User_Libs/Lora.d ./User_Libs/Lora.o ./User_Libs/Lora.su ./User_Libs/TFT.d ./User_Libs/TFT.o ./User_Libs/TFT.su ./User_Libs/W25Q.d ./User_Libs/W25Q.o ./User_Libs/W25Q.su

.PHONY: clean-User_Libs

