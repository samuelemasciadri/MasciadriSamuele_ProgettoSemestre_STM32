################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F0xx_HAL_Driver/Src/subdir.mk
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


BUILD_ARTIFACT_NAME := Masciadri-Samuele_ProgettoSemestre
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Masciadri-Samuele_ProgettoSemestre.elf \

MAP_FILES += \
Masciadri-Samuele_ProgettoSemestre.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Masciadri-Samuele_ProgettoSemestre.list \


# All Target
all: main-build

# Main-build Target
main-build: Masciadri-Samuele_ProgettoSemestre.elf secondary-outputs

# Tool invocations
Masciadri-Samuele_ProgettoSemestre.elf Masciadri-Samuele_ProgettoSemestre.map: $(OBJS) $(USER_OBJS) C:\Users\samue\OneDrive\Desktop\4°Semestre\Progetto\ di\ Semestre\Masciadri\ Samuele\A5-\ FW\STM32\STM32F042K6TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Masciadri-Samuele_ProgettoSemestre.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0 -T"C:\Users\samue\OneDrive\Desktop\4°Semestre\Progetto di Semestre\Masciadri Samuele\A5- FW\STM32\STM32F042K6TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Masciadri-Samuele_ProgettoSemestre.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Masciadri-Samuele_ProgettoSemestre.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Masciadri-Samuele_ProgettoSemestre.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Masciadri-Samuele_ProgettoSemestre.elf Masciadri-Samuele_ProgettoSemestre.list Masciadri-Samuele_ProgettoSemestre.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
