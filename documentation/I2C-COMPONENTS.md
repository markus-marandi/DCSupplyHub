# I2C Communication Documentation

### Following components are connected to the I2C bus:

### 1. [MAX5395NATA+T](https://www.farnell.com/datasheets/2297485.pdf) - Digital Potentiometer (U10)
- **Purpose**: Controls I_SET for LTC3649.
- **I2C Address**: Configurable based on the ADDR0 pin setting:
  - `0x28` (ADDR0 = GND)
- **Operation**:
  - Supports both read and write operations.
  - Utilizes an 8-bit command structure for adjusting wiper positions and configuration settings.

### 2. [MCP4726](https://ww1.microchip.com/downloads/aemDocuments/documents/OTH/ProductDocuments/DataSheets/22272C.pdf) (U16, U11) - DAC for VSET and ISET Control (x2)
- **Purpose**: Provides analog output for VSET and ISET control.
- **I2C Address**: `0x60` (default). Address can be altered via the A0 pin for using multiple devices on the same bus.
  - **U11 - 0x63**
  - **U16 - 0x60**
- **Operation**:
  - Operates as a slave device.
  - Supports standard, fast, and high-speed modes.
  - Writing and reading operations adhere to the I2C protocol with specific command/data formats.

### 3. [INA232](https://www.ti.com/lit/ds/symlink/ina232.pdf?HQS=dis-mous-null-mousermode-dsf-pf-null-wwe&ts=1700934645658&ref_url=https%253A%252F%252Fwww.mouser.com%252F) (U7)- 16-bit Current Sensor
- **Purpose**: Measures current with high precision.
- **I2C Address**: `0x48` (checked).
- **I2C Operatios**:


## Non-I2C Component

### [DFROBOT DFR0645-R](https://www.farnell.com/datasheets/3708797.pdf) (J6, I3) - 4-Digital LED Segment Display Module
- **Note**: These modules are not compatible with I2C communication due to design limitations that cause them to occupy the entire I2C bus. They must be operated using a digital interface.

## Programming Considerations

- **Unique I2C Addresses**: Ensure each device on the I2C bus has a unique address to prevent conflicts.
- **Data Transfer Protocol**:
  - **Start Condition**: Signal the beginning of an I2C communication session.
  - **Slave Addressing**: Followed by the R/W bit to indicate read or write operation.
  - **Write Operation**:
    - Send the command byte followed by data bytes.
    - Each byte transmitted must be acknowledged by the receiver.
  - **Read Operation**:
    - Send the command byte to specify the register to read from.
    - Issue a repeated START condition and transmit the slave address with the read bit.
    - Receive data bytes, acknowledging after each byte, except for the last one.
  - **Stop Condition**: Marks the end of an I2C communication session.
- **Device-Specific Commands**: Consult each component's datasheet for detailed information on command structures, operational modes, and configurations.

## Note on DFROBOT DFR0645-R

The DFROBOT DFR0645-R modules require digital signals for operation and cannot share the I2C bus with other devices due to their design. Allocate separate digital pins for these modules when integrating them into systems that also use I2C communication.
