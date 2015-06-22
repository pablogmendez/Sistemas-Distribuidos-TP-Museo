################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/Utils/subdir.mk
-include src/Museo/subdir.mk
-include src/Logger/subdir.mk
-include src/IPC/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

COMMON += \
./src/Utils/ParserConfiguracion.o \
./src/Utils/Utils.o \
./src/Logger/Lock.o \
./src/Logger/Logger.o \
./src/Logger/MensajeLog.o \
./src/IPC/Fifo.o \
./src/IPC/FifoEscritura.o \
./src/IPC/FifoLectura.o \
./src/IPC/Pipe.o \
./src/IPC/Semaforo.o \
./src/IPC/SignalHandler.o \
./src/Museo/Destructor.o \
./src/Museo/Lanzador.o \
./src/Museo/Persona.o \
./src/Museo/PersonaNormal.o \
./src/Museo/Investigador.o \
./src/Museo/Puerta.o \
./src/Museo/Administrador.o

T1 += $(COMMON) \
./src/main_Administrador.o

T2 += $(COMMON) \
./src/main_Destructor.o

T3 += $(COMMON) \
./src/main_Lanzador.o

T4 += $(COMMON) \
./src/main_Puerta.o

T5 += $(COMMON) \
./src/main_Persona.o

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: TPFINAL

# Tool invocations
TPFINAL: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o "Administrador" $(T1) $(USER_OBJS) $(LIBS)
	g++  -o "Destructor" $(T2) $(USER_OBJS) $(LIBS)
	g++  -o "Lanzador" $(T3) $(USER_OBJS) $(LIBS)
	g++  -o "Puerta" $(T4) $(USER_OBJS) $(LIBS)
	g++  -o "Persona" $(T5) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) Administrador Destructor Lanzador Puerta Persona
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
