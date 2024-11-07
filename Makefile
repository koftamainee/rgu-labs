BUILD_DIR = build
TARGET = rgu-labs
LLDB = lldb

TOOLCHAIN_LINUX = ../cmake/toolchain_linux.cmake
TOOLCHAIN_WINDOWS = ../cmake/toolchain_windows.cmake

all: debug

.PHONY: clean

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN_LINUX) -DCMAKE_BUILD_TYPE=Debug ..

$(BUILD_DIR)/x86_64/$(TARGET): $(BUILD_DIR)
	cd $(BUILD_DIR) && ninja

run: $(BUILD_DIR)/x86_64/$(TARGET)
	cd $(BUILD_DIR)/x86_64 && ./$(TARGET)

debug: $(BUILD_DIR)/x86_64/$(TARGET)
	cd $(BUILD_DIR)/x86_64 && $(LLDB) $(TARGET)

build_linux:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN_LINUX) -DCMAKE_BUILD_TYPE=Release ..
	cd $(BUILD_DIR) && ninja

build_windows:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN_WINDOWS) -DCMAKE_BUILD_TYPE=Release ..
	cd $(BUILD_DIR) && ninja

docker: clean
	docker build -t rgu-labs .
	docker run -it --rm rgu-labs

clean:
	rm -rf $(BUILD_DIR)
