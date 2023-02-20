################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User_Lib/pn532.c 

OBJS += \
./User_Lib/pn532.o 

C_DEPS += \
./User_Lib/pn532.d 


# Each subdirectory must supply rules for building sources it contributes
User_Lib/%.o User_Lib/%.su: ../User_Lib/%.c User_Lib/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"/home/hoanganh/STM32CubeIDE/NoiLamViec/PN532/User_Lib" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User_Lib

clean-User_Lib:
	-$(RM) ./User_Lib/pn532.d ./User_Lib/pn532.o ./User_Lib/pn532.su

.PHONY: clean-User_Lib

