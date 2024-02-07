# LAB DC POWER SUPPLY Development Blog

## 18.09.2023

- Created the initial version of Hardware Requirements Specification.
- Documented key features, requirements, and project scope.

## 20.09.2023

- Updated the Hardware Requirements Specification.
- Added a GANTT chart for project timeline visualization.

## 26.09.2023

- Researched FARNELL's catalogue and listed essential components.
- Finalized components' part numbers and estimated costs.

| Component                  | Farnell Part Number | Price (€) |
|----------------------------|---------------------|-----------|
| Microcontroller (MCU)      | 1748525             | €4.9      |
| 3.3V LDO Voltage Regulator | 1467779             | €0.7      |
| 5.0V LDO Voltage Regulator | 1467781             | €0.5      |
| 16MHz Crystal Oscillator   | 1611806             | €0.8      |
| Push Buttons               | 1821244             | €0.5      |
| Electrolytic Capacitors    | 1217619, 4434778, 8767432 | €4-7 |
| Ceramic capacitors         | 2320821, 1759057, 1520289 | €1-2 |
| Diodes                     | 2118071, 1462392, 2329772, 3230195, 1634076 | €5-7 |
| Dual LCD Display (16 x 2)  | 2675047, 2125942    | €2-4      |
| Rotary Encoders            | 3769973             | €11       |
| LED Indicator, heat sink   | 2065054             | €5.2      |
| Resistors                  | 2112111             | €6        |
| **Total**                  |                     | **€45**   |

## 27.09.2023

- Initialized a GitHub repository for the project.
- Added Hardware Requirements Specification, README, and this development blog.

## 01.10.2023

- Started with initial schematics design. 
- Searched for previously used libraries and components which are available in Delta.

## 04.10.2023

- Continued with schematics design.

## 10.10.2023

- Finished the initial schematics and ready for review.

## 15.10.2023

- Added II version of schematics with significant improvements and fixes.

## 20.10.2023
- Started with PCB design.

## 25.10.2023
- Continued with PCB design.

## 30.10.2023
- Made modifications to schematics based on PCB design requirements.

## 05.11.2023
- Removed some unnecessary components from the schematics.
- Started with the second version of PCB design.

## 10.11.2023
- Continued with PCB design.

## 15.11.2023
- Made more modifications to the schematics based on the second version of PCB design.
- Thought about head dissipation and started with 3D modeling.

## 20.11.2023
- Continued with the PCB design.

## 01.12.2023
- Finished the PCB design and ready for review.

## 05.12.2023
- Made modifications to PCB design. 

## 10.12.2023
- Ordered components that were missing

## 27.12.2023

- Started 3D modeling, focusing on designing an enclosure that would provide adequate cooling and easy access to the power supply's interfaces.

## 29.12.2023

- Refined the design of the enclosure, adding support structures for internal components and ensuring the model was optimized for 3D printing.

## 02.01.2024

- Began the soldering process, focusing on the precision placement of microcontroller and power regulation components.

## 04.01.2024

- Continued with soldering, addressing the more complex sections involving the analog circuitry and ensuring all connections were solid for optimal performance.

## 06.01.2024

- Completed the soldering of all components. Performed inspection to identify any potential solder bridges or cold joints.

## 03.01.2024

- Initiated the 3D printing process. The first print was a prototype to test fit and identify any adjustments needed for the enclosure design. Had to reprint the enclosure due to a few minor issues.

## 05.01.2024

- Made necessary adjustments to the 3D model based on the prototype test fit. Reprinted the enclosure with the modifications to ensure fit for all internal components.

## 17.01.2024

- Began programming the microcontroller, implementing the initial firmware setup and focusing on basic power output control functionalities. Had to debug initially MCU because of cold solder joints.

## 20.01.2024

- Implemented controlling logic features in the firmware, such as overcurrent protection and voltage control. 
- Added a feature to display the current and voltage on the LCD display.
- Found out that displays are not I2C compatible and had to be solved with a digital interface.

## 24.01.2024

- Debugged the firmware, identifying bugs and areas for improvement in the control logic.
- Started testing the power supply with a dummy load to verify the output voltage and current regulation but it failed. 

## 27.01.2024

- Made more firmware adjustments to fix the issues with the power supply's output regulation.

## 31.01.2024

- Started the debugging process, going through the code and trying to identify the root cause of the power supply's output regulation issues.

## 06.02.2024
- Found out that the issue was in the current measurement and regulation part of the circuit.
- Made a document for important I2C components and their addresses.
- Made a document for the conclusion and improvements.
---
