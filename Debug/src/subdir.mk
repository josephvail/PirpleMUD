################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CCharacter.cpp \
../src/CItem.cpp \
../src/CMUD.cpp \
../src/CMobile.cpp \
../src/CPlayer.cpp \
../src/CRoom.cpp \
../src/CSocket.cpp \
../src/CTCPClient.cpp \
../src/CTCPServer.cpp \
../src/CUser.cpp \
../src/CZone.cpp \
../src/main.cpp \
../src/pugixml.cpp 

OBJS += \
./src/CCharacter.o \
./src/CItem.o \
./src/CMUD.o \
./src/CMobile.o \
./src/CPlayer.o \
./src/CRoom.o \
./src/CSocket.o \
./src/CTCPClient.o \
./src/CTCPServer.o \
./src/CUser.o \
./src/CZone.o \
./src/main.o \
./src/pugixml.o 

CPP_DEPS += \
./src/CCharacter.d \
./src/CItem.d \
./src/CMUD.d \
./src/CMobile.d \
./src/CPlayer.d \
./src/CRoom.d \
./src/CSocket.d \
./src/CTCPClient.d \
./src/CTCPServer.d \
./src/CUser.d \
./src/CZone.d \
./src/main.d \
./src/pugixml.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


