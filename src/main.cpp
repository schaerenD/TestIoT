#include <Arduino.h>
#include <M5Stack.h>
#include "M5GFX.h"

#include "SX_mqtt.h"
#include "SX_modem.h"
#include "SX_display.h"
#include "SX_energy.h"
#include "SX_sensors.h"
#include "SX_system.h"

void setup() 
{
  // put your setup code here, to run once:
  M5.begin(); //Init M5Core. Initialize M5Core
  M5.Power.begin(); //Init Power module. Initialize the power module
  Wire.begin();

  display_init();

  //->>amperemetertest();
  //->>tickerInit();
  while (1)
  {
    //log(String(adcValue));
    //log(String("\r\n"));
    //delay(10);
  }
}

void loop() 
{
  // put your main code here, to run repeatedly:
  tickerInit();
  //->test1_calculation_for_Opertional();  // Settings Calcultaion
  //->init_modem();
  //->init_mqqt("thingsboard.cloud","1883");
  while(1)
  {
    //->test2_Screen_SM();
    //->test3_takeMeteo_SM();
    //->test5_takeCurrent_SM();
    //->test4_Post_SM("v1/devices/me/telemetry");
  }
}