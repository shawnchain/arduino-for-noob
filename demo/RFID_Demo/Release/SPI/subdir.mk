################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/Users/shawn/Arduino/developer/Arduino.app/Contents/Resources/Java/libraries/SPI/SPI.cpp 

OBJS += \
./SPI/SPI.o 

CPP_DEPS += \
./SPI/SPI.d 


# Each subdirectory must supply rules for building sources it contributes
SPI/SPI.o: /Users/shawn/Arduino/developer/Arduino.app/Contents/Resources/Java/libraries/SPI/SPI.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"/Users/shawn/Arduino/developer/Arduino.app/Contents/Resources/Java/libraries/SPI" -I"/Users/shawn/works/tb-joylabs/arduino.git/libraries" -I"/Users/shawn/Arduino/developer/Arduino.app/Contents/Resources/Java/hardware/arduino/cores/arduino" -I"/Users/shawn/Arduino/developer/Arduino.app/Contents/Resources/Java/hardware/arduino/variants/mega" -D__IN_ECLIPSE__=1 -DARDUINO=103 -DUSB_PID= -DUSB_VID= -Wall -Os -ffunction-sections -fdata-sections -fno-exceptions -g -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"  -c -o "$@" -x c++ "$<"
	@echo 'Finished building: $<'
	@echo ' '


