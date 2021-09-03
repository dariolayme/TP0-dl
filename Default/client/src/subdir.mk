################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../client/src/client.c \
../client/src/utils.c 

OBJS += \
./client/src/client.o \
./client/src/utils.o 

C_DEPS += \
./client/src/client.d \
./client/src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
client/src/%.o: ../client/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


