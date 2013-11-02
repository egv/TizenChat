################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AppResourceId.cpp \
../src/GetTokenFormClass.cpp \
../src/LoginFormClass.cpp \
../src/TizenChat.cpp \
../src/TizenChatEntry.cpp \
../src/TizenChatFormFactory.cpp \
../src/TizenChatFrame.cpp \
../src/TizenChatMainForm.cpp \
../src/TizenChatPanelFactory.cpp \
../src/TizenChatTab1.cpp \
../src/TizenChatTab2.cpp \
../src/TizenChatTab3.cpp 

OBJS += \
./src/AppResourceId.o \
./src/GetTokenFormClass.o \
./src/LoginFormClass.o \
./src/TizenChat.o \
./src/TizenChatEntry.o \
./src/TizenChatFormFactory.o \
./src/TizenChatFrame.o \
./src/TizenChatMainForm.o \
./src/TizenChatPanelFactory.o \
./src/TizenChatTab1.o \
./src/TizenChatTab2.o \
./src/TizenChatTab3.o 

CPP_DEPS += \
./src/AppResourceId.d \
./src/GetTokenFormClass.d \
./src/LoginFormClass.d \
./src/TizenChat.d \
./src/TizenChatEntry.d \
./src/TizenChatFormFactory.d \
./src/TizenChatFrame.d \
./src/TizenChatMainForm.d \
./src/TizenChatPanelFactory.d \
./src/TizenChatTab1.d \
./src/TizenChatTab2.d \
./src/TizenChatTab3.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: C++ Compiler'
	clang++.exe -I"pch" -D_DEBUG -I"C:\Users\egv\workspace\TizenChat\inc" -O0 -g3 -Wall -c -fmessage-length=0 -target i386-tizen-linux-gnueabi -gcc-toolchain "C:/tizen-sdk/tools/smart-build-interface/../i386-linux-gnueabi-gcc-4.5/" -ccc-gcc-name i386-linux-gnueabi-g++ -march=i386 -Wno-gnu -fPIE --sysroot="C:/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native" -I"C:/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include/libxml2" -I"C:\tizen-sdk\library" -I"C:/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include" -I"C:/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include/osp" -D_APP_LOG -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


