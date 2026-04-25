# Extruder

Arduino Nano firmware for the Felfil filament spooler. Controls three stepper motors (puller, distributor, spool) with a PID loop to maintain target filament diameter, driven by an analog width sensor. User interface is a rotary encoder and 16×2 I²C LCD.

## Hardware

- Arduino Nano (ATmega328P, old bootloader)
- Three A4988/DRV8825 stepper drivers sharing a common enable pin
- Analog filament width sensor on A7
- KY-040 rotary encoder on A0–A3
- 16×2 LCD via I²C at 0x27

## Building

Requires [arduino-cli](https://arduino.github.io/arduino-cli/).

```sh
make setup   # install Arduino AVR core (libraries are fetched automatically at build time)
make build   # compile
make flash   # compile and upload
make monitor # open serial monitor
```

## Docs

Architecture, pin assignments, control flow and UI state diagrams are in [`docs/diagrams/`](docs/diagrams/).
