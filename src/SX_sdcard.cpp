#include <Arduino.h>
#include <M5Stack.h>
#include "SX_sensors.h"
#include "SX_display.h"

void sdcard_init()
{   
    String Test;
    display_debug_output("FUNCCALL");
    if (!SD.begin()) 
    {
        display_debug_output("ERROR");
        while (1);
    }

    File f = SD.open("/CONFIGURATION.txt", FILE_READ);
    if (f) {
        Test = f.readString();
        display_debug_output(Test);
        f.close();
        display_debug_output("DONE");
    }
    else
    {
        display_debug_output("NOPE");
    }

    // Init SD Card
    int firstParameterBegin = Test.indexOf('=');
    int firstParameterEnd = Test.indexOf('\r');
    int secondParameterBegin = Test.indexOf('=',firstParameterEnd);
    int secondParameterEnd = Test.indexOf('\r',secondParameterBegin);
    int thirdParameterBegin = Test.indexOf('=',secondParameterEnd);
    int thirdParameterEnd = Test.indexOf('\r',thirdParameterBegin);

    //int secondParameter =  Test.indexOf(',');
    //int thirdParameter =  Test.indexOf(',');

    String firstParameter = Test.substring(firstParameterBegin+1, firstParameterEnd);
    String secondParameter = Test.substring(secondParameterBegin+1, secondParameterEnd);
    String thirdParameter = Test.substring(thirdParameterBegin+1, thirdParameterEnd);

    display_debug_output(firstParameter);
    display_debug_output(secondParameter);
    display_debug_output(thirdParameter);

    delay(10000);
}