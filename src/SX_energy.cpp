#include <Arduino.h>
#include <M5Stack.h>
#include "M5_ADS1115.h"
#include "SX_energy.h"
#include "SX_display.h"

ADS1115 Ammeter(AMETER, AMETER_ADDR, AMETER_EEPROM_ADDR);
float adcValue;
float adcValueRingbuffer[16];

// init Energy
void energy_amperemeter_setup()
{
    Ammeter.setMode(CONTINUOUS);
    Ammeter.setRate(RATE_8);
    Ammeter.setGain(PAG_512);
}

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

void energy_statemachine()
{
    
}