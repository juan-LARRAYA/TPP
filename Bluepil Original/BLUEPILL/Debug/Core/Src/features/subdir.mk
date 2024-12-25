################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/features/BMS_COM.c \
../Core/Src/features/MPPT.c \
../Core/Src/features/alm_mem_flash.c \
../Core/Src/features/com_satelite.c \
../Core/Src/features/temp_control.c 

OBJS += \
./Core/Src/features/BMS_COM.o \
./Core/Src/features/MPPT.o \
./Core/Src/features/alm_mem_flash.o \
./Core/Src/features/com_satelite.o \
./Core/Src/features/temp_control.o 

C_DEPS += \
./Core/Src/features/BMS_COM.d \
./Core/Src/features/MPPT.d \
./Core/Src/features/alm_mem_flash.d \
./Core/Src/features/com_satelite.d \
./Core/Src/features/temp_control.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/features/%.o Core/Src/features/%.su Core/Src/features/%.cyclo: ../Core/Src/features/%.c Core/Src/features/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-features

clean-Core-2f-Src-2f-features:
	-$(RM) ./Core/Src/features/BMS_COM.cyclo ./Core/Src/features/BMS_COM.d ./Core/Src/features/BMS_COM.o ./Core/Src/features/BMS_COM.su ./Core/Src/features/MPPT.cyclo ./Core/Src/features/MPPT.d ./Core/Src/features/MPPT.o ./Core/Src/features/MPPT.su ./Core/Src/features/alm_mem_flash.cyclo ./Core/Src/features/alm_mem_flash.d ./Core/Src/features/alm_mem_flash.o ./Core/Src/features/alm_mem_flash.su ./Core/Src/features/com_satelite.cyclo ./Core/Src/features/com_satelite.d ./Core/Src/features/com_satelite.o ./Core/Src/features/com_satelite.su ./Core/Src/features/temp_control.cyclo ./Core/Src/features/temp_control.d ./Core/Src/features/temp_control.o ./Core/Src/features/temp_control.su

.PHONY: clean-Core-2f-Src-2f-features

