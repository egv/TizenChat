################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AppResourceId.cpp \
../src/GetTokenFormClass.cpp \
../src/LoginFormClass.cpp \
../src/Message.cpp \
../src/SettingsPanelClass.cpp \
../src/TizenChat.cpp \
../src/TizenChatDataManager.cpp \
../src/TizenChatDialogsTab.cpp \
../src/TizenChatEntry.cpp \
../src/TizenChatFormFactory.cpp \
../src/TizenChatFrame.cpp \
../src/TizenChatMainForm.cpp \
../src/TizenChatPanelFactory.cpp \
../src/TizenChatTab2.cpp \
../src/TizenChatTab3.cpp \
../src/Utils.cpp 

OBJS += \
./src/AppResourceId.o \
./src/GetTokenFormClass.o \
./src/LoginFormClass.o \
./src/Message.o \
./src/SettingsPanelClass.o \
./src/TizenChat.o \
./src/TizenChatDataManager.o \
./src/TizenChatDialogsTab.o \
./src/TizenChatEntry.o \
./src/TizenChatFormFactory.o \
./src/TizenChatFrame.o \
./src/TizenChatMainForm.o \
./src/TizenChatPanelFactory.o \
./src/TizenChatTab2.o \
./src/TizenChatTab3.o \
./src/Utils.o 

CPP_DEPS += \
./src/AppResourceId.d \
./src/GetTokenFormClass.d \
./src/LoginFormClass.d \
./src/Message.d \
./src/SettingsPanelClass.d \
./src/TizenChat.d \
./src/TizenChatDataManager.d \
./src/TizenChatDialogsTab.d \
./src/TizenChatEntry.d \
./src/TizenChatFormFactory.d \
./src/TizenChatFrame.d \
./src/TizenChatMainForm.d \
./src/TizenChatPanelFactory.d \
./src/TizenChatTab2.d \
./src/TizenChatTab3.d \
./src/Utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: C++ Compiler'
	clang++.exe -I"pch" -I"C:\Users\egv\workspace\TizenChat\inc" -O3 -g -Wall -c -fmessage-length=0 -target i386-tizen-linux-gnueabi -gcc-toolchain "C:/tizen-sdk/tools/smart-build-interface/../i386-linux-gnueabi-gcc-4.5/" -ccc-gcc-name i386-linux-gnueabi-g++ -march=i386 -Wno-gnu -fPIE --sysroot="C:/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native" -I"C:/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include/libxml2" -I"C:\tizen-sdk\library" -I"C:/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include" -I"C:/tizen-sdk/platforms/tizen2.2/rootstraps/tizen-emulator-2.2.native/usr/include/osp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


