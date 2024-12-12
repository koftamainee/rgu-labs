BUILD_DIR = build
BUILD_TYPE ?= Release
TARGET = rgu-labs
LLDB = lldb
RELEASE_DIR = release

TOOLCHAIN_LINUX = ../cmake/toolchain_linux.cmake
TOOLCHAIN_WINDOWS = ../cmake/toolchain_windows.cmake

all: docker

.PHONY: clean run debug release

debug: BUILD_TYPE = Debug
debug: build_linux
	cd $(BUILD_DIR)/x86_64 && $(LLDB) $(TARGET)

run: BUILD_TYPE = Debug
run: build_linux
	cd $(BUILD_DIR)/x86_64 && ./$(TARGET) $(f)

build_linux_with_make:
	make clean
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -G Unix\ Makefiles -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN_LINUX) -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ..
	cd $(BUILD_DIR) && make

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
	docker run -it --rm rgu-labs $(f)

release:
	@rm -rf release
	@mkdir -p release
	@touch release/build.log
	@echo "[==>] Starting release build." | tee -a release/build.log
	@echo "[==>] Done." | tee -a release/build.log
	@echo ""

	@echo "[==>] Stage 1: Building for linux..." | tee -a release/build.log
	make build_linux >> release/build.log
	cp -r build/x86_64/ release/rgu-labs_v$(v)_linux >> release/build.log
	@echo "[==>] Done." | tee -a release/build.log
	@echo ""
	@echo "[==>] Cleaning..." | tee -a release/build.log
	make clean >> release/build.log
	@echo "[==>] Done." | tee -a release/build.log
	@echo ""

	@echo "[==>] Stage 2: Building for Windows..." | tee -a release/build.log
	make build_windows >> release/build.log
	cp -r build/x86_64/ release/rgu-labs_v$(v)_windows >> release/build.log
	@echo "[==>] Done." | tee -a release/build.log
	@echo ""
	@echo "[==>] Cleaning..." | tee -a release/build.log
	make clean >> release/build.log
	@echo "[==>] Done." | tee -a release/build.log
	@echo ""

	@echo "[==>] Stage 3: Compressing..." | tee -a release/build.log
	zip -r release/rgu-labs_v$(v)_linux.zip release/rgu-labs_v$(v)_linux >> release/build.log
	zip -r release/rgu-labs_v$(v)_windows.zip release/rgu-labs_v$(v)_windows >> release/build.log
	rm -rf release/rgu-labs_v$(v)_windows release/rgu-labs_v$(v)_linux
	@echo "[==>] Done." | tee -a release/build.log
	@echo ""

	@echo "[==>] Stage 4: Create new git release branch..." | tee -a release/build.log
	git checkout -b release-v$(v) >> release/build.log
	git push origin release-v$(v) >> release/build.log
	git checkout master >> release/build.log
	@echo "[==>] Done." | tee -a release/build.log
	@echo ""

	@echo "[==>] Stage 5: Creating new docker release" | tee -a release/build.log
	docker build -t rgu-labs . >> release/build.log
	docker login >> release/build.log
	docker tag rgu-labs koftamainee/rgu-labs:$(v) >> release/build.log
	docker push koftamainee/rgu-labs:$(v) >> release/build.log
	@echo "[==>] Done." | tee -a release/build.log
	@echo ""

	@echo ""
	@echo "-------------------------------------------------------------------------------------------"
	@echo ""
	@echo "Release build v$(v) finished. Logs stored in release/build.log."
	@echo ""
	@echo "☆ Have a good day ☆"
	@echo ""

	


clean:
	rm -rf $(BUILD_DIR)
