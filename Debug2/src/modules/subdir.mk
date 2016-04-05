################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/modules/mod80211.c \
../src/modules/mod802151.c 

OBJS += \
./src/modules/mod80211.o \
./src/modules/mod802151.o 

C_DEPS += \
./src/modules/mod80211.d \
./src/modules/mod802151.d 


# Each subdirectory must supply rules for building sources it contributes
src/modules/%.o: ../src/modules/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


