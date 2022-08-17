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
M5Canvas sub_canvas1(&display);
M5Canvas sub_canvas2(&display);
M5Canvas sub_canvas_text(&display);


M5_SIM7080G device;

String readstr;

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
  send_at_command("AT+CMEE=2\r\n", "OK", 1000);// Enable verbos Error Code 
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
  //read_csq_readcommand();
  send_at_command("AT+CGATT?\r\n", "OK", 1000);
  send_at_command("AT+COPS?\r\n", "OK", 1000);
  send_at_command("AT+CEREG?\r\n", "OK", 1000);
  send_at_command("AT+CNCFG?\r\n", "OK", 1000);
  send_at_command("AT+CPSI?\r\n", "OK", 1000);
  send_at_command("AT+CNACT?\r\n", "OK", 1000);
  send_at_command("AT+CGPADDR\r\n", "OK", 1000);
}

void init_mqqt(const String URL, const String Port)
{
  log("--------------------\r\n");
  send_at_command("AT+CNACT=0,1\r\n", "OK", 1000);     //  AT+CNACT=<pdpidx>,<action>  <pdpidx>=0: Context 0-3, <action>=0: Deactive, 1:Active, 2: Auto Active
  send_at_command("AT+CNACT?\r\n", "OK", 1000);
  String Conf = "AT+SMCONF=\"URL\",\"" + URL + "\",\"" + Port + "\"\r\n";
  //send_at_command("AT+SMCONF=\"URL\",\"test.mosquitto.org\",\"1883\"\r\n", "OK", 1000);
  send_at_command(Conf, "OK", 1000);
  send_at_command("AT+SMCONF=\"KEEPTIME\",30\r\n", "OK", 1000);
  send_at_command("AT+SMCONF=\"CLEANSS\",1\r\n", "OK", 1000);
  send_at_command("AT+SMCONF=\"CLIENTID\",\"1fcba86c-e310-4cb3-862d-8a6ed676f5c6\"\r\n", "OK", 1000);
  send_at_command("AT+SMCONF=\"USERNAME\",\"h3LGa97iU0kz5ufuKlCQ\"\r\n", "OK", 1000);
  send_at_command("AT+SMCONF=\"PASSWORD\",\"1234\"\r\n", "OK", 1000);
  send_at_command("AT+SMCONF=\"QOS\",\"0\"\r\n", "OK", 1000);
  //send_at_command("AT+SMCONF=\"TOPIC\",\"v1/devices/me/telemetry\"\r\n", "OK", 1000);
  //send_at_command("AT+SMCONF=\"ASYNCMODE\",\"1\"\r\n", "OK", 1000);
  //send_at_command("AT+SMCONF=\"SUBHEX\",\"1\"\r\n", "OK", 1000);
  send_at_command("AT+SMCONF?\r\n", "OK", 1000);
  send_at_command("AT+SMCONN\r\n", "OK", 5000);
  send_at_command("AT+SMSTATE?\r\n", "OK", 1000);
}

void ping(const String URL)
{
  log("--------------------\r\n");
  send_at_command("AT+SNPDPID=0\r\n", "OK", 1000);    // Take PDP Context 0 for Ping's
  String Conf = "AT+SNPING4=\"" + URL + "\",3,16,1000\r\n";
  send_at_command(Conf, "OK", 8000);      // IPV4 Ping Sending
}

void post_mqqt(const String topic)
{
  //device.sendMsg("AT+SMSUB=\"sub_topic\",0\r\n");   // AT+SMSUB=<topic>,<qos>   <topic>=Topic to Subscripe,    <qos>=0:only one, 1:min one, 2:Only once
  //readstr = device.waitMsg(2000);
  //log(readstr);
  
  send_at_command("AT+SMSTATE?\r\n", "OK", 1000);
  
  //device.sendMsg("AT+SMPUB=\"v1/devices/me/telemetry\",5,1,1\r\n");
  String Conf = "AT+SMPUB=\"" + topic + "\",19,0,0\r\n";
  device.sendMsg(Conf);
  delay(500);
  device.sendMsg("{\"temperature\": 25}");
  readstr = device.waitMsg(20000);

  log(readstr);
}

void post_mqtt_temp(const String topic)
{
  String JSON_Header = "{\"temperature\": ";
  String JSON_Second_Part = ",\r\n \"humidity\": ";
  String JSON_Third_Part = ",\r\n \"pressure\": ";
  String JSON_End = "}";
  String JSON1;
  String JSON2;

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

  // String JOSN
  JSON1 = JSON_Header + tmp + JSON_Second_Part + hum + JSON_Third_Part + pressure + JSON_End;
  int JSON_length = JSON1.length();

  String JSON_length_String = "";
  JSON_length_String.concat(JSON_length);

  send_at_command("AT+SMSTATE?\r\n", "OK", 1000);
  
  //device.sendMsg("AT+SMPUB=\"v1/devices/me/telemetry\",5,1,1\r\n");
  String Conf = "AT+SMPUB=\"" + topic + "\"," + JSON_length_String + ",0,0\r\n";
  device.sendMsg(Conf);
  delay(500);
  device.sendMsg(JSON1);
  readstr = device.waitMsg(5000);
  log(readstr);
}

void read_csq_readcommand()
{
  String response_string = readstr;//"AT+CEDRXRDP\r\n+CEDRXRDP: 5,\"0000\",\"0010\",\"0100\"\n\r\n\rOK";

  String answerHeader = "+CSQ: ";
  int answerHeaderLength = answerHeader.length();

  int startOfAnswer = response_string.indexOf(answerHeader);
  if (startOfAnswer != -1)
  {
    // First Parameter
    int firstComma = response_string.indexOf(',');
    int secondComma =  response_string.indexOf(',', firstComma + 1 );
    int endOfAnswer = response_string.length();

    String firstParameter = response_string.substring(startOfAnswer+10, firstComma);
    String secondParameter = response_string.substring(firstComma+1, endOfAnswer);

    log("\n\r First Parameter:");
    log(firstParameter);
    log("\n\r Second Parameter:");
    log(secondParameter);
  }
}

void read_cedrxrdp_readcommand()
{
  String response_string = readstr;//"AT+CEDRXRDP\r\n+CEDRXRDP: 5,\"0000\",\"0010\",\"0100\"\n\r\n\rOK";

  String answerHeader = "+CEDRXRDP: ";
  int answerHeaderLength = answerHeader.length();

  int startOfAnswer = response_string.indexOf(answerHeader);
  if (startOfAnswer != -1)
  {
    // First Parameter
    int firstComma = response_string.indexOf(',');
    int secondComma =  response_string.indexOf(',', firstComma + 1 );
    int thirdComma =  response_string.indexOf(',', secondComma + 1 );
    int endOfAnswer = response_string.length();

    String firstParameter = response_string.substring(startOfAnswer+10, firstComma);
    String secondParameter = response_string.substring(firstComma+1, secondComma);
    String thirdParameter = response_string.substring(secondComma+1, thirdComma);
    String fourthParameter = response_string.substring(thirdComma+1, endOfAnswer);

    log("\n\r First Parameter:");
    log(firstParameter);
    log("\n\r Second Parameter:");
    log(secondParameter);
    log("\n\r Third Parameter:");
    log(thirdParameter);
    log("\n\r Fourth Parameter:");
    log(fourthParameter);
  }
}

void read_cpsmrdp_readcommand()
{
  String response_string = readstr;   //"AT+CPSMRDP\r\n+CPSMRDP: 0,20,14400,0,0,4320\n\r\n\rOK";

  String answerHeader = "+CPSMRDP: ";
  int answerHeaderLength = answerHeader.length();

  int startOfAnswer = response_string.indexOf(answerHeader);
  if (startOfAnswer != -1)
  {
    // First Parameter
    int firstComma = response_string.indexOf(',');
    int secondComma =  response_string.indexOf(',', firstComma + 1 );
    int thirdComma =  response_string.indexOf(',', secondComma + 1 );
    int fourthComma =  response_string.indexOf(',', thirdComma + 1 );
    int fifthComma =  response_string.indexOf(',', fourthComma + 1 );
    int endOfAnswer = response_string.length();

    String firstParameter = response_string.substring(startOfAnswer+10, firstComma);
    String secondParameter = response_string.substring(firstComma+1, secondComma);
    String thirdParameter = response_string.substring(secondComma+1, thirdComma);
    String fourthParameter = response_string.substring(thirdComma+1, fourthComma);
    String fifthParameter = response_string.substring(fourthComma+1, fifthComma);
    String sixthParameter = response_string.substring(fifthComma+1, endOfAnswer);

    log("\n\r First Parameter:");
    log(firstParameter);
    log("\n\r Second Parameter:");
    log(secondParameter);
    log("\n\r Third Parameter:");
    log(thirdParameter);
    log("\n\r Fourth Parameter:");
    log(fourthParameter);
    log("\n\r Fifth Parameter:");
    log(fifthParameter);
    log("\n\r Sixth Parameter:");
    log(sixthParameter);
  }
}

void read_cpsms_readcommand()
{
  String response_string = readstr; //"AT+CPSMS?\r\n+CPSMS: 0,,,\"01100000\",\"00000000\"\n\r\n\rOK";

  String answerHeader = "+CPSMS: ";
  int answerHeaderLength = answerHeader.length();

  int startOfAnswer = response_string.indexOf(answerHeader);
  if (startOfAnswer != -1)
  {
    // First Parameter
    int firstComma = response_string.indexOf(',');
    int secondComma =  response_string.indexOf(',', firstComma + 1 );
    int thirdComma =  response_string.indexOf(',', secondComma + 1 );
    int fourthComma =  response_string.indexOf(',', thirdComma + 1 );
    int endOfAnswer = response_string.length();

    String firstParameter = response_string.substring(startOfAnswer+8, firstComma);
    String secondParameter = response_string.substring(firstComma+1, secondComma);
    String thirdParameter = response_string.substring(secondComma+1, thirdComma);
    String fourthParameter = response_string.substring(thirdComma+1, fourthComma);
    String fifthParameter = response_string.substring(fourthComma+1, endOfAnswer);

    log("\n\r First Parameter:");
    log(firstParameter);
    log("\n\r Second Parameter:");
    log(secondParameter);
    log("\n\r Third Parameter:");
    log(thirdParameter);
    log("\n\r Fourth Parameter:");
    log(fourthParameter);
    log("\n\r Fifth Parameter:");
    log(fifthParameter);
  }
}

void power_save_settings()
{
  send_at_command("AT+CPSMS?\r\n", "OK", 1000); // Read PSM Values
  read_cpsms_readcommand();
  send_at_command("AT+CPSMRDP\r\n", "OK", 1000); // eDRX Values Read
  read_cpsms_readcommand();
  send_at_command("AT+CEDRXRDP\r\n", "OK", 1000); // eDRX Values
  read_cedrxrdp_readcommand();
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

  M5.Power.begin(); //Init Power module. Initialize the power module

  Wire.begin();
  qmp6988.init();
}

void loop() 
{

  setup_canvas_write_test();
  canvas_write_test("BLA");
  canvas_write_test("BLA");
  canvas_write_test("BLA");
  canvas_write_test("BLA");
  canvas_write_test("BLA");
  canvas_write_test("sub_canvas_textsub_canvas_textsub_canvas_textsub_canvas_textsub_canvas_textsub_canvas_textsub_canvas_textsub_canvas_textsub_canvas_textsub_canvas_textsub_canvas_textsub_canvas_textsub_canvas_text");


  // put your main code here, to run repeatedly:
  //loop
  init_modem();
  init_mqqt("thingsboard.cloud","1883");
  //init_mqqt("test.mosquitto.org","1883");

  while(1)
  {
    //general_information();
    //ping("www.google.com");
    //post_mqqt("v1/devices/me/telemetry");

    ////send_at_command("AT+CPSMSTATUS=1\r\n", "OK", 1000);
    ////send_at_command("AT+IPR=115200\r\n", "OK", 1000);
    ////send_at_command("AT+CEREG=4\r\n", "OK", 1000);
    ////send_at_command("AT+CEREG?\r\n", "OK", 1000);

    power_save_settings();
    send_at_command("AT+CSQ\r\n", "OK", 1000); // eDRX Values
    read_csq_readcommand();

    //general_information();

    ////power_save_settings();
    ////power_save_settings();
    ////power_save_settings();
    ////power_save_settings();
    ////power_save_settings();
    ////power_save_settings();
    ////power_save_settings();
    ////power_save_settings();
    ////power_save_settings();
    ////power_save_settings();
////
    //post_mqtt_temp("v1/devices/me/telemetry");
    //post_mqtt_temp("v1/devices/me/telemetry");
    //post_mqtt_temp("v1/devices/me/telemetry");
    //post_mqtt_temp("v1/devices/me/telemetry");
    //post_mqtt_temp("v1/devices/me/telemetry");
    //post_mqtt_temp("v1/devices/me/telemetry");
    //post_mqtt_temp("v1/devices/me/telemetry");
  }
}