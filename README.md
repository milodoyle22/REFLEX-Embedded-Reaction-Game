# REFLEX - Embedded Reaction-Time Game

REFLEX is a five-round embedded reaction-time game built around an Arduino Mega 2560. It combines embedded C++, digital electronics, transistor-switched illuminated arcade controls, a 16x2 LCD, audio feedback, permanent perfboard hardware, and a custom enclosure designed in Fusion 360 for 3D printing.

> **Build status:** Core electronics, firmware, perfboard transfer, and enclosure CAD are complete. Final enclosure assembly and full-system validation are still in progress.

## Demo

A working breadboard prototype demonstrates randomized cues, reaction-time measurement, scoring, LCD feedback, and wrong-button handling before the design was transferred to permanent hardware.

**[Watch the REFLEX breadboard prototype demo](docs/demo/REFLEX_Prototype_Demo.mp4)**

## Engineering Highlights

- Built a complete five-round reaction game around an Arduino Mega 2560
- Structured the firmware as a finite-state machine rather than a blocking sequence
- Replaced the original blocking wait logic with non-blocking timing so early presses can be detected
- Measured the illuminated arcade-button LED load at approximately **22 mA**
- Added three 2N2222A NPN low-side driver stages instead of placing the LED load directly on MCU GPIO
- Used Arduino internal pull-ups for simple, reliable active-low button inputs
- Migrated the validated breadboard circuit to soldered perfboard with dedicated +5 V and GND distribution
- Designed a serviceable enclosure in Fusion 360 around the real component dimensions

## System Overview

| Subsystem | Implementation |
| --- | --- |
| Controller | Arduino Mega 2560 |
| Inputs | Red, green, blue, and START arcade-button switches |
| Outputs | 3 illuminated color cues, 16x2 LCD, passive buzzer |
| LED drivers | 3x 2N2222A NPN transistor low-side switches |
| Permanent hardware | Custom soldered perfboard |
| Mechanical design | Fusion 360 enclosure for 3D printing |
| Power | Designed for internal USB power-bank operation |

### Signal Map

| Function | Arduino Pin |
| --- | ---: |
| Blue LED driver | D4 |
| Green LED driver | D5 |
| Red LED driver | D6 |
| Red button | D8 |
| Green button | D9 |
| Blue button | D10 |
| START button | D11 |
| Passive buzzer | D12 |
| LCD RS | D22 |
| LCD E | D23 |
| LCD D4 | D24 |
| LCD D5 | D25 |
| LCD D6 | D26 |
| LCD D7 | D27 |

## Firmware Architecture

The firmware uses five states:

~~~mermaid
stateDiagram-v2
    [*] --> IDLE
    IDLE --> WAITING: START
    WAITING --> RESULT: false start
    WAITING --> CUE_ACTIVE: randomized wait expires
    CUE_ACTIVE --> RESULT: correct or wrong input
    RESULT --> WAITING: next round
    RESULT --> GAME_OVER: round 5 complete
    GAME_OVER --> IDLE: START / reset
~~~

- **IDLE** - waits for the player to start
- **WAITING** - runs a randomized 2-5 second delay while still checking for false starts
- **CUE_ACTIVE** - activates one randomized color cue and measures response time
- **RESULT** - reports the round result and waits to continue
- **GAME_OVER** - displays final score and average successful reaction time

Reaction time is captured with **micros()**, while the waiting state uses non-blocking **millis()** timing so the program can continue checking inputs before the cue appears.

## Hardware Design

The illuminated arcade-button LEDs were measured at approximately **22 mA**. REFLEX therefore uses a transistor stage for each colored LED instead of driving the load directly from an Arduino output.

~~~text
Arduino output -> 1 kOhm -> transistor base
+5 V -> 220 Ohm -> button LED +
button LED - -> transistor collector
transistor emitter -> GND
~~~

The red, green, blue, and START switches use **INPUT_PULLUP**, so each input is HIGH when released and LOW when pressed. The white START-button LED is continuously illuminated as a status indicator.

## Development Process

The project was developed and validated incrementally:

1. Breadboarded the LCD, buzzer, input buttons, and cue LEDs
2. Wrote and debugged the initial reaction-game firmware
3. Identified the limitation of a blocking randomized delay
4. Reworked the game flow into a non-blocking finite-state machine
5. Measured the arcade-button LED current and added transistor drivers
6. Tested all three color-driver channels
7. Transferred the working circuit to soldered perfboard
8. Designed the enclosure around the actual Mega, perfboard, LCD, controls, and power hardware
9. Exported the printable enclosure parts and submitted them for 3D printing

## CAD Files

Printable enclosure exports are available in [cad/stl/](cad/stl/):

- [REFLEX_Shell.stl](cad/stl/REFLEX_Shell.stl)
- [REFLEX_Cover.stl](cad/stl/REFLEX_Cover.stl)
- [REFLEX_Coupon.stl](cad/stl/REFLEX_Coupon.stl)

The coupon is a small print used to validate fit/tolerance before committing to the full enclosure print.

## Repository Structure

~~~text
REFLEX-Embedded-Reaction-Game/
|-- README.md
|-- firmware/
|   |-- REFLEX.ino
|   `-- README.md
|-- hardware/
|   |-- README.md
|   `-- pinout.md
|-- cad/
|   |-- README.md
|   `-- stl/
|-- docs/
|   |-- demo/
|   `-- images/
`-- bom/
    `-- BOM.md
~~~

## Current Build Status

- [x] Breadboard prototype
- [x] Core game firmware
- [x] Non-blocking FSM timing
- [x] LCD interface
- [x] False-start and wrong-button logic
- [x] Transistor LED drivers
- [x] Perfboard transfer
- [x] Three-channel LED hardware test
- [x] Fusion 360 enclosure design
- [x] STL exports
- [x] Enclosure submitted for 3D printing
- [x] Breadboard prototype demo video
- [ ] Final enclosure assembly
- [ ] Final integrated system test
- [ ] Final assembled-device photos
- [ ] Final assembled-device demo

## Tools and Technologies

**Hardware:** Arduino Mega 2560, 2N2222A transistors, 16x2 LCD, illuminated arcade buttons, passive buzzer, perfboard

**Software:** Arduino IDE, C++, Fusion 360, Git/GitHub

**Test Equipment:** Digital multimeter, oscilloscope, bench power supply

**Fabrication:** Breadboarding, soldering, perfboard assembly, 3D printing

## Author

**Milo Doyle**  
Electrical Engineering Student  
California State University, Long Beach  
Expected Graduation: May 2028

---

This repository documents an ongoing engineering build. Final enclosure photos and integrated-system results will be added after assembly.
