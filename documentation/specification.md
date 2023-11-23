# LAB DC POWER SUPPLY
## Hardware Requirements Specification
### Version 1.1

---

### Revision History

| Date       | Version | Description             | Author        |
|------------|---------|-------------------------|---------------|
| 18.09.2023 | 1.0     | Initial Version         | Markus Marandi|
| 20.09.2023 | 1.1     | Added GANTT chart       | Markus Marandi|

---

## Table of Contents

1. [Introduction](#introduction)
    - [Purpose](#purpose)
    - [Scope](#scope)
    - [Definitions, Acronyms, and Abbreviations](#definitions-acronyms-and-abbreviations)
2. [Overall Description](#overall-description)
    - [Product Perspective](#product-perspective)
    - [Product Functions and Components](#product-functions-and-components)
    - [Constraints](#constraints)
    - [Assumptions and Dependencies](#assumptions-and-dependencies)
3. [Specific Requirements](#specific-requirements)
    - [External Interface Requirements](#external-interface-requirements)
    - [Performance Requirements](#performance-requirements)
    - [Safety Requirements](#safety-requirements)
    - [Environmental Requirements](#environmental-requirements)
    - [Reliability and Durability](#reliability-and-durability)
4. [Flowchart](#flowchart)
5. [GANTT chart](#gantt-chart)

---

## 1. Introduction

This document provides a detailed technical specification for a 0-30 VDC Stabilised Power Supply with Current Control ranging from 2 mA to 3 A as a hardware project.

### 1.2 Purpose

The purpose of this specification is to offer a comprehensive and detailed description of the requirements for the 0-30V / 0-3A Bench Power Supply. It encapsulates its external behavior, functional requirements, design constraints, and other crucial details to ensure the successful design and execution of the hardware.

### 1.2 Scope

This hardware project centers on the 0-30V / 0-3A Bench Power Supply, a device proficient in delivering a continuously variable stabilized output adjustable between 0 and 30VDC. The power supply can modulate its output current from a minimal 2 mA to a maximum of 3A. This document encompasses the entirety of the hardware's functionalities, design considerations, and safety features.

### 1.3 Definitions, Acronyms, and Abbreviations

- **VDC**: Volts Direct Current
- **mA**: Milliamperes
- **A**: Amperes
- **PCB**: Printed Circuit Board
- **SRS**: Software Requirements Specification
- **PSU**: Power Supply Unit

---

## 2. Overall Description

### 2.1 Product Perspective

The 0-30V / 0-3A Bench Power Supply is envisioned to be a compact and efficient unit, suitable for diverse electronics testing and experimentation. The choice of a maximum 30V output ensures broader compatibility with prevalent electronics components and systems, simultaneously ensuring safety and affordability.

### 2.2 Product Functions and Components

- **Variable Voltage and Current Control**: Adjust the output voltage from 0 to 30V and the current from 2 mA to 3A.
- **LCD Displays**: Dual LCD displays for real-time voltage and current readings.
- **Control Knobs**: Precise control over voltage and current settings with digital potentiometers.
- **LED Indicator**: Signifies the operational status of the power supply.
- **Current Limiting and Short Circuit Protection**: Safeguards against overcurrent damage.
- **Power Switch**: An ON/OFF mechanism.
- **Microcontroller (ATmega32U4)**: Governs display and overall control.
- **Case**: Well-ventilated and heat-proof with anti-slippery feet.
- **Stadardised Connectors**: Banana plugs and binding posts.

### 2.3 Constraints

Approximate dimensions are 105 x 165 x 249 mm.

### 2.4 Assumptions and Dependencies

It's assumed that the device will be utilized in a dry environment.

---

## 3. Specific Requirements

### 3.1 External Interface Requirements

- **Input Interface**: TBD
- **Output Interface**: Connectors for DC output (0 – 30V, preferable up to 36V) and a USB connector (5V, 1A).
- **Display**: Dual-line for output voltage and current.

### 3.2 Performance Requirements

- **Voltage Stability**: Max ripple of 0.05%.
- **Current Limiting Accuracy**: Tolerance of ± 0.01A.

### 3.3 Safety Requirements

- **Overcurrent Protection**: Safeguard against potential overcurrent.
- **Thermal Management**: Prevent overheating.

### 3.4 Environmental Requirements

- **Operating Temperature**: Optimal between -10 to 40°C.
- **Humidity Tolerance**: Up to 80% relative humidity without condensation.

### 3.5 Reliability and Durability

- **Component Quality**: Ensure longevity and reliability.
