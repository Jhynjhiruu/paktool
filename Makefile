all: paktool.z64
.PHONY: all

BUILD_DIR = build
SOURCE_DIR = src
include $(N64_INST)/include/n64.mk
N64_ROM_CONTROLLER1 = n64,pak=controller
N64_ROM_REGIONFREE = true

OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/menu.o $(BUILD_DIR)/read.o $(BUILD_DIR)/rw.o $(BUILD_DIR)/write.o

N64_CFLAGS += -std=gnu23 -Os -DNDEBUG

paktool.z64: N64_ROM_TITLE = "Controller pak tool"

$(BUILD_DIR)/paktool.elf: $(OBJS)

# Clean code
clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
