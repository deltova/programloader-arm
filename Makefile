TOOLCHAIN=./gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi

CC=$(TOOLCHAIN)-gcc
OBJCPY=$(TOOLCHAIN)-objcopy
GDB=$(TOOLCHAIN)-gdb
ST=st
FLASH=$(ST)-flash

CFLAGS=-std=c99 -nostartfiles -Wl,-gc-sections -static -g3 -O0 -mcpu=cortex-m4 \
			 -mthumb -mapcs -Isrc/

LDSCRIPT=linker/script.ld
SOURCES=src/main.c src/startup.c
OBJ=$(SOURCES:.c=.o)
BIN=out.bin
ELF=$(BIN).elf

all: $(BIN)

$(BIN): $(ELF)
	$(OBJCPY) -O binary -S $@.elf $@

$(ELF): $(OBJ)
	$(CC)	$(CFLAGS) -T $(LDSCRIPT) $^ -o $@

gdb:
	$(GDB) $(ELF) -ix gdbcmd

flash: $(BIN)
	@echo "Flashing"
	$(FLASH)  --reset write $(BIN) 0x8000000
	@echo "Done"

clean:
	$(RM) -f $(BIN)
	$(RM) -f $(OBJ)
	$(RM) -f $(BIN)
	$(RM) -f $(ELF)
