# REFLEX - Embedded Reaction-Time Game

REFLEX is a five-round embedded reaction-time game built around an Arduino Mega 2560. The project combines embedded C++, digital electronics, transistor-driven illuminated arcade controls, a 16x2 LCD, audio feedback, custom soldered perfboard hardware, and a 3D-printed enclosure designed in Fusion 360.

## Project Goals

REFLEX was built as a hands-on electrical engineering project focused on the complete development cycle:

1. prototype the circuit on a breadboard,
2. write and debug the embedded firmware,
3. measure and validate hardware behavior,
4. redesign the output stage where needed,
5. transfer the circuit to permanent perfboard,
6. design a serviceable enclosure,
7. integrate and test the complete system.

## Features

- Randomized red, green, and blue reaction cues
- Five-round game flow
- Reaction-time measurement
- False-start detection
- Wrong-button detection
- Score tracking
- Best and average reaction-time reporting
- 16x2 LCD user feedback
- Passive buzzer for audio feedback
- Separate START/NEXT control
- Finite-state-machine firmware architecture
- Illuminated arcade buttons
- Custom perfboard power distribution and transistor driver circuitry
- Internal USB power-bank operation
- Custom Fusion 360 enclosure

## System Overview

The system is organized into four main blocks:

- **Controller:** Arduino Mega 2560
- **User interface:** 3 colored arcade buttons, white START button, 16x2 LCD, passive buzzer
- **Driver hardware:** 2N2222A NPN transistor stages for the red, green, and blue illuminated buttons
- **Power/mechanical integration:** perfboard power rails, internal USB power bank, and a 3D-printed enclosure

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

## Hardware Design

The illuminated arcade buttons were tested at approximately **22 mA** LED current. Rather than placing that load directly on the Arduino GPIO pins, REFLEX uses three NPN transistor switching stages.

Each colored LED channel follows this general structure:

```text
Arduino output -> 1 kOhm -> transistor base
+5 V -> 220 Ohm -> button LED +
button LED - -> transistor collector
transistor emitter -> GND
```

The red, green, and blue button switches are read independently using the Arduino's internal pull-up resistors.

The white START button uses the same switch arrangement, while its LED remains continuously illuminated as a power/status indicator.

## Firmware Architecture

The game firmware is organized as a finite-state machine with the following states:

- `IDLE`
- `WAITING`
- `CUE_ACTIVE`
- `RESULT`
- `GAME_OVER`

This structure separates game flow from input handling and timing logic. Reaction time is captured using microsecond-resolution timing, while a randomized delay prevents the user from predicting when the next cue will appear.

## Development and Testing

REFLEX began as a breadboard prototype and was tested subsystem-by-subsystem before permanent assembly.

Key development steps included:

- validating arcade-button switch behavior,
- measuring LED current with a digital multimeter,
- adding transistor driver stages,
- debugging button-state and hold behavior,
- validating all three LED channels independently,
- transferring the working circuit to perfboard,
- creating dedicated +5 V and GND distribution rails,
- soldering and testing permanent wiring,
- designing the enclosure around the real component dimensions.

## Repository Structure

```text
REFLEX-Embedded-Reaction-Game/
|
|-- README.md
|-- firmware/
|-- hardware/
|   |-- schematic/
|   |-- perfboard/
|   `-- wiring/
|-- cad/
|   |-- source/
|   `-- stl/
|-- docs/
|   `-- images/
|-- bom/
|   `-- BOM.md
`-- LICENSE
```

## Current Build Status

- [x] Breadboard prototype
- [x] Core game firmware
- [x] LCD interface
- [x] False-start and wrong-button logic
- [x] Transistor LED drivers
- [x] Perfboard transfer
- [x] Three-channel LED hardware test
- [x] Fusion 360 enclosure design
- [x] Enclosure submitted for 3D printing
- [ ] Final enclosure assembly
- [ ] Final integrated system test
- [ ] Final project photos and demo video

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

This repository documents an ongoing engineering build. Final CAD exports, firmware, hardware documentation, and project photos will be added as the system reaches final assembly.
