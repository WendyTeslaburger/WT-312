#ifndef WT312_h
#define WT312_h

#include <Arduino.h>

#define ADCAVG 4

const uint8_t PROGMEM gamma8[] = {
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
  
class WT312
{
    //static const portMUX_TYPE _timerMux = portMUX_INITIALIZER_UNLOCKED;
    public:
        WT312(int PINA, int PINB, int PINLVL, int PINFB, int PINLED);
        int init();
        void setIntensity(uint8_t intensity);
        void pulseTrain(int width, int freq, int duration);
    
    private:
        //hw_timer_t * _timer = NULL;
        int _characModule();
        uint16_t _testCurrent(uint8_t DACCode);
        void _stimPulse(uint microseconds);
        int _DAC_MIN;
        int _DAC_MAX;
        int _PINA;
        int _PINB;
        int _PINLVL;
        int _PINFB;
        int _LED;
        uint8_t _LED_Brightness;
        long _PulseStart;
        long _PulseEnd;
        //static void IRAM_ATTR _onTimer();
        //volatile int _interruptCounter;
        //int _totalInterruptCounter;

};

#endif