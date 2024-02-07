#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define F_CPU 16000000UL // Clock frequency
#define BAUD 9600 // UART baud rate

// UART Definitions
#define UBRR_VALUE (((F_CPU / (BAUD * 16UL))) - 1)

// I2C Addresses
#define MAX5395_ADDR 0x28 // Adjust based on ADDR0
#define MCP4726_ADDR 0x60 // Default address
#define INA232_ADDR 0x40  // Fixed address

#define SHUNT_RESISTOR_OHMS 1.0 // 1 Ohm
#define ADC_REF 2.048 // ADC reference voltage

// Function Prototypes
void uart_init(void);
void uart_transmit(uint8_t data);
uint8_t uart_receive(void);
void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_write(uint8_t data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);
void adc_init(void);
uint16_t adc_read(void);

// UART Functions
void uart_init(void) {
    UBRR1H = (uint8_t)(UBRR_VALUE>>8);
    UBRR1L = (uint8_t)UBRR_VALUE;
    UCSR1B = (1<<RXEN1)|(1<<TXEN1); // Enable receiver and transmitter
    UCSR1C = (1<<UCSZ10)|(1<<UCSZ11); // Set frame format: 8data, 1stop bit
}

void uart_transmit(uint8_t data) {
    while (!(UCSR1A & (1<<UDRE1))); // Wait for empty transmit buffer
    UDR1 = data; // Put data into buffer, sends the data
}

uint8_t uart_receive(void) {
    while (!(UCSR1A & (1<<RXC1))); // Wait for data to be received
    return UDR1; // Get and return received data from buffer
}

// I2C Functions
void i2c_init(void) {
    TWSR = 0x00; // Set prescaler to 1
    TWBR = 0x0C; // SCL frequency is F_CPU / (16 + 2(TWBR) * prescalerValue) = 400kHz
}

void i2c_start(void) {
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

void i2c_stop(void) {
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void i2c_write(uint8_t data) {
    TWDR = data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

uint8_t i2c_read_ack(void) {
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

uint8_t i2c_read_nack(void) {
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

// ADC Initialization
void adc_init(void) {
    ADMUX = (1 << REFS0); // AVcc with external capacitor at AREF pin
    ADCSRA = (1 << ADEN) | (7 << ADPS0); // Enable ADC and set prescaler to 128
}

// ADC Read
uint16_t adc_read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF8) | channel; // Select ADC channel with safety mask
    ADCSRA |= (1 << ADSC); // Start single conversion
    while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
    return ADC;
}

int main(void) {
    uart_init();
    i2c_init();
    adc_init();

int main(void) {
    uart_init();
    i2c_init();
    adc_init();
    sei(); // Enable global interrupts

    char buffer[256]; // Buffer for UART data
    uint8_t index = 0; // Index for buffer position
    char receivedChar;
    float voltage = 0, current = 0; // Variables for received settings

    while (1) {
        // Check if data is received via UART
        if (UCSR1A & (1<<RXC1)) {
            receivedChar = uart_receive(); // Read the received data
            if (receivedChar == '\n' || receivedChar == '\r') { // Check for end of line
                buffer[index] = '\0'; // Null-terminate the string
                index = 0; // Reset index for next data

                // Parsing the received string for voltage or current settings
                if (strncmp(buffer, "VOLTAGE", 7) == 0) {
                    voltage = atof(buffer + 8); // Convert string to float
                    // Convert voltage to DAC value and set on MAX5395
                    uint8_t voltage_dac_value = (uint8_t)((voltage / 30.0) * 255); // Example conversion
                    i2c_start();
                    i2c_write(MAX5395_ADDR << 1);
                    i2c_write(voltage_dac_value); // Assuming direct value write
                    i2c_stop();
                } else if (strncmp(buffer, "AMPS", 4) == 0) {
                    current = atof(buffer + 5); // Convert string to float
                    // Convert current to DAC value and set on MCP4726
                    uint8_t current_dac_value = (uint8_t)((current / 3.0) * 255); // Example conversion
                    i2c_start();
                    i2c_write(MCP4726_ADDR << 1);
                    i2c_write(current_dac_value); // Assuming direct value write
                    i2c_stop();
                }
            } else {
                // If not end of line, add character to buffer and increment index
                buffer[index++] = receivedChar;
            }
        }

        // ADC reading for current limit detection
        uint16_t adc_result = adc_read(7); // Reading ADC from channel 7 (PF7)
        float measured_current = (adc_result / 1023.0) * ADC_REF; // Convert ADC to current
          if (measured_current > CURRENT_LIMIT) {
                // Reduce current by adjusting DAC value on MCP4726
                uint8_t reduced_dac_value = current_dac_value - CURRENT_ADJUSTMENT; // Example reduction
                i2c_start();
                i2c_write(MCP4726_ADDR << 1);
                i2c_write(reduced_dac_value); // Apply reduced value
                i2c_stop();
            }

        float actual_current = 0.0; // Actual current measured by INA232
        float desired_current = current; // Desired current set via UART
        float adc_current_limit = 0.0; // Current limit detected by ADC

        // Read actual current from INA232
        i2c_start();
        i2c_write(INA232_ADDR << 1); // Write device address with write bit
        i2c_write(INA232_CURRENT_REG); // Specify the current register address
        i2c_restart(); // Send a repeated start condition
        i2c_write((INA232_ADDR << 1) | 1); // Switch to read mode

        // Assuming the INA232 sends back 2 bytes of data for the current measurement
        uint8_t msb = i2c_read_ack(); // Read MSB and send ACK for more data
        uint8_t lsb = i2c_read_nack(); // Read LSB and send NACK to end reading
        i2c_stop();

        // Combine MSB and LSB into a single 16-bit value
        uint16_t raw_current = ((uint16_t)msb << 8) | lsb;

        float convert_raw_to_actual_current(uint16_t raw_value) {
        float shunt_voltage_microvolts = raw_value; // Direct conversion, replace with actual formula
        float current_amps = shunt_voltage_microvolts / (1000000 * SHUNT_RESISTOR_OHMS); // Convert μV to V, then apply Ohm's law
        return current_amps;
}

        // Compare actual current with desired current and adjust if necessary
        if (fabs(actual_current - desired_current) > SOME_THRESHOLD) {
            // Adjust current setting using MCP4726
            // This involves writing a new value to MCP4726 based on the difference
            // Example placeholder code, assumes direct linear correlation
            uint8_t new_dac_value = (uint8_t)((desired_current / 3.0) * 255);
            i2c_start();
            i2c_write(MCP4726_ADDR << 1);
            i2c_write(new_dac_value); // Update with new DAC value
            i2c_stop();
        }

        // ADC reading for current limit detection on PF7
        uint16_t adc_result = adc_read(7); // Assuming adc_read() is already defined to read from a specific channel
        // Convert ADC result to actual current, using ADC_REF and assuming a linear correlation
        adc_current_limit = ((float)adc_result / ADC_MAX) * ADC_REF;

        // Check if the current exceeds a predefined limit
        if (adc_current_limit > CURRENT_LIMIT) {
            // Adjust MCP4726 downward to reduce current
            // Placeholder code to reduce current, actual implementation depends on the system's response characteristics
            uint8_t reduced_dac_value = new_dac_value - SOME_ADJUSTMENT; // Reduce DAC value to lower current
            i2c_start();
            i2c_write(MCP4726_ADDR << 1);
            i2c_write(reduced_dac_value); // Apply reduced value
            i2c_stop();
        }

_delay_ms(100); // Delay for loop stability and timing


        _delay_ms(100); // Add delay for loop timing and stability
    }
}
