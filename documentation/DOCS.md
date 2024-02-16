### DC Supply Hub Documentation

#### Overview

The DC Supply Hub is designed to provide a configurable power supply, capable of adjusting voltage and current settings through user inputs. This system utilizes a microcontroller unit (MCU) interfaced with various components including digital potentiometers, digital-to-analog converters (DACs), and current sensors, to control and monitor the output power.

#### System Architecture

![System Flowchart](documentation/images/system-flowchart.png)

**Components**
- **Microcontroller (MCU):** At the heart of the system is an ATMEGA32U4-AU MCU, responsible for coordinating the interactions between all peripherals and executing the firmware logic.
- **Digital Potentiometer (MAX5395NATA+T):** Used for fine-tuning the output voltage by adjusting the resistance in the feedback loop of an operational amplifier.
- **DACs (MCP4726):** Two DACs are used, one for setting the output voltage (VSET) and the other for setting the output current limit (ISET).
- **Current Sensor (INA232BIDDFR):** Provides accurate monitoring of the output current by measuring the voltage across a shunt resistor.
- **Encoders:** Two rotary encoders provide user inputs to set the desired voltage and current settings.

**I2C Addresses**
- 0x28: Digital potentiometer (MAX5395NATA+T) [U10].
- 0x30: Encoder values for voltage and current.
- 0x48: INA232BIDDFR (16-bit Current sensor).
- 0x60: MCP4726 (DAC for VSET) [U16].
- 0x63: MCP4726 (DAC for ISET) [U11].

**MCU logical I/O**
- PIN 1 (CLIM): Current limit indicator/output.
- PIN 28 (LED_MCU): LED indicator controlled by the MCU.
- PIN 36 (ADC-IOUT): Analog-to-digital converter input for measuring output current directly from the MCU.

#### Firmware Logic

![Firmware Flowchart](documentation/images/firmware-flowchart.pdf)

The firmware controls the DC Supply Hub's operation, including reading encoder inputs for voltage and current settings, adjusting the output voltage and current accordingly, and monitoring the actual output current for safety and accuracy. The main components of the firmware logic include:

1. **Initialization:** Setup of I2C communication, DACs initialization, encoder input setup, and current sensor configuration.
2. **Input Processing:** Receives encoder inputs for desired voltage and current settings and updates the system settings accordingly.
3. **Output Adjustment:** Controls the digital potentiometer and DACs to adjust the output voltage and current settings based on user inputs.
4. **Current Monitoring:** Uses the INA232 current sensor to monitor the actual output current, ensuring it does not exceed the user-set limit.
5. **Safety Features:** Includes mechanisms to prevent overcurrent and overheating, such as dynamic current limiting and thermal shutdown.

#### Bill of Materials (BOM)

```markdown
| Component | Part Number | Description | Quantity |
|-----------|-------------|-------------|----------|
| D3 | SSB43L | 40V 0.45V 5A Schottky Barrier Diodes | 7 |
| ENC1, ENC2 | 440054-3 | 1x3P 2mm Male pin Wire To Board Connector | 10 |
| J2 | 2013499-1 | MICRO USB REC TYPE B DIP | 5 |
| J4 | 1757242 | 2 Bend 5.08mm 1x2P Pluggable Terminal Block | 5 |
| LED1 | CheckFuse | 0805 Red LED | 20 |
| LED2 | YELLOW | 0805 Yellow LED | - |
| LED3 | BLUE | 0805 Blue LED | - |
| LED7 | GREEN | 0805 Green LED | - |
| U5 | ATMEGA32U4-AU | 32KB Flash Microcontroller | 5 |
| D1, D2, D6 | CM1223-02SO | SOT-23-5 TVS | 15 |
| J3, J5, J6 | 61900411121 | 2x32P 2.54mm Plugin Connector | 15 |

```
### Programming the MCU

The MCU was programmed using Arduino's In-Circuit Serial Programming (ICSP) method, ensuring a reliable and efficient way to upload the firmware directly to the chip. This approach allows for direct programming of the MCU without the need for a pre-installed bootloader, providing more control over the hardware and enabling the use of the entire flash memory for the application code.

### Conclusion

The DC Supply Hub represents a sophisticated electronic design that integrates digital control with analog power management to provide a versatile and user-friendly power supply solution. The documentation provided outlines the system's architecture, key components, and firmware logic, providing a comprehensive overview for understanding and replicating the design.