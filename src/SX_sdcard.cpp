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
        f.close();
    }
    else
    {
        // ToDo: File not found
    }

    // Init SD Card
    int firstParameterBegin = Test.indexOf('=');
    int firstParameterEnd = Test.indexOf('\r');
    int secondParameterBegin = Test.indexOf('=',firstParameterEnd);
    int secondParameterEnd = Test.indexOf('\r',secondParameterBegin);
    int thirdParameterBegin = Test.indexOf('=',secondParameterEnd);
    int thirdParameterEnd = Test.indexOf('\r',thirdParameterBegin);
    int fourthParameterBegin = Test.indexOf('=',thirdParameterEnd);
    int fourthParameterEnd = Test.indexOf('\r',fourthParameterBegin);
    int fifthParameterBegin = Test.indexOf('=',fourthParameterEnd);
    int fifthParameterEnd = Test.indexOf('\r',fifthParameterBegin);
    int sixthParameterBegin = Test.indexOf('=',fifthParameterEnd);
    int sixthParameterEnd = Test.indexOf('\r',sixthParameterBegin);    
    int seventhParameterBegin = Test.indexOf('=',sixthParameterEnd);
    int seventhParameterEnd = Test.indexOf('\r',seventhParameterBegin);

    String firstParameter = Test.substring(firstParameterBegin+1, firstParameterEnd);
    String secondParameter = Test.substring(secondParameterBegin+1, secondParameterEnd);
    String thirdParameter = Test.substring(thirdParameterBegin+1, thirdParameterEnd);
    String fourthParameter = Test.substring(fourthParameterBegin+1, fourthParameterEnd);
    String fifthParameter = Test.substring(fifthParameterBegin+1, fifthParameterEnd);
    String sixthParameter = Test.substring(sixthParameterBegin+1, sixthParameterEnd);
    String seventhParameter = Test.substring(seventhParameterBegin+1, seventhParameterEnd);

    extern int eDRX_Value;
    eDRX_Value = firstParameter.toInt();
    extern int Pageinig_Window;
    Pageinig_Window = secondParameter.toInt();
    extern int TAU_T3412_Unit;
    TAU_T3412_Unit = thirdParameter.toInt();
    extern int TAU_T3412_Value;
    TAU_T3412_Value = fourthParameter.toInt();
    extern int Activetime_T3324_Unit;
    Activetime_T3324_Unit= fifthParameter.toInt();
    extern int Activetime_T3324_Value;
    Activetime_T3324_Value = sixthParameter.toInt();
    extern String apn_name;
    apn_name = seventhParameter;
}