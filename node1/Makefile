# List all source files to be compiled; separate with space
SOURCE_FILES := $(patsubst src/%, %, $(shell find "src" -name '*.c'))
C_INCLUDES := -Iinc

# Set this flag to "yes" (no quotes) to use JTAG; otherwise ISP (SPI) is used
PROGRAM_WITH_JTAG := yes

# Feel free to ignore anything below this line
PROGRAMMER := atmelice_isp
ifeq ($(PROGRAM_WITH_JTAG), yes)
	PROGRAMMER := atmelice
endif

BUILD_DIR := build
TARGET_CPU := atmega162
TARGET_DEVICE := m162

# Okay so somtiems the mcu goes in to a crash loop, i.e. it starts and almost immediatly crashes and then starts again and so on.
# What we've narrowed it down to is that is probably has something to do with the binary file. Either it is corrupted,
# uses too much stack or something else. We though that disabling ffuntion-sections and fdata-sections flags and turning on
# optimalization with respect to size (the Os flag) would help. And it did... until it didn't. So we're still not sure what
# the problem is. But for now playing with the optimalization flag and the section flags seems like the best option.

# Skratch all that. The MCU was f**ked.

CC := avr-gcc
CFLAGS := $(C_INCLUDES) # Specify all the include files for the compiler.
CFLAGS += -O # Enable optimalization with repesct to size.
CFLAGS += -std=c11 # Specify C version.
CFLAGS += -mmcu=$(TARGET_CPU) # Target CPU instructions.
CFLAGS += -ggdb # Tell the compiler to include information for debugging in the final binary file.
CFLAGS += -fshort-enums # Tell the compiler to use the smallest possible data type for enums.
CFLAGS += -ffunction-sections # Move functions into their own sections.
CFLAGS += -fdata-sections # Move data into their own sections.

CFLAGS += -Wl,--gc-sections,--print-gc-sections # Do garbage collection on (i.e. remove unused) sections. Also print which sections are removed for debugging purposes.

OBJECT_FILES = $(SOURCE_FILES:%.c=$(BUILD_DIR)/%.o)

.DEFAULT_GOAL := $(BUILD_DIR)/main.hex

$(BUILD_DIR):
	@mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	@mkdir -p $(@D)
	$(CC) -Wall $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.hex: $(OBJECT_FILES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJECT_FILES) -o $(BUILD_DIR)/a.out
	avr-objcopy -j .text -j .data -O ihex $(BUILD_DIR)/a.out $(BUILD_DIR)/main.hex

.PHONY: flash
flash: $(BUILD_DIR)/main.hex
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)/main.hex:i

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: erase
erase:
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -e
	
.PHONY: debug
debug:
	if pgrep avarice; then pkill avarice; fi
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)/main.hex:i
	x-terminal-emulator -e avarice --edbg --ignore-intr :4242
	sleep 2
	avr-gdb -tui -iex "target remote localhost:4242" $(BUILD_DIR)/a.out
	killall -s 9 avarice	