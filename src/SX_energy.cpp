#include <Arduino.h>
#include <M5Stack.h>
#include "M5_ADS1115.h"
#include "SX_energy.h"
#include "SX_display.h"

ADS1115 Ammeter(AMETER, AMETER_ADDR, AMETER_EEPROM_ADDR);
float adcValue;
const int adcValueRingbufferLength = 500;
float adcValueRingbuffer[adcValueRingbufferLength];

void energy_test_output()
{
    float current = Ammeter.getValue();

    String ddd = String(adcValue);

    log(ddd);
    log("\n\r");
}
// timer init
 
// get Value

void ticker_current_callback()
{
  static uint8_t i = 0;
  adcValue = Ammeter.getValue();
  adcValueRingbuffer[i++] = adcValue;
  if(i >= adcValueRingbufferLength)
  {
    i=0;
  }
}

void test5_takeCurrent_SM()
{
  //->amperevalue();
}
/***********************************/
/**
Energy Initsystem

@param  Takes Current for Powertaker
*/
void energy_init()
{
  Ammeter.setMode(CONTINUOUS);
  Ammeter.setRate(RATE_8);
  Ammeter.setGain(PAG_512);
}

/**
Energy Average Calculation

@param  Calculate the Average Current from Ringbuffer
*/
void energy_current_average()
{
  noInterrupts();
  float newActuelAverage = 0.0;
  int numberOfValues = 0;
  for (int d = 0;d < adcValueRingbufferLength; d++)
  {
    float actuelValue = adcValueRingbuffer[d];
    adcValueRingbuffer[d] = 0.0;
    // IDEA: Calaculate only with 32 Bit int for better Performance, Xtensa 240 MhZ DSP FPU Performance???
    if(actuelValue != 0)
    {
      newActuelAverage = newActuelAverage + actuelValue;
      numberOfValues++;
    }
  }
  interrupts();  // Interrupts Turn On

  if(numberOfValues!=0)
  {
    newActuelAverage = newActuelAverage/numberOfValues;
  }
  else
  {
    newActuelAverage = 0.0;
  }
}

/**
Energy Statemachine

ACTUEL NOT USED
Takes Current for Powertaker

@param  Description of method's or function's input parameter
*/
void energy_statemachine()
{
  // Copy Ringbuffer
  energy_current_average();
}