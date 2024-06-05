#include <Arduino.h>

int led = 13;
byte isEmergency = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0){
    char received = Serial.read();
    Serial.print("Received: ");
    Serial.println(received);

    if(received == '1'){
      isEmergency = 1;
    } else if(received == '0'){
      isEmergency = 0;
    }

    if(isEmergency){
      digitalWrite(led, HIGH);
    } else {
      digitalWrite(led, LOW);
    }
  }
}
