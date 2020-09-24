################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/My\ Reference\ Project.cpp 

OBJS += \
./src/My\ Reference\ Project.o 

CPP_DEPS += \
./src/My\ Reference\ Project.d 


# Each subdirectory must supply rules for building sources it contributes
src/My\ Reference\ Project.o: ../src/My\ Reference\ Project.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/My Reference Project.d" -MT"src/My\ Reference\ Project.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


