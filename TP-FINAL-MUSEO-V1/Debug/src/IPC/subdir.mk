################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/IPC/Fifo.cpp \
../src/IPC/FifoEscritura.cpp \
../src/IPC/FifoLectura.cpp \
../src/IPC/Pipe.cpp \
../src/IPC/Semaforo.cpp \
../src/IPC/SignalHandler.cpp 

OBJS += \
./src/IPC/Fifo.o \
./src/IPC/FifoEscritura.o \
./src/IPC/FifoLectura.o \
./src/IPC/Pipe.o \
./src/IPC/Semaforo.o \
./src/IPC/SignalHandler.o 

CPP_DEPS += \
./src/IPC/Fifo.d \
./src/IPC/FifoEscritura.d \
./src/IPC/FifoLectura.d \
./src/IPC/Pipe.d \
./src/IPC/Semaforo.d \
./src/IPC/SignalHandler.d 


# Each subdirectory must supply rules for building sources it contributes
src/IPC/%.o: ../src/IPC/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


