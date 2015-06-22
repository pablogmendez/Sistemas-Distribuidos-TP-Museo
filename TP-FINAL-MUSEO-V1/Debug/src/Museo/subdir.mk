################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Museo/Administrador.cpp \
../src/Museo/Destructor.cpp \
../src/Museo/Investigador.cpp \
../src/Museo/Lanzador.cpp \
../src/Museo/Persona.cpp \
../src/Museo/PersonaNormal.cpp \
../src/Museo/Puerta.cpp 

OBJS += \
./src/Museo/Administrador.o \
./src/Museo/Destructor.o \
./src/Museo/Investigador.o \
./src/Museo/Lanzador.o \
./src/Museo/Persona.o \
./src/Museo/PersonaNormal.o \
./src/Museo/Puerta.o 

CPP_DEPS += \
./src/Museo/Administrador.d \
./src/Museo/Destructor.d \
./src/Museo/Investigador.d \
./src/Museo/Lanzador.d \
./src/Museo/Persona.d \
./src/Museo/PersonaNormal.d \
./src/Museo/Puerta.d 


# Each subdirectory must supply rules for building sources it contributes
src/Museo/%.o: ../src/Museo/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


