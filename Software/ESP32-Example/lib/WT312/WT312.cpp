#include "Arduino.h"
#include "WT312.h"

WT312::WT312(int PINA, int PINB, int PINLVL, int PINFB, int PINLED){
    _LED = PINLED; // I don't know a better way of naming a new unique ledc PWM channel.
    ledcSetup(_LED, 1000, 8);
    ledcAttachPin(PINLED, _LED);
    pinMode(PINA, OUTPUT);
    pinMode(PINB, OUTPUT);
    _PINA = PINA;
    _PINB = PINB;
    _PINLVL = PINLVL;
    _PINFB = PINFB;
    _LED_Brightness = 0;
}

int WT312::init(){
    if(!_characModule()){return 0;}
    //_timer = timerBegin(0, 80, true);
    //timerAttachInterrupt(_timer, &_onTimer, true);
    return 1;
}

void WT312::setIntensity(uint8_t intensity){
    _LED_Brightness = gamma8[map(intensity, 0, 100, 0, 255)];
    dacWrite(_PINLVL, map(intensity, 0, 100, _DAC_MAX, _DAC_MIN));
}

int WT312::_characModule(){
    _DAC_MAX = 0;
    _DAC_MIN = 0;

    uint16_t maxShuntVoltage = 0, minShuntVoltage = 4095;
    for(uint8_t DACcode = 0; DACcode < 255; DACcode++){
        uint16_t shuntVoltage = _testCurrent(DACcode);
        Serial.printf("DACCode: %d, Voltage: %d \n", DACcode, shuntVoltage);
        if (shuntVoltage > maxShuntVoltage)
        {
            _DAC_MIN = DACcode;
            maxShuntVoltage = shuntVoltage;
        }
        // I noticed weird voltages at start so delay until DACcode 20
        if (shuntVoltage < minShuntVoltage && DACcode > 20)
        {
            _DAC_MAX = DACcode;
            minShuntVoltage = shuntVoltage;
        }
    }
    Serial.printf("DAC_MIN: %d; DAC_MAX: %d \n", _DAC_MIN, _DAC_MAX);

    if((_DAC_MAX - _DAC_MIN) < 90) return 0;
    return 1;
}

uint16_t WT312::_testCurrent(uint8_t DACCode){
  dacWrite(_PINLVL, DACCode);
  delayMicroseconds(1000);
  digitalWrite(_PINA, HIGH);
  digitalWrite(_PINB, HIGH);
  delayMicroseconds(1000);
  uint16_t retval = 0;
  for(uint8_t i = 0; i< ADCAVG; i++){ retval += analogRead(_PINFB); }
  digitalWrite(_PINA, LOW);
  digitalWrite(_PINB, LOW);
  delayMicroseconds(1000);
  return retval/ADCAVG;
}

void WT312::_stimPulse(uint microseconds){
    digitalWrite(_PINA, HIGH);
    delayMicroseconds(microseconds);
    digitalWrite(_PINA, LOW);
    digitalWrite(_PINB, HIGH);
    delayMicroseconds(microseconds);
    digitalWrite(_PINB, LOW);
}

/* 
* This is a very crude way to drive the output. This will block all other code execution. o_O But good enough for an example!
* Use timer and interupts to allow other code to execute. Or even beter setup the LEDC or MCPWM to assynchroniously make the correct pulses for you.
*/
void WT312::pulseTrain(int width, int freq, int duration){
  ledcWrite(_LED, _LED_Brightness);
  int timeBetweenPulse = (1000/freq)-((2*width)/1000);
  long timeout = millis() + duration;
  while (millis()<timeout)
  {
    _stimPulse(width);
    delay(timeBetweenPulse);
  }
  ledcWrite(_LED, 0);
}

/*static void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&_timerMux);
  _interruptCounter++;
  portEXIT_CRITICAL_ISR(&_timerMux);
}*/