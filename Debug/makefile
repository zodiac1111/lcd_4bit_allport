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
lcd_2.lss \

FLASH_IMAGE += \
lcd_2.hex \

EEPROM_IMAGE += \
lcd_2.eep \

SIZEDUMMY += \
sizedummy \

AVRDUDEDUMMY += \
avrdudedummy \


# All Target
all: lcd_2.elf secondary-outputs

# Tool invocations
lcd_2.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,lcd_2.map -mmcu=atmega8 -o "lcd_2.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

lcd_2.lss: lcd_2.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S lcd_2.elf  >"lcd_2.lss"
	@echo 'Finished building: $@'
	@echo ' '

lcd_2.hex: lcd_2.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex lcd_2.elf  "lcd_2.hex"
	@echo 'Finished building: $@'
	@echo ' '

lcd_2.eep: lcd_2.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex lcd_2.elf  "lcd_2.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: lcd_2.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=berkeley -t lcd_2.elf
	@echo 'Finished building: $@'
	@echo ' '

avrdudedummy: lcd_2.elf
	@echo 'Invoking: AVRDude'
	/usr/bin/avrdude -pm8 -cusbasp -PttyUSB0 -b115200 -B500 -i500 -Uflash:w:lcd_2.hex:a
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(AVRDUDEDUMMY)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) lcd_2.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY) $(AVRDUDEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
