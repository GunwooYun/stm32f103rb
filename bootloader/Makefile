# Toolchain setup
TOOLCHAIN_PATH = ../toolchain/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi
CC = $(TOOLCHAIN_PATH)/bin/arm-none-eabi-gcc
OBJCOPY = $(TOOLCHAIN_PATH)/bin/arm-none-eabi-objcopy
SIZE = $(TOOLCHAIN_PATH)/bin/arm-none-eabi-size

# Target
TARGET = bootloader

# Architecture Cortex-M3
MCU = -mcpu=cortex-m3 -mthumb

# 컴파일러 옵션
CFLAGS = $(MCU) -Wall -O0 -g -ffunction-sections -fdata-sections --specs=nosys.specs

# 링커 옵션
LDFLAGS = $(MCU) -T scripts/bootloader.ld -Wl,--gc-sections -Wl,-Map=$(TARGET).map

# 소스 파일 목록
SRCS = src/main.c src/startup.c
OBJS = $(SRCS:.c=.o)

# 빌드 규칙
all: $(TARGET).bin

$(TARGET).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin $(TARGET).map

.PHONY: all clean
