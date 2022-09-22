#include <Arduino.h>
#include <M5Stack.h>
#include "M5_ENV.h"
#include "SX_sensors.h"
#include "SX_display.h"

float tmp      = 0.0;
float hum      = 0.0;
float pressure = 0.0;

SHT3X sht30;
QMP6988 qmp6988;

void test3_takeMeteo_SM()
{
  pressure = qmp6988.calcPressure();
  if (sht30.get() == 0)   // Obtain the data of shT30.
  {
    tmp = sht30.cTemp;    // Store the temperature obtained from shT30.
    hum = sht30.humidity; // Store the humidity obtained from the SHT30.
  }
  else 
  {
    tmp = 0;
    hum = 0;
  }
}

void sensor_init()
{
  qmp6988.init();
  test3_takeMeteo_SM(); 
}
  