# LAB DC POWER SUPPLY Development Blog

---

## 18.09.2023

- Created the initial version of Hardware Requirements Specification.
- Documented key features, requirements, and project scope.

---

## 20.09.2023

- Updated the Hardware Requirements Specification.
- Added a GANTT chart for project timeline visualization.

---

## 26.09.2023

- Researched FARNELL's catalogue and listed essential components.
- Finalized components' part numbers and estimated costs.


| Component                  | Farnell Part Number                         | Price (€)    |
|----------------------------|---------------------------------------------|--------------|
| Microcontroller (MCU)      | 1748525                                     | €4.9        |
| 3.3V LDO Voltage Regulator | 1467779                                     | €0.7        |
| 5.0V LDO Voltage Regulator | 1467781                                     | €0.5        |
| 16MHz Crystal Oscillator   | 1611806                                     | €0.8        |
| Push Buttons               | 1821244                                     | €0.5        |
| Electrolytic Capacitors    | 1217619, 4434778, 8767432                   | €4-7        |
| Ceramic capacitors         | 2320821, 1759057, 1520289                   | €1-2        |
| Diodes                     | 2118071, 1462392, 2329772, 3230195, 1634076 | €5-7        |
| Dual LCD Display (16 x 2)  | 2675047, 2125942                                   | €2-4        |
| Rotary Encoders            | 3769973                                     | €11         |
| LED Indicator, heat sink   | 2065054                                     | €5.2        |
| Resistors                  | 2112111                                     | €6          |
| **Total**                  |                                             | **€45**     |


---

## 27.09.2023

- Initialized a GitHub repository for the project.
- Added Hardware Requirements Specification, README, and this development blog.

---

## 01.10.2023
- Started with initial schematics design. 
- Searched for previously used libraries and components which are available in Delta.

---

## 04.10.2023
- Continued with schematics design.

---

## 10.10.2023
- Finished the initial schematics and ready for review. 

---

## 15.10.2023

Added II version of schematics

- divided the schematics on 3 different A4s
- modified the grid size
- modified fuse symbol and designator
- flipped J4 on Y-axis and added references for + and -
- added reset pull up cap and resistor with reset button to MCU
- added micro-USB for programming MCU
- some minor schematics and symbol fixes

Still missing or needs fixing: 

- Modify the location of V_IN_SENSE to the place where it is measured
- Some symbols are with different style
- Opamp needs to have opamp symbol on footprint
- Use DAC if we have stable output from PWM_OUT
- 2 Zerner diodes should be replaced by one component
- Every component should have URL to datasheet
- All of the power NET's should have voltage in their name and then they should have different symbol than signal nets
- Review caps, right now there are 3 different packages
- Review D8 (THT?)
- Add bypass caps to U9 and U10
- U11 and U12 bulk caps?
- Add no ERC flag to all not-used pins







