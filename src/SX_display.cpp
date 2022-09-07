#include <Arduino.h>
#include <M5Stack.h>
#include "M5GFX.h"
#include "SX_display.h"
#include "SX_sensors.h"

M5GFX display;
M5Canvas canvas(&display);

M5Canvas sub_canvas1(&display);
M5Canvas sub_canvas2(&display);
M5Canvas sub_canvas_text(&display);

void log(String str) 
{
  Serial.print(str);
  canvas.print(str);
  canvas.pushSprite(0, 120);
}

void setup_canvas()
{
  sub_canvas1.createSprite(30, 30);
  sub_canvas1.fillSprite(TFT_BLUE);
  sub_canvas1.println("sub1");
  sub_canvas1.fillCircle(15, 15, 5, TFT_YELLOW);
  sub_canvas2.createSprite(30, 30);
  sub_canvas2.fillSprite(TFT_GREEN);
  sub_canvas2.println("sub2");
  sub_canvas2.fillTriangle(15, 10, 0, 30, 30, 30, TFT_BLUE);
  
  sub_canvas1.pushSprite(5, 30);
  sub_canvas2.pushSprite(40, 30);
}

void setup_canvas_write_test()
{
  sub_canvas_text.createSprite(100, 100);
  sub_canvas_text.fillSprite(TFT_BLUE);
  sub_canvas_text.pushSprite(40, 30);
}

void canvas_write_test(String str)
{
  sub_canvas_text.println(str);
  sub_canvas_text.pushSprite(40, 30);
}

void MeteoDisplay()
{
  M5Canvas MeteoCanvas(&display);
  String OutputSting;
  extern float tmp;
  extern float hum;
  extern float pressure;

  extern uint8_t seconds;


  OutputSting = "Temperature: " + String(tmp) + "°C\r\nHumidity: " + String(hum) + "%\r\nPressure: " + String(seconds) + "Pa";

  MeteoCanvas.createSprite(230, 120);
  MeteoCanvas.fillSprite(TFT_BLACK);
  MeteoCanvas.println("METEO\n\r");
  MeteoCanvas.println(OutputSting);
  MeteoCanvas.pushSprite(0, 0);
}

void ValuesDisplay()
{
  M5Canvas ValuesCanvas(&display);
  String OutputSting;
  extern String TAU_T3412_Time_String;
  extern String Activetime_T3324_Time_String;
  extern String eDRX_Time_String;

  OutputSting = "TAU: " + String(TAU_T3412_Time_String) + "Activtime: " + String(Activetime_T3324_Time_String) + "\r\eDRX: " + String(eDRX_Time_String);

  ValuesCanvas.createSprite(230, 120);
  ValuesCanvas.fillSprite(TFT_BLACK);
  ValuesCanvas.println("VALUES\n\r");
  ValuesCanvas.println(OutputSting);
  ValuesCanvas.pushSprite(0, 0);
}

void CategoriesDisplay()
{
  M5Canvas CategoriesCanvas(&display);
  String OutputSting;
  extern float tmp;
  extern float hum;
  extern float pressure;

  OutputSting = "Temperature: " + String(tmp) + "\r\nActive: " + String(hum) + "\r\nPressure: " + String(pressure);

  CategoriesCanvas.createSprite(230, 120);
  CategoriesCanvas.fillSprite(TFT_BLACK);
  CategoriesCanvas.println("CATEGORIES\n\r");
  CategoriesCanvas.println(OutputSting);
  CategoriesCanvas.pushSprite(0, 0);
}

void display_init()
{
  display.begin();
  canvas.setColorDepth(1); // mono color
  canvas.createSprite(display.width(), display.height());
  canvas.setTextSize((float)canvas.width() / 160);
  canvas.setTextScroll(true);
}

void test2_Screen_SM()
{
  static int forgrundDisplay = 0;
  static int oldSeconds = 0;
  extern uint8_t seconds;

  if(oldSeconds == seconds)
  {
    return;
  }
  oldSeconds = seconds;
  switch (forgrundDisplay)
  {
    case 0:
      MeteoDisplay(); // Display Meteo
      break;
    case 1:
      ValuesDisplay();  // Display Values
      break;
    case 2:
      CategoriesDisplay(); // Display Kategorie
      break;
    default:
      forgrundDisplay = 0;
      break;
  }
  forgrundDisplay++;
  if (forgrundDisplay > 2)
  {
    forgrundDisplay = 0;
  }    
}

/**
Energy Statemachine

Debug Output

@param  This Function is only used to pull out Debug Strings to the Display
@param  String to Output
*/
void display_debug_output(String outputstring)
{
  Serial.print(outputstring);
  canvas.print(outputstring);
  canvas.pushSprite(0, 0);
}