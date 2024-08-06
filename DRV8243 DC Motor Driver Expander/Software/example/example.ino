#include <DRV8243.h>

DRV8243 motorDriver(10, 9, 8, 7);  // For hardware interface

void setup() {
    Serial.begin(9600);
    motorDriver.begin();
}

void loop() {
    motorDriver.setDirection(true);
    motorDriver.setPWM(128);  // 50% duty cycle
    delay(2000);
    
    motorDriver.setDirection(false);
    motorDriver.setPWM(128);  // 50% duty cycle
    delay(2000);
    
    if (motorDriver.readFault()) {
        Serial.println("Fault detected!");
        motorDriver.clearFault();
    } else {
        Serial.println("No fault.");
    }
    
    float current = motorDriver.readCurrent();
    Serial.print("Current: ");
    Serial.print(current);
    Serial.println(" A");
    
    delay(1000);
}
