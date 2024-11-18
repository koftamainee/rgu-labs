BUILD_DIR = build
BUILD_TYPE ?= Release
TARGET = rgu-labs
LLDB = lldb

TOOLCHAIN_LINUX = ../cmake/toolchain_linux.cmake
TOOLCHAIN_WINDOWS = ../cmake/toolchain_windows.cmake

all: docker

.PHONY: clean run debug

# Override BUILD_TYPE for debug build
debug: BUILD_TYPE = Debug
debug: build_linux
	cd $(BUILD_DIR)/x86_64 && $(LLDB) $(TARGET)

run: BUILD_TYPE = Debug
run: build_linux
	cd $(BUILD_DIR)/x86_64 && ./$(TARGET)

build_linux:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN_LINUX) -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ..
	cd $(BUILD_DIR) && ninja

build_windows:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN_WINDOWS) -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ..
	cd $(BUILD_DIR) && ninja

docker: clean
	docker build -t rgu-labs .
	docker run -it --rm rgu-labs

clean:
	rm -rf $(BUILD_DIR)
