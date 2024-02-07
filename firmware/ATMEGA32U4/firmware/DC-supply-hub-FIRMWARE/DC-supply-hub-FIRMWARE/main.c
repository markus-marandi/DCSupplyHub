/*
 * main.c
 *
 * Created: 1/18/2024 3:49:54 PM
 *  Author: markus
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#define LED_PIN PB4
#define LED_DDR DDRB
#define LED_PORT PORTB
#define F_CPU 16000000UL  // Assuming a 16 MHz clock

#define MAX5395_ADDR 0x28
#define MCP4726_VSET_ADDR 0x60
#define MCP4726_ISET_ADDR 0x61
#define INA232_ADDR 0x40

volatile uint8_t uartDataReady = 0;
volatile float receivedVoltage = 0.0;

// Initialize I2C at 2MHz
void i2c_init() {
    // Set SCL to 2MHz
    TWSR = 0x00; // no prescaler
    TWBR = ((F_CPU / 400000L) - 16) / 2; // SCL frequency is 2MHz
}

// Start I2C transmission
uint8_t i2c_start(uint8_t address) {
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));

    TWDR = address;
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));

    if ((TWSR & 0xF8) != TW_START && (TWSR & 0xF8) != TW_REP_START)
    return 0;

    TWDR = address;
    TWCR = (1 << TWEN) | (1 << TWINT);

    while (!(TWCR & (1 << TWINT)));

    uint8_t twst = TW_STATUS & 0xF8;
    if ((twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK)) return 0;

    return 1;
}

// Stop I2C transmission
void i2c_stop() {
    TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
}

// Simple delay function
void delay_ms(unsigned int ms) {
    for (unsigned int i = 0; i < ms; i++) {
        for (int j = 0; j < 1000; j++) {
            __asm__ __volatile__ ("nop");
        }
    }
}

// UART receive interrupt handler
ISR(USART_RX_vect) {
    // Read the received data
    char receivedByte = UDR0;

    // Check if the received data is for setting the voltage
    if (receivedByte == 'V') {
        uartDataReady = 1;
    } else if (uartDataReady) {
        // Parse the received data as voltage value
        if (receivedByte == '.') {
            receivedVoltage += 0.05;  // Increase by 0.05 for each decimal place
        } else if (receivedByte >= '0' && receivedByte <= '9') {
            receivedVoltage = receivedVoltage * 10 + (receivedByte - '0');
        } else {
            // End of voltage value
            uartDataReady = 0;
        }
    }
}

// Initialize UART
void uart_init() {
    // Set baud rate to 9600
    UBRR0H = 0;
    UBRR0L = 103;

    // Enable receiver and receiver interrupt
    UCSR0B = (1 << RXEN0) | (1 << RXCIE0);
    
    // Set frame format: 8 data, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Send voltage value over UART
void uart_send_voltage(float voltage) {
    // Send voltage value as string
    char buffer[10];
    sprintf(buffer, "%.2fV", voltage);
    for (int i = 0; buffer[i] != '\0'; i++) {
        while (!(UCSR0A & (1 << UDRE0)));  // Wait for empty transmit buffer
        UDR0 = buffer[i];  // Put data into buffer, sends the data
    }
}

int main(void) {
    // Set LED pin as output
    LED_DDR |= (1 << LED_PIN);

    // Initialize I2C
    i2c_init();

    // Initialize UART
    uart_init();

    // Enable global interrupts
    sei();

    // Main loop
    while (1) {
        if (uartDataReady) {
            // Set output voltage using MAX5395 based on received voltage
            i2c_start(MAX5395_ADDR << 1);
            // Write command to set wiper position for voltage control
            // Replace 'command' with appropriate command to set wiper position
            // Example: TWDR = 0x10;  // Command to set wiper position
            TWCR = (1 << TWEN) | (1 << TWINT);
            while (!(TWCR & (1 << TWINT)));
            // End of I2C transmission
            i2c_stop();

            // Send the received voltage value over UART
            uart_send_voltage(receivedVoltage);
        }

        // Perform other operations as required
        
        delay_ms(500); // Example delay
    }

    return 0;
}
