################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../myDrivers/App.c \
../myDrivers/Low_Power.c \
../myDrivers/RGB.c \
../myDrivers/SO.c \
../myDrivers/data_structures.c 

C_DEPS += \
./myDrivers/App.d \
./myDrivers/Low_Power.d \
./myDrivers/RGB.d \
./myDrivers/SO.d \
./myDrivers/data_structures.d 

OBJS += \
./myDrivers/App.o \
./myDrivers/Low_Power.o \
./myDrivers/RGB.o \
./myDrivers/SO.o \
./myDrivers/data_structures.o 


# Each subdirectory must supply rules for building sources it contributes
myDrivers/%.o: ../myDrivers/%.c myDrivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK66FN2M0VMD18 -DCPU_MK66FN2M0VMD18_cm4 -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\MCU_SOTR\XD\board" -I"C:\MCU_SOTR\XD\myDrivers" -I"C:\MCU_SOTR\XD\source" -I"C:\MCU_SOTR\XD\drivers" -I"C:\MCU_SOTR\XD\component\uart" -I"C:\MCU_SOTR\XD\device" -I"C:\MCU_SOTR\XD\CMSIS" -I"C:\MCU_SOTR\XD\component\serial_manager" -I"C:\MCU_SOTR\XD\component\lists" -I"C:\MCU_SOTR\XD\utilities" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-myDrivers

clean-myDrivers:
	-$(RM) ./myDrivers/App.d ./myDrivers/App.o ./myDrivers/Low_Power.d ./myDrivers/Low_Power.o ./myDrivers/RGB.d ./myDrivers/RGB.o ./myDrivers/SO.d ./myDrivers/SO.o ./myDrivers/data_structures.d ./myDrivers/data_structures.o

.PHONY: clean-myDrivers

