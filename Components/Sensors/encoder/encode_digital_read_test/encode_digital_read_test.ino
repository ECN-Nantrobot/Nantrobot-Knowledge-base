#include <Arduino.h>

#define C1 23
#define C2 22

void setup(){
    Serial.begin(115200);
    
    pinMode(C1, INPUT_PULLUP);
    pinMode(C2, INPUT_PULLUP);
    
    Serial.println("Encoder Test - Reading both channels");
    Serial.println("Rotate the encoder to see signal changes");
    Serial.println("Format: C1_state | C2_state");
}

void loop(){
    int c1_state = digitalRead(C1);
    int c2_state = digitalRead(C2);
    
    Serial.print("C1: ");
    Serial.print(c1_state);
    Serial.print(" | C2: ");
    Serial.println(c2_state);
    
    delay(100);
}