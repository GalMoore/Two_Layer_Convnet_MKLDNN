################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CE_CPP.cpp \
../src/Function_0_codegen.cpp \
../src/aligned_buffer.cpp 

OBJS += \
./src/CE_CPP.o \
./src/Function_0_codegen.o \
./src/aligned_buffer.o 

CPP_DEPS += \
./src/CE_CPP.d \
./src/Function_0_codegen.d \
./src/aligned_buffer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/opt/ngraph/include/ -I/home/elad/MKLDNNROOT_MASTER/include -O3 -Wall -c -fmessage-length=0 -mavx2 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


