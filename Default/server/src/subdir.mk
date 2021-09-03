################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../server/src/server.c \
../server/src/utils.c 

OBJS += \
./server/src/server.o \
./server/src/utils.o 

C_DEPS += \
./server/src/server.d \
./server/src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
server/src/%.o: ../server/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


