#ifndef DRV8243_H
#define DRV8243_H

#include <Arduino.h>
#include <SPI.h>

class DRV8243 {
public:
    // Constructor for hardware interface
    DRV8243(uint8_t enPin, uint8_t phPin, uint8_t sleepPin, uint8_t faultPin, uint8_t currentSensePin = A0);

    // Constructor for SPI interface
    DRV8243(uint8_t csPin, uint8_t enPin, uint8_t phPin, uint8_t sleepPin, uint8_t faultPin, uint8_t currentSensePin = A0);

    // Initialization functions
    void begin();
    void beginSPI();

    // Control functions
    void setPWM(uint8_t dutyCycle);
    void setDirection(bool direction);
    void enableDriver();
    void disableDriver();
    void sleepDriver();
    void wakeDriver();

    // Diagnostic functions
    float readCurrent();
    bool readFault();
    void clearFault();

    // Protection and monitoring functions
    void checkOvercurrent();
    void checkOvertemperature();
    void monitorVoltage();

    // Device verification
    bool verifyDeviceID();

private:
    uint8_t _enPin;
    uint8_t _phPin;
    uint8_t _sleepPin;
    uint8_t _faultPin;
    uint8_t _csPin;
    uint8_t _currentSensePin;
    bool _useSPI;

    // Private helper functions
    void sendSPICommand(uint16_t command);
    uint16_t readSPIData();
};

#endif
