#include <Arduino.h>
#include <M5Stack.h>
#include "M5_ENV.h"
#include "M5GFX.h"
#include "M5_SIM7080G.h"

SHT3X sht30;
QMP6988 qmp6988;

float tmp      = 0.0;
float hum      = 0.0;
float pressure = 0.0;

const String MQTT_Broker_URL = "thingsboard.cloud";
const String MQTT_Broker_Port = "1883";
const int MQTT_Broker_Keeptime = 60;
const int CLIENTID = 60;
const String MQTT_Subtopic = "sub_topic";

M5GFX display;
M5Canvas canvas(&display);

M5_SIM7080G device;

String readstr;

void log(String str) 
{
    Serial.print(str);
    canvas.print(str);
    canvas.pushSprite(0, 0);
}

void send_at_command(const String atcommand, const String answer, int waittime)
{
  int error_counter = 0;
  while(1)
  {
    device.sendMsg(atcommand);       // We Add some AT Commands to give the Modem the Chance to Synchronize with UART
    readstr = device.waitMsg(waittime);
    //log("COMMAND: ");
    //log(atcommand);
    //log("ANSWER: ");
    log(readstr); // Printout Answer
    if (answer.indexOf("NO ANWSER CHECK EXPECTET") == 0)
    {
      //log("NO ANWSER CHECK EXPECTET\r\n");
      break;  // No Answer Check
    }
    else if(readstr.indexOf(answer) == -1) 
    {
      log("ERROR\r\n");
      error_counter++;    // Send Again
      continue;
    }
    else
    {
      //log("SUCESS\r\n");
      break;              // Sucess
    }
  }
}

void init_modem()
{
  send_at_command("AT\r\n", "OK", 1000);// We Add some AT Commands to give the Modem the Chance to Synchronize with UART
  send_at_command("AT+CREBOOT\r\n", "OK", 1000);// Reboot the Modul
  send_at_command("AT\r\n", "OK", 1000);//  Same as above. Synchonize UART Interface after Reboot
  send_at_command("AT\r\n", "OK", 1000);//  Same as above. Synchonize UART Interface after Reboot
  send_at_command("AT+CMEE=1\r\n", "OK", 1000);// Enable verbos Error Code 
  send_at_command("AT+CGATT=0\r\n", "OK", 3000);// Deatach from Network
  send_at_command("AT+CPIN?\r\n", "OK", 3000);// Check if SIM PIN is required
  send_at_command("AT+CGDCONT=1,\"IP\",\"gprs.swisscom.ch\"\r\n", "OK", 1000);// Set PDP Context with APN, IP and DHCP
  send_at_command("AT+CEREG=1\r\n", "OK", 1000);// ???
  send_at_command("AT+CNCFG=0,1,\"gprs.swisscom.ch\"\r\n", "OK", 1000);// Activate Context

  send_at_command("AT+CGATT=1\r\n", "OK", 1000);
}

void general_information()
{
  log("--------------------\r\n");
  send_at_command("AT+CSQ\r\n", "OK", 1000);
  send_at_command("AT+CGATT?\r\n", "OK", 1000);
  send_at_command("AT+COPS?\r\n", "OK", 1000);
  send_at_command("AT+CEREG?\r\n", "OK", 1000);
  send_at_command("AT+CNCFG?\r\n", "OK", 1000);
  send_at_command("AT+CPSI?\r\n", "OK", 1000);
  send_at_command("AT+CNACT?\r\n", "OK", 1000);
  send_at_command("AT+CGPADDR\r\n", "OK", 1000);
}

void init_mqqt()
{
  log("--------------------\r\n");
  send_at_command("AT+CNACT=0,1\r\n", "OK", 1000);     //  AT+CNACT=<pdpidx>,<action>  <pdpidx>=0: Context 0-3, <action>=0: Deactive, 1:Active, 2: Auto Active
  send_at_command("AT+CNACT?\r\n", "OK", 1000);
  send_at_command("AT+SMCONF=\"URL\",\"broker.emqx.io\",\"1883\"\r\n", "OK", 1000);
  send_at_command("AT+SMCONF=\"KEEPTIME\",60\r\n", "OK", 1000);
  send_at_command("AT+SMCONF=\"CLEANSS\",1\r\n", "OK", 1000);
  send_at_command("AT+SMCONF=\"CLIENTID\",\"simmqtt\"\r\n", "OK", 1000);
  send_at_command("AT+SMCONN\r\n", "OK", 5000);
}

    //device.sendMsg("AT+CNACT=0,1\r\n");   //  AT+CNACT=<pdpidx>,<action>  <pdpidx>=0: Context 0-3, <action>=0: Deactive, 1:Active, 2: Auto Active
    //readstr = device.waitMsg(200);
    //log(readstr);
//
    //device.sendMsg("AT+CNACT?\r\n");    // Answer is <pdpidx>=Context,<statusx>=Active(1) or Deactive(0),<addressx>=IP-Adresse
    //readstr = device.waitMsg(200);
    //log(readstr);
//
    //device.sendMsg("AT+SMCONF=\"URL\",\"broker.emqx.io\",\"1883\"\r\n");     // Set Broker Adress
    //readstr = device.waitMsg(1000);
    //log(readstr);
//
    //device.sendMsg("AT+SMCONF=\"KEEPTIME\",60\r\n");   // Set Keeptime, Hold Connection for XX Seconds, max is 65535 Seconds
    //readstr = device.waitMsg(1000);
    //log(readstr);
//
    //device.sendMsg("AT+SMCONF=\"CLEANSS\",1\r\n");  // Set Cleaness: 0: Resume communication based on persent session, 1: Resume communication with a new session
    //readstr = device.waitMsg(1000);
    //log(readstr);
//
    //device.sendMsg("AT+SMCONF=\"CLIENTID\",\"simmqtt\"\r\n");  // Set Client ID
    //readstr = device.waitMsg(1000);
    //log(readstr);
//
    //device.sendMsg("AT+SMCONN\r\n");    // MQTT Connection, OK or ERROR
    //readstr = device.waitMsg(5000);
    //log(readstr);
//
void post_mqqt()
{
  device.sendMsg("AT+SMSUB=\"sub_topic\",1\r\n");   // AT+SMSUB=<topic>,<qos>   <topic>=Topic to Subscripe,    <qos>=0:only one, 1:min one, 2:Only once
  readstr = device.waitMsg(1000);
  log(readstr);
  
  while(1)
  {
    M5.update();
    if(M5.BtnA.wasPressed()) 
    {
      device.sendMsg("AT+SMPUB=\"v1/devices/me/telemetry\",5,1,1\r\n");
      delay(100);
      device.sendMsg("hello\r\n");
    }
    readstr = device.waitMsg(0);
    Serial.print(readstr);
    log(readstr);
  }
}

void setup() 
{
  // put your setup code here, to run once:

  M5.begin(); //Init M5Core. Initialize M5Core
  display.begin();

  canvas.setColorDepth(1); // mono color
  canvas.createSprite(display.width(), display.height());
  canvas.setTextSize((float)canvas.width() / 160);
  canvas.setTextScroll(true);

  device.Init(&Serial2, 16, 17);

  //device.sendMsg("AT+CREBOOT\r\n");
  //delay(1000);
  //while(1)
  //{
  //      device.sendMsg("AT+CSQ\r\n");
  //      readstr = device.waitMsg(1000);
  //      log(readstr);
  //      if(readstr.indexOf("+CSQ: 99,99") ==-1)
  //      {
  //          break;
  //      }
  //}

  M5.Power.begin(); //Init Power module. Initialize the power module
 
  //M5.Lcd.print(M5.Lcd.height());    //Display the heigh on the screen
  //M5.Lcd.print(M5.Lcd.width());     //Display the width on the screen
 
                    /* Power chip connected to gpio21, gpio22, I2C device
                      Set battery charging voltage and current
                      If used battery, please call this function in your project */
  //M5.Lcd.setTextDatum(MC_DATUM);    //Set text alignment to center
  //M5.Lcd.drawString("hello", 160, 120, 2);    //Print the string in font 2 at (160, 120) hello
  //M5.Lcd.drawString("Gross", 170, 100, 4);    //Display the string Hello M5 in font 2 at (160, 100)

  Wire.begin();
  qmp6988.init();
  
  //M5.Lcd.fillScreen(RED);
  //M5.Lcd.print("Hello World"); // Print text on the screen (string) Print text on the screen (string)
}

void loop() 
{

  // put your main code here, to run repeatedly:
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

  //loop

  init_modem();
  init_mqqt();

  while(1)
  {
    general_information();
  }
}