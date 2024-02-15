/*
I2C addresses
- 0x28 - MAX5395NATA+T (Digital potentiometer) [U10]
- 0x30 - incoming encoder values for voltage and current
- 0x48 - INA232BIDDFR (16-bit Current sensor)
- 0x60 - MCP4726 (DAC for VSET) [U16]
- 0x63 - MCP4726 (DAC for ISET) [U11]

MCU logical I/O
- PIN 1 - CLIM
- PIN 28 - LED_MCU
- PIN 36 - ADC-IOUT

*/

// https://app.code2flow.com/Dm8F9oMX7uvp


#include <Wire.h>
#include <Adafruit_MCP4725.h>

// Define I2C addresses
#define ADDR_DISPLAY_SET 0x30
#define ADDR_DAC_VSET 0x60
#define ADDR_DAC_ISET 0x63
#define ADDR_MAX5395 0x28
#define INA232_ADDRESS 0x48
#define SHUNT_VOLTAGE_REG 0x01
#define SHUNT_CURRENT_REG 0x04

// Define MCU logical I/O pins
#define PIN_CLIM 1
#define PIN_LED_MCU 28
#define PIN_ADC_IOUT 36

// INA232 Registers
#define INA232_CONFIG 0x00
#define INA232_SHUNT_VOLTAGE 0x01

// Global variable for current resolution (1mV/A in this case)
float currentResolution = 0.001;  // You can change this value as needed
float Kp = 1;                     //tunable parameter that determines how aggressively the system responds to the difference between the desired and actual currents


// Global variable to hold the current DAC setting for ISET
int isetDacValue = 0;  // Initialize to 0 or appropriate value based on your system requirements


// Initialize DAC objects
Adafruit_MCP4725 dacVset;
Adafruit_MCP4725 dacIset;


// Define constants for voltage and current calculation
const float biasCurrent = 50e-6;       // 50µA bias current
const float maxResistance = 100000.0;  // 100kΩ, max resistance of the digital pot
const int maxWiperPosition = 255;      // MAX5395 has 256 positions, 0-255
const float MAX_CURRENT = 3.00;

// Constants for the DACs
const float VREF = 2.048;                              // Reference voltage for MCP4726 is 2.048V
const int DAC_RESOLUTION = 4096;                       // MCP4726 is a 12-bit DAC, so it has 4096 levels
const float VOLTAGE_PER_STEP = VREF / DAC_RESOLUTION;  // Voltage per step for the DAC

volatile float encoder_voltage = 0.0;
volatile float encoder_current = 0.0;

// Get values from encoders
void receiveEvent(int bytes) {
  String receivedData = "";
  while (Wire.available()) {
    char c = Wire.read();  // receive byte as a character
    receivedData += c;
  }

  // Parse the incoming string format "0.5 A, 2.5 V"
  int separatorIndex = receivedData.indexOf('A');
  if (separatorIndex != -1) {
    // Create substrings for current and voltage
    String currentStr = receivedData.substring(0, separatorIndex);
    String voltageStr = receivedData.substring(separatorIndex + 2, receivedData.indexOf('V'));

    // Trim whitespace from the substrings
    currentStr.trim();
    voltageStr.trim();

    // Convert the trimmed strings to float values
    encoder_current = currentStr.toFloat();
    encoder_voltage = voltageStr.toFloat();
  }
}


void setup() {
  Serial.begin(9600);  // Start serial communication at 9600 baud
  while (!Serial)
    ;  // Wait for the serial port to connect - necessary for Leonardo/Micro
  Serial.println("Serial communication started.");

  Wire.begin();  // Join I2C bus
  Serial.println("I2C bus initialized.");
  dacVset.begin(ADDR_DAC_VSET);  // Initialize the VSET DAC
  dacIset.begin(ADDR_DAC_ISET);  // Initialize the ISET DAC
  Wire.begin(ADDR_DISPLAY_SET);  // Join I2C bus with the address for encoder values
  Wire.onReceive(receiveEvent);  // Register event for receiving I2C data
  pinMode(PIN_LED_MCU, OUTPUT);  // Initialize the MCU LED pin as an output

  uint16_t initialIsetDacValue = (uint16_t)((encoder_current / VOLTAGE_PER_STEP) + 0.5);  // Convert an initial current to DAC value
  isetDacValue = initialIsetDacValue;                                                     // Store this initial value in the global variable
  dacIset.setVoltage(isetDacValue, false);                                                // Apply the initial setting

  // Configure INA232
  Serial.println("Configuring INA232...");
  // Set the configuration register to the desired settings
  // Example: continuous shunt and bus voltage measurement with 1 average
  if (writeRegister(INA232_CONFIG, 0x4127)) {
    Serial.println("INA232 configured successfully.");
  } else {
    Serial.println("Failed to configure INA232.");
  }
}

// Function to calculate and set the DAC values for voltage and current
void setVoltageAndCurrent(float voltage, float current) {
  // Calculate DAC value for voltage (VSET)
  // Considering a gain of 15x in the inverting amplifier
  uint16_t vsetDacValue = (uint16_t)((voltage / (VOLTAGE_PER_STEP * 15)) + 0.5);  // Adding 0.5 for rounding to nearest integer

  // Calculate DAC value for current (ISET)
  // 1V corresponds to the full-scale current limit
  uint16_t isetDacValue = (uint16_t)((current / VOLTAGE_PER_STEP) + 0.5);  // Adding 0.5 for rounding to nearest integer

  // Write the DAC values
  dacVset.setVoltage(vsetDacValue, false);
  dacIset.setVoltage(isetDacValue, false);
}

// Function to write a value to the MAX5395
void setPotentiometerWiper(uint8_t wiperValue) {
  Wire.beginTransmission(ADDR_MAX5395);
  Wire.write((uint8_t)0x00);  // Command byte to select the wiper register
  Wire.write(wiperValue);     // Wiper value to set the resistance
  Wire.endTransmission();
}

// Function to convert voltage to resistance and then to wiper position
uint8_t voltageToWiperPosition(float voltage) {
  // Calculate the desired resistance from the voltage
  float desiredResistance = voltage / biasCurrent;

  // If the calculated resistance exceeds the potentiometer's capability, limit it
  if (desiredResistance > maxResistance) {
    desiredResistance = maxResistance;
  }

  // Convert the resistance to a wiper position
  uint8_t wiperPosition = (uint8_t)((desiredResistance / maxResistance) * maxWiperPosition);

  return wiperPosition;
}


void loop() {
  bool readSuccess;
  // Correctly call readRegister with both required arguments
  int16_t shuntVoltage = readRegister(INA232_SHUNT_VOLTAGE, readSuccess);

  if (!readSuccess) {
    Serial.println("Failed to read shunt voltage.");  // Handle the failure case
  } else {
    // If read was successful, proceed with using the shuntVoltage
    float current = shuntVoltage * currentResolution;
    Serial.print("Current: ");
    Serial.print(current);
    Serial.println(" A");
  }

  // Set the desired voltage and current
  setVoltageAndCurrent(encoder_voltage, encoder_current);

  // Convert voltage to wiper position
  uint8_t wiperPosition = voltageToWiperPosition(encoder_voltage);

  // Set the potentiometer wiper to achieve the desired voltage
  setPotentiometerWiper(wiperPosition);

  adjustCurrent();  // Adjust the current based on encoder input and actual measurement

  // Toggle LED to indicate operation
  digitalWrite(PIN_LED_MCU, HIGH);
  delay(500);
  digitalWrite(PIN_LED_MCU, LOW);

  delay(1000);  // Wait for 1 second before the next iteration
}



bool writeRegister(uint8_t reg, uint16_t value) {
  Wire.beginTransmission(INA232_ADDRESS);
  Wire.write(reg);
  Wire.write((value >> 8) & 0xFF);       // MSB
  Wire.write(value & 0xFF);              // LSB
  byte result = Wire.endTransmission();  // Execute the transmission and capture the result
  return result == 0;                    // Return true if transmission was successful, false otherwise
}


int16_t readRegister(uint8_t reg, bool& success) {
  Wire.beginTransmission(INA232_ADDRESS);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) {
    success = false;  // Indicate failure
    return 0;         // Early exit on transmission failure
  }
  Wire.requestFrom(static_cast<uint8_t>(INA232_ADDRESS), static_cast<uint8_t>(2));
  if (Wire.available() < 2) {
    success = false;  // Indicate failure if not enough data is available
    return 0;         // Early exit on data availability issue
  }
  success = true;                     // Indicate success
  uint16_t value = Wire.read() << 8;  // MSB
  value |= Wire.read();               // LSB
  return (int16_t)value;
}

void adjustCurrent() {
  bool readSuccess;
  // Correctly call readRegister with the updated signature
  int16_t shuntVoltage = readRegister(INA232_SHUNT_VOLTAGE, readSuccess);

  if (!readSuccess) {
    Serial.println("Error reading shunt voltage.");
    return;  // Early exit if read operation failed
  }

  // Proceed with using shuntVoltage for calculations
  float actual_current = shuntVoltage * currentResolution;
  // Calculate the difference between desired and actual current
  float current_difference = encoder_current - actual_current;

  // Proportional adjustment based on the difference
  int adjustment = (int)(Kp * current_difference * DAC_RESOLUTION / VREF);

  // Apply limits to isetDacValue to ensure it's within DAC range and safety limits
  isetDacValue += adjustment;
  isetDacValue = max(0, min(isetDacValue, DAC_RESOLUTION - 1));
  float proposedCurrent = isetDacValue * VOLTAGE_PER_STEP;
  if (proposedCurrent > MAX_CURRENT) {
    isetDacValue = (int)(MAX_CURRENT * DAC_RESOLUTION / VREF);
  }

  // Apply the adjusted DAC value
  dacIset.setVoltage(isetDacValue, false);
  Serial.print("Adjusted DAC Value for ISET: ");
  Serial.println(isetDacValue);
}
