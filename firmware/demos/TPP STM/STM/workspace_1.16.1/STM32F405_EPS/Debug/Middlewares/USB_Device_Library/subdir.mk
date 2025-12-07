################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c \
/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c 

OBJS += \
./Middlewares/USB_Device_Library/usbd_cdc.o \
./Middlewares/USB_Device_Library/usbd_core.o \
./Middlewares/USB_Device_Library/usbd_ctlreq.o \
./Middlewares/USB_Device_Library/usbd_ioreq.o 

C_DEPS += \
./Middlewares/USB_Device_Library/usbd_cdc.d \
./Middlewares/USB_Device_Library/usbd_core.d \
./Middlewares/USB_Device_Library/usbd_ctlreq.d \
./Middlewares/USB_Device_Library/usbd_ioreq.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/USB_Device_Library/usbd_cdc.o: /Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c Middlewares/USB_Device_Library/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F405xx -c -I../Core/Inc -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/STM32F4xx_HAL_Driver/Inc -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/USB_Device_Library/usbd_core.o: /Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c Middlewares/USB_Device_Library/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F405xx -c -I../Core/Inc -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/STM32F4xx_HAL_Driver/Inc -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/USB_Device_Library/usbd_ctlreq.o: /Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c Middlewares/USB_Device_Library/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F405xx -c -I../Core/Inc -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/STM32F4xx_HAL_Driver/Inc -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/USB_Device_Library/usbd_ioreq.o: /Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c Middlewares/USB_Device_Library/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F405xx -c -I../Core/Inc -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/STM32F4xx_HAL_Driver/Inc -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I/Users/apple/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.1/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-USB_Device_Library

clean-Middlewares-2f-USB_Device_Library:
	-$(RM) ./Middlewares/USB_Device_Library/usbd_cdc.cyclo ./Middlewares/USB_Device_Library/usbd_cdc.d ./Middlewares/USB_Device_Library/usbd_cdc.o ./Middlewares/USB_Device_Library/usbd_cdc.su ./Middlewares/USB_Device_Library/usbd_core.cyclo ./Middlewares/USB_Device_Library/usbd_core.d ./Middlewares/USB_Device_Library/usbd_core.o ./Middlewares/USB_Device_Library/usbd_core.su ./Middlewares/USB_Device_Library/usbd_ctlreq.cyclo ./Middlewares/USB_Device_Library/usbd_ctlreq.d ./Middlewares/USB_Device_Library/usbd_ctlreq.o ./Middlewares/USB_Device_Library/usbd_ctlreq.su ./Middlewares/USB_Device_Library/usbd_ioreq.cyclo ./Middlewares/USB_Device_Library/usbd_ioreq.d ./Middlewares/USB_Device_Library/usbd_ioreq.o ./Middlewares/USB_Device_Library/usbd_ioreq.su

.PHONY: clean-Middlewares-2f-USB_Device_Library

