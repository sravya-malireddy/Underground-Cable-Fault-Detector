# Underground Cable Fault Detector

An embedded system for detecting short-circuit faults in underground cable lines and estimating the distance of the fault from the feeder end.

The project uses a resistor-based cable model to represent different sections of an underground cable. When a short-circuit fault occurs, the voltage across the corresponding section changes. The voltage is measured using the ADC of an ATmega16 microcontroller, processed to estimate the fault location, and displayed on a 16×2 LCD.

## Project Overview

Locating faults in underground cables can be difficult because the cable and the fault point are not directly visible.

This project demonstrates a simple embedded approach for identifying the approximate location of a cable fault.

The prototype represents the three cable phases using three resistor networks:


Phase R  →  Resistor Network  →  ADC
Phase Y  →  Resistor Network  →  ADC
Phase B  →  Resistor Network  →  ADC
                                      |
                                      v
                                  ATmega16
                                      |
                                      v
                                  16×2 LCD

Each resistor represents a section of cable at a particular distance. A short circuit changes the voltage measured across the network. The controller uses this change to determine the approximate fault distance.

Working Principle

The system is based on the voltage-current relationship described by Ohm's law.

A low DC voltage is applied at the feeder end through the resistor network representing the cable. When a line-to-ground short circuit occurs, the resistance between the feeder and the fault point determines the resulting current and voltage drop.

The changed voltage is measured using the microcontroller's ADC and converted into digital data. The controller then processes the measurement and displays the estimated fault distance on the LCD.

The project specifically focuses on short-circuit faults. The original project documentation identifies open-circuit fault detection as a possible future extension.

System Workflow
                  Start
                    |
                    v
             Initialize System
                    |
                    v
          Apply DC Test Voltage
                    |
                    v
        Scan R / Y / B Cable Lines
                    |
                    v
             Read ADC Values
                    |
                    v
          Detect Voltage Change
                    |
                    v
          Calculate Fault Distance
                    |
                    v
          Display Result on LCD
                    |
                    v
                  Repeat
Hardware
ATmega16 microcontroller
16×2 LCD
Resistor networks representing underground cable sections
DC power supply
ADC inputs
General-purpose PCB / prototype board
Connecting wires
Supporting resistors and components
Microcontroller

The system uses the ATmega16 AVR microcontroller.

Relevant features used by the project include:

10-bit ADC
Multiple ADC channels
GPIO ports
Timers/Counters
Embedded program memory
LCD interfacing capability

The ATmega16 provides an 8-channel ADC for converting analog input signals into digital values used by the controller.

Three-Phase Fault Detection

The prototype contains three resistor rows representing:

R Phase
Y Phase
B Phase

Each phase is monitored through an ADC channel. The resistor sections represent different cable distances, allowing the measured voltage variation to be associated with the approximate location of the fault.

Fault Detection

The project is designed primarily for:

Line-to-ground short-circuit faults

When a fault occurs, the voltage across the cable representation changes. The ADC detects this change and the microcontroller uses the measurement to estimate the distance of the fault from the feeder end.

Display

A 16×2 LCD is used to display the detected fault information.

Example:

FAULT: PHASE R
DIST: 2.00 KM

The exact displayed distance depends on the calibration of the resistor network.

Software

The embedded software is responsible for:

Initializing the ATmega16
Configuring ADC channels
Reading analog voltage levels
Comparing phase measurements
Calculating the estimated fault distance
Updating the LCD display
Development Tools
Embedded C
Keil μVision
Proteus ISIS

<img width="900" height="1058" alt="WhatsApp Image 2026-08-06 at 20 18 59" src="https://github.com/user-attachments/assets/69c492f2-573d-432b-8c7a-bcc6021a74d2" />

<img width="2034" height="2644" alt="WhatsApp Image 2026-08-06 at 20 18 59 (1)" src="https://github.com/user-attachments/assets/e1daa182-505a-490c-9786-01483f8a8193" />


The project was implemented as a physical prototype using a general-purpose PCB, resistor networks, microcontroller circuitry and an LCD display.

The resistor network represents the three cable phases, while the controller processes the measured voltage changes.

Applications

The underlying concept can be applied to:

Underground cable monitoring
Distribution cable fault detection
Electrical maintenance systems
Fault-location demonstration systems
Embedded power-system monitoring
Limitations
The prototype focuses on short-circuit faults.
The resistor network represents a simplified cable model rather than an actual underground cable.
Fault-distance accuracy depends on resistor values and calibration.
The system does not currently address open-circuit faults.
Future Scope

Possible extensions include:

Open-circuit fault detection
Improved fault-distance calibration
Higher-resolution measurement
Digital data logging
Remote fault reporting
IoT-based monitoring
Real-time fault monitoring
Improved protection and isolation mechanisms

The original project documentation also identifies open-circuit fault detection as a future extension.

Key Learning

This project provided practical exposure to:

Embedded C programming
AVR microcontroller architecture
ADC interfacing
Analog voltage measurement
LCD interfacing
Fault detection logic
Resistor-network modeling
PCB assembly
Hardware-software integration
Project Information
| Category | Details |
|---|---|
| Project | Underground Cable Fault Detector |
| Domain | Embedded Systems |
| Controller | ATmega16 |
| Programming | Embedded C |
| Measurement | ADC |
| Display | 16×2 LCD |
| Fault Type | Short-circuit / line-to-ground |
| Phases | R, Y, B |
| Development | Keil μVision |
| Simulation | Proteus ISIS |
| Project Type | Academic Hardware Project |
