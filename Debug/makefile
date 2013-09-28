################################################################################
# 自动生成的文件。不要编辑！
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
lcd_4bit_allport.lss \

FLASH_IMAGE += \
lcd_4bit_allport.hex \

EEPROM_IMAGE += \
lcd_4bit_allport.eep \

SIZEDUMMY += \
sizedummy \

AVRDUDEDUMMY += \
avrdudedummy \


# 所有目标
all: lcd_4bit_allport.elf secondary-outputs

# 工具调用
lcd_4bit_allport.elf: $(OBJS) $(USER_OBJS)
	@echo '正在构建目标： $@'
	@echo '正在调用： AVR C Linker'
	avr-gcc -Wl,-Map,lcd_4bit_allport.map -mmcu=atmega8 -o "lcd_4bit_allport.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo '已结束构建目标： $@'
	@echo ' '

lcd_4bit_allport.lss: lcd_4bit_allport.elf
	@echo '正在调用： AVR Create Extended Listing'
	-avr-objdump -h -S lcd_4bit_allport.elf  >"lcd_4bit_allport.lss"
	@echo '已结束构建： $@'
	@echo ' '

lcd_4bit_allport.hex: lcd_4bit_allport.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex lcd_4bit_allport.elf  "lcd_4bit_allport.hex"
	@echo '已结束构建： $@'
	@echo ' '

lcd_4bit_allport.eep: lcd_4bit_allport.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex lcd_4bit_allport.elf  "lcd_4bit_allport.eep"
	@echo '已结束构建： $@'
	@echo ' '

sizedummy: lcd_4bit_allport.elf
	@echo '正在调用： Print Size'
	-avr-size --format=avr --mcu=atmega8 lcd_4bit_allport.elf
	@echo '已结束构建： $@'
	@echo ' '

avrdudedummy: lcd_4bit_allport.elf
	@echo '正在调用： AVRDude'
	/usr/bin/avrdude -pm8 -cusbasp -Uflash:w:lcd_4bit_allport.hex:a
	@echo '已结束构建： $@'
	@echo ' '

# 其他目标
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(AVRDUDEDUMMY)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) lcd_4bit_allport.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY) $(AVRDUDEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
