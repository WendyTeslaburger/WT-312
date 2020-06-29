#include <Arduino.h>
#include "pins.h"

/*
* Written by Wendy Teslaburger.
* USE AT OWN RISK!
* 
* This is some simple example code written to show how an ESP32 can interface with the WT-312 module.
* Eventually it would be nice to have everything nicely organised in a WT312-class, but that is something for later. 
*
*/

const int LED0 = 0;
void setupPins(void);
void stimPulse(int);
void pulseTrain(int, int, int);
void setIntensity(uint8_t);
uint16_t testCurrent(uint8_t);

void setup() {
  Serial.begin(115200);
  setupPins();
  Serial.println("Testing all intensity levels:");
  for(uint8_t intensity = 0; intensity < 100; intensity++){
    Serial.print(intensity);
    Serial.print(":");
    Serial.println(testCurrent(intensity));
  }
  Serial.println("\n Starting loop\n=============");
}

void loop() {
  setIntensity(50);
  for(uint8_t count = 0; count < 10; count++){
    pulseTrain(250, 100, 250);
    delay(250);
  }
  setIntensity(75);
  for(uint8_t count = 0; count < 5; count++){
    pulseTrain(250, 100, 350);
    delay(250);
  }
  setIntensity(100);
  pulseTrain(250, 100, 1500);
}

/*
* Set the DAC to the level param and measure the shunt voltage when current is flowing through the transformer.
* This will cause the output to be energised, maybe it is a better idea to draw current from both sides of the transformer and divide by 2.
* One more thing to be investigated.
*/
uint16_t testCurrent(uint8_t level){
  setIntensity(level);
  delayMicroseconds(100);
  digitalWrite(PIN_B, HIGH);
  delayMicroseconds(1000);
  uint16_t retval = analogRead(35);
  digitalWrite(PIN_B, LOW);
  delayMicroseconds(100);
  return retval;
}

void setupPins(){
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  ledcSetup(LED0, 1000, 8);
  ledcAttachPin(PIN_LED, LED0);
}


void setIntensity(uint8_t intensity){
  ledcWrite(LED0, intensity); // The onboard LED gives us some clue about the intensity of the applied pulses.
  dacWrite(25, map(intensity, 0, 100, 120, 0));
}


/* 
* This is a very crude way to drive the output. This will block all other code execution. o_O But good enough for an example!
* Use timer and interupts to allow other code to execute. Or even beter setup the LEDC or MCPWM to assynchroniously make the correct pulses for you.
*/
void pulseTrain(int width, int freq, int duration){
  int timeBetweenPulse = (1000/freq)-((2*width)/1000);
  long timeout = millis() + duration;
  while (millis()<timeout)
  {
    stimPulse(width);
    delay(timeBetweenPulse);
  }
}

void stimPulse(int microseconds){
  digitalWrite(PIN_A, HIGH);
  delayMicroseconds(microseconds);
  digitalWrite(PIN_A, LOW);
  digitalWrite(PIN_B, HIGH);
  delayMicroseconds(microseconds);
  digitalWrite(PIN_B, LOW);
}
