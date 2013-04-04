################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include SPI/subdir.mk
-include JOYLABS_ARDUINO_LIBS/RFID/subdir.mk
-include JOYLABS_ARDUINO_LIBS/LCD/subdir.mk
-include JOYLABS_ARDUINO_LIBS/Keypad/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
RFID_Demo.lss \

FLASH_IMAGE += \
RFID_Demo.hex \

EEPROM_IMAGE += \
RFID_Demo.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: RFID_Demo.elf secondary-outputs

dependents:
	-cd /Users/shawn/Arduino/workspace/Arduino_Mega_2560_or_Mega_ADK/Release && $(MAKE) clean all

# Tool invocations
RFID_Demo.elf: $(OBJS) $(USER_OBJS) /Users/shawn/Arduino/workspace/Arduino_Mega_2560_or_Mega_ADK/Release/libArduino_Mega_2560_or_Mega_ADK.a
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C++ Linker'
	avr-gcc -Os -Wl,--gc-sections  -L"/Users/shawn/Arduino/workspace/Arduino_Mega_2560_or_Mega_ADK/Release" -mmcu=atmega2560  -o"RFID_Demo.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lm
	@echo 'Finished building target: $@'
	@echo ' '

RFID_Demo.lss: RFID_Demo.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S RFID_Demo.elf  >"RFID_Demo.lss"
	@echo 'Finished building: $@'
	@echo ' '

RFID_Demo.hex: RFID_Demo.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex RFID_Demo.elf  "RFID_Demo.hex"
	@echo 'Finished building: $@'
	@echo ' '

RFID_Demo.eep: RFID_Demo.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex RFID_Demo.elf  "RFID_Demo.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: RFID_Demo.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega2560 RFID_Demo.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EEPROM_IMAGE)$(ELFS)$(FLASH_IMAGE)$(LSS)$(S_DEPS)$(CXX_DEPS)$(S_UPPER_DEPS)$(C++_DEPS)$(ASM_DEPS)$(CC_DEPS)$(CPP_DEPS)$(C_UPPER_DEPS)$(SIZEDUMMY) RFID_Demo.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:
/Users/shawn/Arduino/workspace/Arduino_Mega_2560_or_Mega_ADK/Release/libArduino_Mega_2560_or_Mega_ADK.a:

-include ../makefile.targets
