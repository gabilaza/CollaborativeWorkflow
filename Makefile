
BUILD_MODE ?= Debug

build: clean
	@echo "Build in ${BUILD_MODE} mode"
	mkdir -p build/${BUILD_MODE}
	@cd build/${BUILD_MODE}; cmake ../../ -DCMAKE_BUILD_TYPE=${BUILD_MODE}
	@cd build/${BUILD_MODE}; make

clean:
	@rm -rf build/${BUILD_MODE}

clean-builds:
	@rm -rf build

