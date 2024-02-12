#include <Wire.h>

// Rotary Encoder Pins for Current
const int encoderPin1_Current = 2;
const int encoderPin2_Current = 3;
const int encoderSwitchPin_Current = 4;

volatile long encoderValue_Current = 0;
float lastVoltageValue = -1.0;  // For storing the last voltage value read from serial
bool voltageLineNext = false;   // Flag to indicate the next line contains voltage value

String sendData = "";     // Data string to send over I2C
String sendCurrent = "";  // Data string to send over I2C
String sendVoltage = "";  // Data string to send over I2C


void setup() {
  Serial.begin(9600);
  Wire.begin(0x30);  // Initialize as I2C slave with address 0x30

  pinMode(encoderPin1_Current, INPUT_PULLUP);
  pinMode(encoderPin2_Current, INPUT_PULLUP);
  pinMode(encoderSwitchPin_Current, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encoderPin1_Current), updateEncoderCurrent, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPin2_Current), updateEncoderCurrent, CHANGE);

  Wire.onRequest(requestEvent);  // Register request event
}

void loop() {
  if (digitalRead(encoderSwitchPin_Current) == LOW) {
    encoderValue_Current = 0;
  }

  long tempEncoderValue_Current = encoderValue_Current;  // Copy to minimize time interrupts are off
  float displayVal_Current = tempEncoderValue_Current * 0.05;
  displayVal_Current = max(0.00, min(displayVal_Current, 3.00));
  //Serial.println(displayVal_Current);

  // Process incoming voltage readings from Serial
  if (Serial.available() > 0) {
    if (voltageLineNext) {
      // If the next line is expected to contain a voltage value
      String voltageValueStr = Serial.readStringUntil('\n');
      float voltageValue = voltageValueStr.toFloat();
      if (voltageValue != lastVoltageValue) {
        lastVoltageValue = voltageValue;
        //Serial.print("Voltage: ");
        //Serial.println(voltageValue, 2);
      }
      voltageLineNext = false;  // Reset flag after reading voltage
    } else {
      String line = Serial.readStringUntil('\n');
      if (line.startsWith("V")) {
        voltageLineNext = true;  // Next line will contain the voltage value
      }
    }
  }

  delay(50);  // Small delay to reduce load
}

void updateEncoderCurrent() {
  static int lastEncoded = 0;
  int MSB = digitalRead(encoderPin1_Current);  // Most significant bit
  int LSB = digitalRead(encoderPin2_Current);  // Least significant bit
  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded << 2) | encoded;

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue_Current++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue_Current--;

  lastEncoded = encoded;
}

void requestEvent() {
  // Prepare the data string just before sending
  sendVoltage = String(lastVoltageValue, 2) + " V ";
  sendCurrent = String(encoderValue_Current * 0.05, 2) + " A ";
  sendData = sendCurrent + ", " + sendVoltage;
  // Send the combined string
  Wire.write(sendData.c_str(), sendData.length());
  // Optional: Debug print to Serial
  Serial.println(sendData);
  Wire.write(sendVoltage.c_str(), sendData.length());
  Wire.write(sendCurrent.c_str(), sendData.length());
}
