################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/utils/conversion/CCircleInput.cpp \
../src/utils/conversion/CXMLOutput.cpp \
../src/utils/conversion/circ2xml.cpp \
../src/utils/conversion/utils.cpp 

OBJS += \
./src/utils/conversion/CCircleInput.o \
./src/utils/conversion/CXMLOutput.o \
./src/utils/conversion/circ2xml.o \
./src/utils/conversion/utils.o 

CPP_DEPS += \
./src/utils/conversion/CCircleInput.d \
./src/utils/conversion/CXMLOutput.d \
./src/utils/conversion/circ2xml.d \
./src/utils/conversion/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/conversion/%.o: ../src/utils/conversion/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


