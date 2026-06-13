# Hardware

## Pin Assignments

| Pin | Direction | Connected to | Notes |
|---|---|---|---|
| D2 | OUT | Puller DIR | |
| D3 | OUT | Distributor DIR | |
| D4 | OUT | Spool DIR | |
| D9 | OUT | Distributor STEP | Timer1 OC1A hardware PWM |
| D10 | OUT | Puller STEP | Timer1 OC1B hardware PWM |
| D7 | OUT | Spool STEP | Software-timed via `millis()` |
| D8 | OUT | Stepper ENABLE | Active LOW; shared across all three drivers |
| A0 | IN | Encoder CLK | RotaryEncoder pin 2 |
| A1 | IN | Encoder DT | RotaryEncoder pin 1 |
| A2 | IN | Encoder SW | Button, INPUT_PULLUP; click on release |
| A3 | OUT | Encoder VCC | Driven HIGH to power the encoder module |
| A4 | I²C SDA | LCD | LiquidCrystal_I2C |
| A5 | I²C SCL | LCD | LiquidCrystal_I2C |
| A7 | IN (ADC) | Width sensor | Analog; 10-bit ADC, mapped via LUT |

## Connection Diagram

```mermaid
%%{init: {'layout': 'elk'}}%%
graph LR
    subgraph nano["Arduino Nano"]
        direction TB
        d2["D2"] & d3["D3"] & d4["D4"]
        d7["D7"]
        d9["D9 OC1A"] & d10["D10 OC1B"]
        d8["D8 ENABLE"]
        a0["A0"] & a1["A1"] & a2["A2"] & a3["A3"]
        a4["A4 SDA"] & a5["A5 SCL"]
        a7["A7"]
    end

    subgraph drivers["Stepper Drivers (A4988 / DRV8825)"]
        puller["Puller"]
        distrib["Distributor"]
        spool["Spool"]
    end

    subgraph enc["Rotary Encoder"]
        eDT["DT"]
        eCLK["CLK"]
        eSW["SW"]
        eVCC["VCC"]
    end

    d2 -->|DIR| puller
    d10 -->|STEP| puller
    d3 -->|DIR| distrib
    d9 -->|STEP| distrib
    d4 -->|DIR| spool
    d7 -->|STEP| spool
    d8 -->|ENABLE| puller
    d8 -->|ENABLE| distrib
    d8 -->|ENABLE| spool

    a1 --> eDT
    a0 --> eCLK
    a2 --> eSW
    a3 -->|3.3 V supply| eVCC

    a4 --> lcd["LCD 16×2<br>I²C 0x27"]
    a5 --> lcd
    a7 --> sensor["Width Sensor<br>(analog)"]
```

## MCU Peripheral Usage

```mermaid
%%{init: {'layout': 'elk'}}%%
graph TD
    subgraph periph["ATmega328P Peripherals"]
        t1["Timer1<br>Fast PWM mode 15<br>TOP = OCR1A<br>prescaler clk/1024"]
        adc["ADC<br>10-bit"]
        twi["TWI / I²C"]
        gpio["GPIO"]
    end

    t1 -->|"OC1A → D9"| distrib_step["Distributor STEP<br>f = F_CPU / (1024 × OCR1A)"]
    t1 -->|"OC1B → D10"| puller_step["Puller STEP<br>duty ≈ OCR1B / OCR1A"]

    adc -->|"A7 analogRead()"| width["Filament width<br>LUT interpolation"]

    twi -->|"0x27 @ 100 kHz"| lcd2["LCD 16×2"]

    gpio -->|"D2–D4 DIR, D7/D9/D10 STEP, D8 EN"| steppers["Stepper control"]
    gpio -->|"A0–A3 quadrature/btn/VCC"| enc2["Encoder"]
```

### Timer1 Detail

Timer1 produces hardware step pulses for the puller and distributor motors, leaving Timer0 untouched so `millis()`/`delay()`/`micros()` remain accurate:

- **Mode**: Fast PWM, TOP = OCR1A (mode 15: WGM13:0 = 1111)
- **OC1A** (D9, Distributor STEP): toggles at TOP → 50 % square wave
- **OC1B** (D10, Puller STEP): clears at OCR1B, sets at BOTTOM → duty ≈ 50 % when `OCR1B = OCR1A/2`
- **Frequency formula**: `f = F_CPU / (prescaler × (OCR1A + 1))`
- `setInterval(n)` sets `OCR1A = 2n`, `OCR1B = n`

The spool motor (`D7`) is driven by software timing using `millis()` in `stepper::spool::tick()`.
