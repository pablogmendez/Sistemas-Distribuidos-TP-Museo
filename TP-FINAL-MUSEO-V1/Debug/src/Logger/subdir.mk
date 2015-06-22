################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Logger/Lock.cpp \
../src/Logger/Logger.cpp \
../src/Logger/MensajeLog.cpp 

OBJS += \
./src/Logger/Lock.o \
./src/Logger/Logger.o \
./src/Logger/MensajeLog.o 

CPP_DEPS += \
./src/Logger/Lock.d \
./src/Logger/Logger.d \
./src/Logger/MensajeLog.d 


# Each subdirectory must supply rules for building sources it contributes
src/Logger/%.o: ../src/Logger/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


