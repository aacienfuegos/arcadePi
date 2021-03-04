################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../arkanoPi.c \
../arkanoPiLib.c \
../fsm.c \
../kbhit.c \
../ledDisplay.c \
../pseudoWiringPi.c \
../teclado_TL04.c \
../tmr.c 

OBJS += \
./arkanoPi.o \
./arkanoPiLib.o \
./fsm.o \
./kbhit.o \
./ledDisplay.o \
./pseudoWiringPi.o \
./teclado_TL04.o \
./tmr.o 

C_DEPS += \
./arkanoPi.d \
./arkanoPiLib.d \
./fsm.d \
./kbhit.d \
./ledDisplay.d \
./pseudoWiringPi.d \
./teclado_TL04.d \
./tmr.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


