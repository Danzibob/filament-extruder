.PHONY: setup build

default: build

setup:
	arduino-cli core install arduino:avr

format:
	find -iname '*.h' -o -iname '*.ino' | xargs clang-format -i

build:
# it would be desirable to use --build-property compiler.cpp.extra_flags=-Werror, but our libraries have warnings so this breaks things
	arduino-cli compile --profile default --warnings all -j 0 .

flash:
	arduino-cli compile -p /dev/ttyUSB* --upload --profile default .

monitor:
	arduino-cli monitor -p /dev/ttyUSB* --fqbn arduino:avr:nano:cpu=atmega328old
