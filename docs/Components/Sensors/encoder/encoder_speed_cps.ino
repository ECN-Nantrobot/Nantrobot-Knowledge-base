#include <Arduino.h>

#define C1 23
#define C2 22

volatile long encoderCount = 0;

void setup(){
    Serial.begin(115200);
    
    pinMode(C1, INPUT_PULLUP);
    pinMode(C2, INPUT_PULLUP);
    
    // Attach interrupt to channel 1 (C1)
    attachInterrupt(digitalPinToInterrupt(C1), encoderISR, CHANGE);
    
    Serial.println("Encoder Speed Test Started");
    Serial.println("Rotate the encoder to see counts and speed");
}

void loop(){
    static long lastPrintedCount = 0;
    static unsigned long lastTime = 0;
    
    unsigned long currentTime = millis();
    long currentCount = encoderCount;  // Read volatile variable once
    
    // Calculate counts per second
    float deltaTime = (currentTime - lastTime) / 1000.0;  // Convert to seconds
    long deltaCount = abs(currentCount - lastPrintedCount);
    float countsPerSecond = deltaCount / deltaTime;
    
    Serial.print("Count: ");
    Serial.print(currentCount);
    Serial.print(" | Change: ");
    Serial.print(currentCount - lastPrintedCount);
    Serial.print(" | CPS: ");
    Serial.println(countsPerSecond, 1);
    
    lastPrintedCount = currentCount;
    lastTime = currentTime;
    
    delay(200);  // Print every 200ms
}

// Interrupt Service Routine (ISR) - Keep it fast!
void encoderISR() {
    int stateC1 = digitalRead(C1);
    int stateC2 = digitalRead(C2);
    
    // Since we only trigger on C1 changes, we know it changed
    if (stateC1 == stateC2) {
        encoderCount--;  // Counter-clockwise
    } else {
        encoderCount++;  // Clockwise
    }
}