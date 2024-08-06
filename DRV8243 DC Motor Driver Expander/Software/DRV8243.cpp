#include "DRV8243.h"

// Constructor for hardware interface
DRV8243::DRV8243(uint8_t enPin, uint8_t phPin, uint8_t sleepPin, uint8_t faultPin, uint8_t currentSensePin)
    : _enPin(enPin), _phPin(phPin), _sleepPin(sleepPin), _faultPin(faultPin), _currentSensePin(currentSensePin), _useSPI(false) {
}

// Constructor for SPI interface
DRV8243::DRV8243(uint8_t csPin, uint8_t enPin, uint8_t phPin, uint8_t sleepPin, uint8_t faultPin, uint8_t currentSensePin)
    : _csPin(csPin), _enPin(enPin), _phPin(phPin), _sleepPin(sleepPin), _faultPin(faultPin), _currentSensePin(currentSensePin), _useSPI(true) {
}

// Initialization for hardware interface
void DRV8243::begin() {
    pinMode(_enPin, OUTPUT);
    pinMode(_phPin, OUTPUT);
    pinMode(_sleepPin, OUTPUT);
    pinMode(_faultPin, INPUT);
    digitalWrite(_sleepPin, HIGH);  // Wake up the driver
}

// Initialization for SPI interface
void DRV8243::beginSPI() {
    begin();  // Call common initialization
    pinMode(_csPin, OUTPUT);
    SPI.begin();
    if (!verifyDeviceID()) {
        Serial.println("Device ID verification failed!");
    } else {
        Serial.println("Device ID verification successful!");
    }
}

// Set PWM duty cycle (0-255)
void DRV8243::setPWM(uint8_t dutyCycle) {
    analogWrite(_enPin, dutyCycle);
}

// Set direction
void DRV8243::setDirection(bool direction) {
    digitalWrite(_phPin, direction ? HIGH : LOW);
}

// Enable driver
void DRV8243::enableDriver() {
    digitalWrite(_enPin, HIGH);
}

// Disable driver
void DRV8243::disableDriver() {
    digitalWrite(_enPin, LOW);
}

// Put driver to sleep
void DRV8243::sleepDriver() {
    digitalWrite(_sleepPin, LOW);
}

// Wake driver from sleep
void DRV8243::wakeDriver() {
    digitalWrite(_sleepPin, HIGH);
}

// Read current (assuming analog input on a specific pin)
float DRV8243::readCurrent() {
    int analogValue = analogRead(_currentSensePin);  // Read from specified pin
    float voltage = analogValue * (5.0 / 1023.0);  // Convert to voltage
    float current = voltage / 0.1;  // Assuming a 0.1 Ohm shunt resistor
    return current;
}

// Read fault status
bool DRV8243::readFault() {
    return digitalRead(_faultPin) == LOW;
}

// Clear fault (for SPI variant)
void DRV8243::clearFault() {
    if (_useSPI) {
        sendSPICommand(0x0200);  // Example command to clear fault
    }
}

// Check overcurrent (for SPI variant)
void DRV8243::checkOvercurrent() {
    if (_useSPI) {
        uint16_t status = readSPIData();
        if (status & 0x0100) {  // Example status bit for overcurrent
            Serial.println("Overcurrent detected!");
        }
    }
}

// Check overtemperature (for SPI variant)
void DRV8243::checkOvertemperature() {
    if (_useSPI) {
        uint16_t status = readSPIData();
        if (status & 0x0200) {  // Example status bit for overtemperature
            Serial.println("Overtemperature detected!");
        }
    }
}

// Monitor voltage (for SPI variant)
void DRV8243::monitorVoltage() {
    if (_useSPI) {
        uint16_t status = readSPIData();
        if (status & 0x0400) {  // Example status bit for voltage monitoring
            Serial.println("Voltage issue detected!");
        }
    }
}

// Verify the device ID for SPI variant
bool DRV8243::verifyDeviceID() {
    if (_useSPI) {
        uint16_t deviceID = readSPIData();
        return (deviceID == 0x32 || deviceID == 0x42 || deviceID == 0x52 || deviceID == 0x36 || deviceID == 0x46 || deviceID == 0x56);
    }
    return false;
}

// Send SPI command
void DRV8243::sendSPICommand(uint16_t command) {
    digitalWrite(_csPin, LOW);
    SPI.transfer16(command);
    digitalWrite(_csPin, HIGH);
}

// Read SPI data
uint16_t DRV8243::readSPIData() {
    digitalWrite(_csPin, LOW);
    uint16_t data = SPI.transfer16(0x0000);  // Send dummy data to read
    digitalWrite(_csPin, HIGH);
    return data;
}
