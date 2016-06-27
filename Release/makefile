################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
CapazitiveTasten.lss \

FLASH_IMAGE += \
CapazitiveTasten.hex \

EEPROM_IMAGE += \
CapazitiveTasten.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: CapazitiveTasten.elf secondary-outputs

# Tool invocations
CapazitiveTasten.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,CapazitiveTasten.map -mmcu=atmega32 -o "CapazitiveTasten.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

CapazitiveTasten.lss: CapazitiveTasten.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S CapazitiveTasten.elf  >"CapazitiveTasten.lss"
	@echo 'Finished building: $@'
	@echo ' '

CapazitiveTasten.hex: CapazitiveTasten.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex CapazitiveTasten.elf  "CapazitiveTasten.hex"
	@echo 'Finished building: $@'
	@echo ' '

CapazitiveTasten.eep: CapazitiveTasten.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex CapazitiveTasten.elf  "CapazitiveTasten.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: CapazitiveTasten.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 CapazitiveTasten.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) CapazitiveTasten.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
