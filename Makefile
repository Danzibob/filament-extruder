.PHONY: setup build

default: build

setup:
	arduino-cli core install arduino:avr
	arduino-cli config set library.enable_unsafe_install true
	find libraries -type f -exec arduino-cli lib install --zip-path {} \;

format:
	find -iname '*.h' -o -iname '*.ino' | xargs clang-format -i

build:
# it would be desirable to use --build-property compiler.cpp.extra_flags=-Werror, but our libraries have warnings so this breaks things
	arduino-cli compile --fqbn arduino:avr:nano --warnings all -j 0  .

flash:
	arduino-cli compile -p /dev/ttyUSB1 --upload --fqbn arduino:avr:nano:cpu=atmega328old .
