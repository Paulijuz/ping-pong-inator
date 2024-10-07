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

CC := avr-gcc
CFLAGS := $(C_INCLUDES) # Specify all the include files for the compiler.
CFLAGS += -Os # Enable optimalization with repesct to size. (If its not with respect to size the binary will be unstable.)
CFLAGS += -std=c11 # Specify C version.
CFLAGS += -mmcu=$(TARGET_CPU) # Target CPU instructions.
CFLAGS += -ggdb # Tell the compiler to include information for debugging in the final binary file.

# The flags bellow make the binary file unstable some times for some reason. We don't know why, but we'll leave them out for now.
# CFLAGS += -ffunction-sections # Move functions into their own sections.
# CFLAGS += -fdata-sections # Move data into their own sections.

CFLAGS += -Wl,-gc-sections,-print-gc-sections # Do garbage collection on (i.e. remove unused) sections. Also print which sections are removed for debugging purposes.

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