# Module Architecture

## Dependency Graph

```mermaid
%%{init: {'layout': 'elk'}}%%
graph TD
    main["extruder.ino<br>― setup() / loop() ―"]

    main --> display
    main --> eeprom
    main --> encoder
    main --> pid
    main --> sensor
    main --> stepper

    display --> encoder
    display --> pid
    display --> sensor
    display --> stepper

    eeprom --> pid
    eeprom --> sensor

    encoder --> stepper

    pid --> sensor
    pid --> stepper

    sensor -.->|"setOffset() triggers eeprom::update()"| eeprom

    encoder --- RotaryEncoder(["RotaryEncoder lib"])
    pid      --- PID_lib(["PID lib"])
    display  --- LCD_lib(["LiquidCrystal_I2C lib"])
    eeprom   --- EEPROM_lib(["EEPROM built-in"])

    style main fill:#dde,stroke:#669
    style RotaryEncoder fill:#efe,stroke:#696
    style PID_lib fill:#efe,stroke:#696
    style LCD_lib fill:#efe,stroke:#696
    style EEPROM_lib fill:#efe,stroke:#696
```

## Module Responsibilities

| Module | Namespace | Responsibility |
|---|---|---|
| `extruder.ino` | — | Top-level setup/loop; stepper enable/disable logic |
| `stepper.ino` | `stepper::` | GPIO and Timer0 PWM for three stepper motors |
| `sensor.ino` | `sensor::` | ADC read, exponential smoothing, LUT interpolation for filament width |
| `pid.ino` | `pid::` | Dual-gain PID loop; maps output to puller step interval |
| `encoder.ino` | `encoder::` | Polls RotaryEncoder, debounces button, exposes `up`/`down`/`clicked` |
| `display.ino` | `display::` | LCD state machine — setup wizard and running menu |
| `eeprom.ino` | `eeprom::` | Persists PID setpoint and sensor offset across power cycles |

## Notes

- `sensor` and `eeprom` have a runtime cycle: `sensor::setOffset()` immediately calls `eeprom::update()` to persist the change. The header-level include graph is acyclic (sensor.h does not include eeprom.h).
- `encoder.h` includes `stepper.h` solely to inherit the `clamp()` macro defined there.
