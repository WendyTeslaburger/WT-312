#include <Arduino.h>
#include "pins.h"
#include <WT312.h>

/*
* Written by Wendy Teslaburger.
* USE AT OWN RISK!
* 
* This is some simple example code written to show how an ESP32 can interface with the WT-312 module.
* Eventually it would be nice to have everything nicely organised in a WT312-class, but that is something for later. 
*
*/

const int LED0 = 0;
uint8_t CH1_LED_VALUE = 0;

WT312 module1 = WT312(PIN_CH1_A, PIN_CH1_B, PIN_CH1_LVL, PIN_CH1_FB, PIN_CH1_LED);

void errorloop();

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200);
  if(!module1.init()) errorloop();


  Serial.println("\n Starting loop\n=============");
}

void loop() {
  for(int intensity = 5; intensity <45; intensity+=5){
    module1.setIntensity(intensity);
    for(int pulseWidth = 100; pulseWidth < 300; pulseWidth+=25){
      module1.pulseTrain(pulseWidth, 100, 100);
      //delay(500);
    }
    for(int pulseWidth = 300; pulseWidth > 100; pulseWidth-=25){
      module1.pulseTrain(pulseWidth, 100, 100);
      //delay(500);
    }
    
  }
}

void errorloop(){
  while (true)  //ERROR!
    {
      digitalWrite(PIN_LED, HIGH);
      delay(100);
      digitalWrite(PIN_LED, LOW);
      delay(100);
    }
}