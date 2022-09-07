#include <Arduino.h>
#include <M5Stack.h>
#include "M5_ADS1115.h"
#include "SX_energy.h"
#include "SX_display.h"

ADS1115 Ammeter(AMETER, AMETER_ADDR, AMETER_EEPROM_ADDR);
float adcValue;
float adcValueRingbuffer[16];

void energy_test_output()
{
    float current = Ammeter.getValue();

    log("--------------");
    String ddd = String(current);

    log(ddd);
    log("\n\r");
}
// timer init
 
// get Value
void ticker_current_callback()
{
  static uint8_t i;
  adcValue = Ammeter.getValue();
  adcValueRingbuffer[i++] = adcValue;
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
Energy Statemachine

ACTUEL NOT USED
Takes Current for Powertaker

@param  Description of method's or function's input parameter
*/
void energy_statemachine()
{
    
}