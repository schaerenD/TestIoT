#include <Arduino.h>
#include <M5Stack.h>
#include "SX_mqtt.h"
#include "SX_modem.h"
#include "SX_display.h"
#include "M5_SIM7080G.h"

const String MQTT_Broker_URL = "thingsboard.cloud";
const String MQTT_Broker_Port = "1883";
const int MQTT_Broker_Keeptime = 60;
const int CLIENTID = 60;
const String MQTT_Subtopic = "sub_topic";


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

void post_mqqt(const String topic)
{
  extern M5_SIM7080G device;
  extern String readstr;

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

void test4_Post_SM(String topic)
{
  String JSON_Header = "{\"temperature\": ";
  String JSON_Second_Part = ",\r\n \"humidity\": ";
  String JSON_Third_Part = ",\r\n \"pressure\": ";
  String JSON_End = "}";
  String JSON1;
  extern float tmp;
  extern float hum;
  extern float pressure;

  extern M5_SIM7080G device;
  extern String readstr;

  extern uint8_t seconds;

  static int oldSeconds = 0;

  if(oldSeconds == seconds)
  {
    return;
  }
  oldSeconds = seconds;

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

void mqtt_init()
{
  init_mqqt("thingsboard.cloud","1883");
}