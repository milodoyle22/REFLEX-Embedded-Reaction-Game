# REFLEX Pinout

## Arduino Mega 2560

| Function | Pin | Direction |
| --- | ---: | --- |
| Blue LED transistor driver | D4 | Output |
| Green LED transistor driver | D5 | Output |
| Red LED transistor driver | D6 | Output |
| Red arcade switch | D8 | Input pull-up |
| Green arcade switch | D9 | Input pull-up |
| Blue arcade switch | D10 | Input pull-up |
| START arcade switch | D11 | Input pull-up |
| Passive buzzer | D12 | Output |
| LCD RS | D22 | Output |
| LCD E | D23 | Output |
| LCD D4 | D24 | Output |
| LCD D5 | D25 | Output |
| LCD D6 | D26 | Output |
| LCD D7 | D27 | Output |

## Arcade Button Wiring

### Red / Green / Blue LED Channels

```text
Arduino output -> 1 kOhm -> NPN base
NPN emitter -> GND
NPN collector -> button LED negative
+5 V -> 220 Ohm -> button LED positive
```

### Switch Inputs

```text
Arduino input -> switch -> GND
```

The firmware uses `INPUT_PULLUP`, so released = HIGH and pressed = LOW.

## White START Button

```text
D11 -> switch -> GND

+5 V -> 220 Ohm -> START LED +
START LED - -> GND
```

The white START LED is always illuminated while the system is powered.
