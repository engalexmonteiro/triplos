################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/charts/charts.c \
../src/charts/gnuplot_i.c 

OBJS += \
./src/charts/charts.o \
./src/charts/gnuplot_i.o 

C_DEPS += \
./src/charts/charts.d \
./src/charts/gnuplot_i.d 


# Each subdirectory must supply rules for building sources it contributes
src/charts/%.o: ../src/charts/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


