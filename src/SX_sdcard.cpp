#include <Arduino.h>
#include <M5Stack.h>
#include "SX_sensors.h"
#include "SX_display.h"

void sdcard_init()
{   
    String Test;
    if (!SD.begin()) 
    {
        display_debug_output("ERROR NO SD CARD");
        while (1);
    }

    File f = SD.open("/CONFIGURATION.txt", FILE_READ);
    if (f) {
        Test = f.readString();
        f.close();
    }
    else
    {
        display_debug_output("ERROR NO FILE WITH NAME CONFIGURATION.txt");
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
    int eigthParameterBegin = Test.indexOf('=',seventhParameterEnd);
    int eigthParameterEnd = Test.indexOf('\r',eigthParameterBegin);
    int ninethParameterBegin = Test.indexOf('=',eigthParameterEnd);
    int ninethParameterEnd = Test.indexOf('\r',ninethParameterBegin);
    int tenthParameterBegin = Test.indexOf('=',ninethParameterEnd);
    int tenthParameterEnd = Test.indexOf('\r',tenthParameterBegin);
    int eleventhParameterBegin = Test.indexOf('=',tenthParameterEnd);
    int eleventhParameterEnd = Test.indexOf('\r',eleventhParameterBegin);
    int twelvthParameterBegin = Test.indexOf('=',eleventhParameterEnd);
    int twelvthParameterEnd = Test.indexOf('\r',twelvthParameterBegin);
    int thirteenthParameterBegin = Test.indexOf('=',twelvthParameterEnd);
    int thirteenthParameterEnd = Test.indexOf('\r',thirteenthParameterBegin);
    int fourteenthParameterBegin = Test.indexOf('=',thirteenthParameterEnd);
    int fourteenthParameterEnd = Test.indexOf('\r',fourteenthParameterBegin);

    String firstParameter = Test.substring(firstParameterBegin+1, firstParameterEnd);
    String secondParameter = Test.substring(secondParameterBegin+1, secondParameterEnd);
    String thirdParameter = Test.substring(thirdParameterBegin+1, thirdParameterEnd);
    String fourthParameter = Test.substring(fourthParameterBegin+1, fourthParameterEnd);
    String fifthParameter = Test.substring(fifthParameterBegin+1, fifthParameterEnd);
    String sixthParameter = Test.substring(sixthParameterBegin+1, sixthParameterEnd);
    String seventhParameter = Test.substring(seventhParameterBegin+1, seventhParameterEnd);
    String eigthParameter = Test.substring(eigthParameterBegin+1, eigthParameterEnd);
    String ninethParameter = Test.substring(ninethParameterBegin+1, ninethParameterEnd);
    String tenthParameter = Test.substring(tenthParameterBegin+1, tenthParameterEnd);
    String eleventhParameter = Test.substring(eleventhParameterBegin+1, eleventhParameterEnd);
    String twelvthParameter = Test.substring(twelvthParameterEnd+1, twelvthParameterEnd);
    String thirteenthParameter = Test.substring(thirteenthParameterBegin+1, thirteenthParameterEnd);
    String fourteenthParameter = Test.substring(fourteenthParameterBegin+1, fourteenthParameterEnd);

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
    extern int sendtime;
    sendtime = tenthParameter.toInt();
    extern String URL;
    URL = eleventhParameter;
    extern String USERNAME;
    USERNAME = twelvthParameter;
    extern String CLIENTID;
    CLIENTID = thirteenthParameter;
    extern String PASSWORD;
    PASSWORD = fourteenthParameter;


    // Check Parameter Site One
    display_debug_output("CHECK PARAMETERS:\n\r");
    display_debug_output("eDRX Cycle: " + firstParameter);
    display_debug_output("\n\rPTW:  " + secondParameter);
    display_debug_output("\n\rT3412_Base:  " + thirdParameter);
    display_debug_output("\n\rT3412_Value:  " + fourthParameter);
    display_debug_output("\n\rT3324_Base:  " + fifthParameter); 
    display_debug_output("\n\rT3324_Value:  " + sixthParameter);
    display_debug_output("\n\rAPN:  " + seventhParameter);
    display_debug_output("\n\rURL:  " + eleventhParameter);
    display_debug_output("\n\rUSERNAME:  " + twelvthParameter);
    display_debug_output("\n\r\n\r\n\r\n\rPRESS LEFT BUTTON");
    CHECKBUTTON:
    M5.update();
    if(M5.BtnA.wasPressed())
    {delay(50);delay(50); }
    else
    {delay(50);delay(50);delay(50);delay(50);goto CHECKBUTTON;}
    display_debug_output("\n\r");
    display_debug_output("\n\r");
    display_debug_output("\n\r");
}