################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../device/system_MK66F18.c 

C_DEPS += \
./device/system_MK66F18.d 

OBJS += \
./device/system_MK66F18.o 


# Each subdirectory must supply rules for building sources it contributes
device/%.o: ../device/%.c device/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK66FN2M0VMD18 -DCPU_MK66FN2M0VMD18_cm4 -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"/home/kenneth/RTOS/OSEK_vUART_PWM/board" -I"/home/kenneth/RTOS/OSEK_vUART_PWM/myDrivers/SAE" -I"/home/kenneth/RTOS/OSEK_vUART_PWM/drivers" -I"/home/kenneth/RTOS/OSEK_vUART_PWM/component/uart" -I"/home/kenneth/RTOS/OSEK_vUART_PWM/device" -I"/home/kenneth/RTOS/OSEK_vUART_PWM/CMSIS" -I"/home/kenneth/RTOS/OSEK_vUART_PWM/component/serial_manager" -I"/home/kenneth/RTOS/OSEK_vUART_PWM/component/lists" -I"/home/kenneth/RTOS/OSEK_vUART_PWM/utilities" -I"/home/kenneth/RTOS/OSEK_vUART_PWM/myDrivers" -I"/home/kenneth/RTOS/OSEK_vUART_PWM/source" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-device

clean-device:
	-$(RM) ./device/system_MK66F18.d ./device/system_MK66F18.o

.PHONY: clean-device

