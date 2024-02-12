#include <Wire.h>

void setup() {
  Wire.begin(); // Join I2C bus as master
  Serial.begin(9600); // Start serial communication at 9600 baud
}

void loop() {
  Wire.requestFrom(0x30, 32); // Request data from slave device at address 0x30, request 32 bytes
  String receivedData = "";

  while (Wire.available()) { // Slave may send less than requested
    char c = Wire.read(); // Receive a byte as character
    receivedData += c; // Add it to the string
  }

  // Extract and print the ampere and voltage values
  if (receivedData.length() > 0) {
    // Find the positions of " A" and " V"
    int ampPos = receivedData.indexOf(" A");
    int voltPos = receivedData.indexOf(" V");

    // Extract the ampere value
    String amps = "";
    if (ampPos != -1) { // Check if " A" is found
      int start = receivedData.lastIndexOf(',', ampPos) + 1; // Find the last comma before " A"
      amps = receivedData.substring(start, ampPos + 2); // Include " A" in the substring
      Serial.println("encoder_amps: " + amps); // Print ampere value
    }

    // Extract the voltage value
    String volts = "";
    if (voltPos != -1 && voltPos > ampPos) { // Check if " V" is found and it's after the ampere value
      int start = receivedData.lastIndexOf(',', voltPos) + 1; // Find the last comma before " V", after " A"
      volts = receivedData.substring(start, voltPos + 2); // Include " V" in the substring
      Serial.println("encoder_voltage: " + volts); // Print voltage value
    }
  }

  delay(1000); // Delay a second before the next request
}
