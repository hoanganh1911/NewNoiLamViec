################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User_Libs/keypad_pcf.c 

OBJS += \
./User_Libs/keypad_pcf.o 

C_DEPS += \
./User_Libs/keypad_pcf.d 


# Each subdirectory must supply rules for building sources it contributes
User_Libs/%.o User_Libs/%.su: ../User_Libs/%.c User_Libs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../User_Libs -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User_Libs

clean-User_Libs:
	-$(RM) ./User_Libs/keypad_pcf.d ./User_Libs/keypad_pcf.o ./User_Libs/keypad_pcf.su

.PHONY: clean-User_Libs

