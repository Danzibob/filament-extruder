.PHONY: setup build

default: build

setup:
	arduino-cli core install arduino:avr
	arduino-cli config set library.enable_unsafe_install true
	find libraries -type f -exec arduino-cli lib install --zip-path {} \;

format:
	find -iname '*.h' -o -iname '*.ino' | xargs clang-format -i --style=webkit

build:
	arduino-cli compile --fqbn arduino:avr:nano .
