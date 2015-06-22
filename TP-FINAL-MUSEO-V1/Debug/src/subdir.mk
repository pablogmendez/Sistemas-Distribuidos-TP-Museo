################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/main_Administrador.cpp \
../src/main_Destructor.cpp \
../src/main_Lanzador.cpp \
../src/main_Persona.cpp \
../src/main_Puerta.cpp 

OBJS += \
./src/main_Administrador.o \
./src/main_Destructor.o \
./src/main_Lanzador.o \
./src/main_Persona.o \
./src/main_Puerta.o 

CPP_DEPS += \
./src/main_Administrador.d \
./src/main_Destructor.d \
./src/main_Lanzador.d \
./src/main_Persona.d \
./src/main_Puerta.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


