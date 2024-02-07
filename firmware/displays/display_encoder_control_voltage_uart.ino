#include "DFRobot_LedDisplayModule.h"
#include <Wire.h>

/*
ENCODER 

2 PIN side
- UPPER to DIG4
- LOWER to GND

3 PIN side
- UPPER to DIG3
- MIDDLE to GND
- LOWER to DIG2

Display
- SDA -  A4
- SCL - A5

UART connection:
Arduino Nano TX (D1) to ATmega32U4 RX pin.
Arduino Nano RX (D0) to ATmega32U4 TX pin.

*/

// Initialize with the correct I2C address
DFRobot_LedDisplayModule LED(&Wire, 0x48);

// Rotary Encoder Pins
const int encoderPin1 = 2;
const int encoderPin2 = 3;
const int encoderSwitchPin = 4;  // Push button switch of the encoder

volatile int lastEncoded = 0;
volatile long encoderValue = 0;
long lastencoderValue = 0;
int lastMSB = 0;
int lastLSB = 0;

void setup() {
  Serial.begin(9600); // have to be the same as the baud rate of the ATmega32U4
  Wire.begin();  // Start I2C communication

  // Setup encoder pins
  pinMode(encoderPin1, INPUT_PULLUP);
  pinMode(encoderPin2, INPUT_PULLUP);
  pinMode(encoderSwitchPin, INPUT_PULLUP);

  // Setup interrupts for rotary encoder
  attachInterrupt(digitalPinToInterrupt(encoderPin1), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPin2), updateEncoder, CHANGE);

  // Setup the display
  while (LED.begin(LED.e4Bit) != 0) {
    Serial.println("Failed to initialize the chip, please confirm the chip connection!");
    delay(1000);
  }
  LED.setDisplayArea(1, 2, 3, 4);
  LED.setBrightness(2);

  // Display initial value
  displayNumber(0);  // Start with 0.00 on display
}

void loop() {
  static long lastDisplayValue = -1;  // Store the last displayed value to update only on changes

  if (digitalRead(encoderSwitchPin) == LOW) {
    // Reset the value if the encoder switch is pressed
    encoderValue = 0;
  }

  // Calculate the display value based on encoder value
  float displayVal = encoderValue * 0.1;           // Example step of 0.1 per encoder step
  displayVal = max(0.00, min(displayVal, 30.00));  // Constrain between 0.00 and 30.00

  if (lastDisplayValue != encoderValue) {
    // Update the display only if the value has changed
    lastDisplayValue = encoderValue;
    displayNumber(displayVal);
    Serial.println(displayVal);
  }

  delay(50);  // Debounce delay
}

void displayNumber(float value) {
  // Format and display the value on the LED display
  int whole = (int)value;                     // Whole part
  float decimalPart = (value - whole) * 100;  // Decimal part as float to apply rounding correctly

  // Apply a rounding fix for the decimal part
  int decimal = (int)(decimalPart + 0.5);  // Adding 0.5 before casting to int rounds the decimal part

  // Ensure that after rounding, the decimal part does not exceed 99
  if (decimal >= 100) {
    whole += 1;      // Increment the whole part if rounding the decimal exceeds 99
    decimal -= 100;  // Adjust the decimal part back to 0 after incrementing the whole part
  }

  // Preparing to display numbers with correct formatting
  if (whole == 0) {
    // Special case for displaying "0."
    LED.print("", "0.", String(decimal / 10).c_str(), String(decimal % 10).c_str());
  } else {
    // For other numbers, dynamically build the string based on the whole and decimal parts
    char formattedNumber[6];  // Including space for null terminator
    sprintf(formattedNumber, "%2d.%02d", whole, decimal);

    // Break down the formatted string into displayable parts
    String part1 = (formattedNumber[0] == ' ') ? "" : String(formattedNumber[0]);
    String part2 = (formattedNumber[1] == '0' && whole < 10) ? "0." : String(formattedNumber[1]) + ".";
    String part3 = String(formattedNumber[3]);
    String part4 = String(formattedNumber[4]);

    // Send parts to the display function
    LED.print(part1.c_str(), part2.c_str(), part3.c_str(), part4.c_str());
  }

  // Debug output to serial monitor
  Serial.print("Displaying: ");
  if (whole == 0) {
    Serial.print(" 0.");
  } else {
    Serial.print(whole);
    Serial.print('.');
  }
  Serial.println(decimal);
}

void updateEncoder() {
  int MSB = digitalRead(encoderPin1);  // Most significant bit
  int LSB = digitalRead(encoderPin2);  // Least significant bit

  int encoded = (MSB << 1) | LSB;          // Converting the 2 pin value to single number
  int sum = (lastEncoded << 2) | encoded;  // Adding it to the previous encoded value

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue--;

  lastEncoded = encoded;  // Store this value for next time
}
