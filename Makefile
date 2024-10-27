rgu-labs:
	@mkdir -p build/release
	@cp -r files/ build/release/files
	@cd build
	@cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain_linux.cmake build/
	@ninja -C build/
	@lldb build/release/rgu-labs