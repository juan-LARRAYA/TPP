################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := BLUEPILL_CHINA_128k
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
BLUEPILL_CHINA_128k.elf \

MAP_FILES += \
BLUEPILL_CHINA_128k.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
BLUEPILL_CHINA_128k.list \

OBJCOPY_HEX += \
BLUEPILL_CHINA_128k.hex \

OBJCOPY_BIN += \
BLUEPILL_CHINA_128k.bin \


# All Target
all: main-build

# Main-build Target
main-build: BLUEPILL_CHINA_128k.elf secondary-outputs

# Tool invocations
BLUEPILL_CHINA_128k.elf BLUEPILL_CHINA_128k.map: $(OBJS) $(USER_OBJS) /Users/apple/STM32CubeIDE/BLUEPILL_CHINA_128k/STM32F103CBTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "BLUEPILL_CHINA_128k.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"/Users/apple/STM32CubeIDE/BLUEPILL_CHINA_128k/STM32F103CBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="BLUEPILL_CHINA_128k.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

BLUEPILL_CHINA_128k.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "BLUEPILL_CHINA_128k.list"
	@echo 'Finished building: $@'
	@echo ' '

BLUEPILL_CHINA_128k.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "BLUEPILL_CHINA_128k.hex"
	@echo 'Finished building: $@'
	@echo ' '

BLUEPILL_CHINA_128k.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "BLUEPILL_CHINA_128k.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) BLUEPILL_CHINA_128k.bin BLUEPILL_CHINA_128k.elf BLUEPILL_CHINA_128k.hex BLUEPILL_CHINA_128k.list BLUEPILL_CHINA_128k.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
