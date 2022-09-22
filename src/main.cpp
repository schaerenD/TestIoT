#include <Arduino.h>
#include <M5Stack.h>
#include "M5GFX.h"

#include "SX_mqtt.h"
#include "SX_modem.h"
#include "SX_display.h"
#include "SX_energy.h"
#include "SX_sdcard.h"
#include "SX_sensors.h"
#include "SX_system.h"

void setup() 
{
  // put your setup code here, to run once:
  M5.begin(); //Init M5Core. Initialize M5Core
  M5.Power.begin(); //Init Power module. Initialize the power module
  Wire.begin(); // Wire/I2C is used for Current and Temperature, Pressure and Humandity.

  // Swisscom Init Functions
  ticker_init(); // Timer Ticker Interrupts
  display_init(); // Init Display Functinality
  sdcard_init();
  energy_init(); // Energy Init
  sensor_init();
  modem_init();
  mqtt_init();


  // Read Parameter

  // Others
  
}

void loop() 
{
  energy_statemachine();
  test3_takeMeteo_SM();
  //mqtt_send();

  delay(1000);
}