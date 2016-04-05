################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/energy/conspower.c 

OBJS += \
./src/energy/conspower.o 

C_DEPS += \
./src/energy/conspower.d 


# Each subdirectory must supply rules for building sources it contributes
src/energy/%.o: ../src/energy/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


