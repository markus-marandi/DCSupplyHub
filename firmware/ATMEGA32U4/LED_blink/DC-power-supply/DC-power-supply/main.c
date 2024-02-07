/*
 * main.c
 *
 * Created: 1/18/2024 3:49:54 PM
 *  Author: markus
 */ 

#include <avr/io.h>
#include <util/twi.h>

#define LED_PIN PB4
#define LED_DDR DDRB
#define LED_PORT PORTB
#define F_CPU 16000000UL  // Assuming a 16 MHz clock

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

	// Check if the start condition was successfully transmitted
	if ((TWSR & 0xF8) != TW_START && (TWSR & 0xF8) != TW_REP_START)
	return 0;

	// Send device address
	TWDR = address;
	TWCR = (1 << TWEN) | (1 << TWINT);

	// Wait for the end of transmission
	while (!(TWCR & (1 << TWINT)));

	// Check if the device has acknowledged the READ / WRITE mode
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

int main(void) {
	// Set LED pin as output
	LED_DDR |= (1 << LED_PIN);

	i2c_init(); // Initialize I2C

	for (int cycle = 0; cycle < 3; cycle++) {
		int deviceCount = 0;
		
		// Scan for I2C devices
		for (uint8_t addr = 1; addr < 127; addr++) {
			if (i2c_start(addr << 1)) {
				deviceCount++;
				i2c_stop();
			}
		}

		// Blink LED for each device found
		for (int i = 0; i < deviceCount; i++) {
			LED_PORT ^= (1 << LED_PIN); // Toggle LED
			delay_ms(500);              // Wait
			LED_PORT ^= (1 << LED_PIN); // Toggle LED
			delay_ms(500);              // Wait
		}

		delay_ms(5000); // 5 second pause
	}

	return 0;
}