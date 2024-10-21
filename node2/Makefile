SOURCE_DIR := src/
BUILD_DIR := build/
MODULES_DIR := inc/
INCLUDE_DIR := $(MODULES_DIR) sam/sam3x/include sam/sam3x/source sam/interrupt sam/cmsis sam

SOURCE_FILES := $(foreach sdir,$(SOURCE_DIR),$(wildcard $(sdir)*.c))
SOURCE_FILES += $(foreach sdir,$(MODULES_DIR),$(wildcard $(sdir)*.c))
CPPCHECK_SRC := $(SOURCE_FILES)
SOURCE_FILES += $(foreach sdir,sam/sam3x/source/,$(wildcard $(sdir)*.c))
OBJECT_FILES := $(subst $(MODULES_DIR),$(BUILD_DIR),$(SOURCE_FILES:.c=.o))
OBJECT_FILES := $(subst $(SOURCE_DIR),$(BUILD_DIR),$(OBJECT_FILES))
INCLUDE_DIR := $(addprefix -I,$(INCLUDE_DIR))

VPATH:= $(dir $(SOURCE_FILES))
CC:=arm-none-eabi-gcc
LD:=arm-none-eabi-gcc
AS:=arm-none-eabi-as

LDSCRIPT = sam/flash.ld
TARGET_CPU = __SAM3X8E__
LDFLAGS:= -T$(LDSCRIPT) -mthumb -mcpu=cortex-m3 -Wl,--gc-sections
CFLAGS:= -mcpu=cortex-m3 -mthumb -ggdb -std=c11 -Wall -Os
CFLAGS+= $(INCLUDE_DIR)
CFLAGS+= -D $(TARGET_CPU)
# do not uncomment if you have epilepsy
#CFLAGS+= -Wpadded -Wall -Wextra -Wshadow -Wdouble-promotion -Wformat=2 -Wundef -fno-common -Wconversion -fstack-usage -Wstack-usage=255 -ffunction-sections -fdata-sections 

# QUITE := @

ELF=$(BUILD_DIR)main.elf
.DEFAULT_GOAL := $(ELF)
$(BUILD_DIR):
	$(QUITE) mkdir $(BUILD_DIR)

$(BUILD_DIR)%.o : %.c | $(BUILD_DIR)
	$(QUITE) $(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)%.o : %.s | $(BUILD_DIR)
	$(QUITE) $(AS) $< -o $@

$(ELF) : $(OBJECT_FILES) | $(BUILD_DIR)
	$(QUITE) $(LD) $(LDFLAGS) -o $@ $(OBJECT_FILES) $(LDLIBS)

.PHONY: clean
clean:
	$(QUIET) rm -rf $(BUILD_DIR) $(DOCS_DIR)

.PHONY: flash
flash: $(ELF)
	$(QUITE) openocd -d0 -f sam/openocd.cfg -c "program $(ELF) reset exit"

.PHONY: check
check:
	cppcheck --enable=all $(CPPCHECK_SRC) $(INCLUDE_DIR) --language=c -q $(TARGET_CPU)

.PHONY: docs
docs: $(DOCS_DIR)
	doxygen 
