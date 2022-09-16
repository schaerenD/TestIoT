#include <Arduino.h>
#include "M5_SIM7080G.h"
#include "SX_display.h"
#include <M5Stack.h>

String apn_name;

uint32_t TAU_T3412_Time_int = 0; 
String TAU_T3412_Time_String = "";
String TAU_T3412_Time_Command_String = "";
uint16_t TAU_T3412_Time_Command_int;

uint32_t Activetime_T3324_Time_int = 0; 
String Activetime_T3324_Time_String = "";
String Activetime_T3324_Time_Command_String = "";
uint16_t Activetime_T3324_Time_Command_int;

const float eDRX_Hyperframetime = 10.24;
const float eDRX_Pageingtime = 2.56;
uint32_t eDRX_Time_int = 0; 
String eDRX_Time_String = "";
String eDRX_Time_Command_String = "";

M5_SIM7080G device;

// eDRX Values
int eDRX_Value = 1;
int Pageinig_Window = 0;

// PSM Values
int TAU_T3412_Unit = 1; // T3412 
int TAU_T3412_Value = 1;
const uint32_t TAU_Unit_Values_seconds[7] = {600, 3600, 36000, 2, 30, 60, 1152000};

int Activetime_T3324_Unit = 1; // T3324  
int Activetime_T3324_Value = 1;
const uint32_t Activetime_Unit_Values_seconds[3] = {2, 60, 360};

String readstr;

void calc_TAU_Activetime_eDRX()
{
  uint32_t TimeBaseSeconds;

  // Calc TAU
  TimeBaseSeconds = TAU_Unit_Values_seconds[TAU_T3412_Unit];
  TAU_T3412_Time_int = TimeBaseSeconds*TAU_T3412_Value;
  TAU_T3412_Time_String = String(TAU_T3412_Time_int);

  ////////display_debug_output("TAU_T3412_Unit:");
  ////////display_debug_output(String(TAU_T3412_Unit));
////////
  ////////display_debug_output("TAU_T3412_Value:");
  ////////display_debug_output(String(TAU_T3412_Value));
////////
  ////////display_debug_output("TimeBaseSeconds:");
  ////////display_debug_output(String(TimeBaseSeconds));
////////

  TAU_T3412_Time_Command_int = (0b11100000 & (TAU_T3412_Unit<<5));  // Set Unit
  TAU_T3412_Time_Command_int = TAU_T3412_Time_Command_int | (0b00011111 & TAU_T3412_Value);  // Set Value

  display_debug_output("TAU_T3412_Time_int:");
  display_debug_output(String(TAU_T3412_Time_int));
  display_debug_output("TAU_T3412_Time_Command_int:");
  display_debug_output(String(TAU_T3412_Time_Command_int));

  // Calc Activetime
  TimeBaseSeconds = Activetime_Unit_Values_seconds[Activetime_T3324_Unit];
  Activetime_T3324_Time_int = TimeBaseSeconds*Activetime_T3324_Value;
  Activetime_T3324_Time_String = String(Activetime_T3324_Time_int);

  Activetime_T3324_Time_Command_int = (0b11000000 & (Activetime_T3324_Unit<<6));  // Set Unit
  Activetime_T3324_Time_Command_int = Activetime_T3324_Time_Command_int | (0b00111111 & Activetime_T3324_Value);  // Set Value

  ///////display_debug_output("Activetime_T3324_Value:");
  ///////display_debug_output(String(Activetime_T3324_Value));
///////
  ///////display_debug_output("Activetime_T3324_Unit:");
  ///////display_debug_output(String(Activetime_T3324_Unit));
///////
  ///////display_debug_output("TimeBaseSeconds:");
  ///////display_debug_output(String(TimeBaseSeconds));
///////
  display_debug_output("----------------\r\n");
  display_debug_output("Activetime_T3324_Time_String:");
  display_debug_output(Activetime_T3324_Time_String);
  display_debug_output("Activetime_T3324_Time_Command_int:");
  display_debug_output(String(Activetime_T3324_Time_Command_int));

  // Calc Activetime
  eDRX_Time_int = TimeBaseSeconds*eDRX_Time_int;
  eDRX_Time_String = String(eDRX_Time_int);

  uint8_t eDRX_Hyperframetime = 0;
  uint8_t eDRX_Pageingtime = 0;
}

void send_at_command(const String atcommand, const String answer, int waittime)
{
  int error_counter = 0;
  while(1)
  {
    device.sendMsg(atcommand);       // We Add some AT Commands to give the Modem the Chance to Synchronize with UART
    readstr = device.waitMsg(waittime);
    display_debug_output(readstr); // Printout Answer
    if (answer.indexOf("NO ANWSER CHECK EXPECTET") == 0)
    {
      //log("NO ANWSER CHECK EXPECTET\r\n");
      break;  // No Answer Check
    }
    else if(readstr.indexOf(answer) == -1) 
    {
      display_debug_output("ERROR\r\n");
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

void connect_modem()
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

void read_csq_readcommand()
{
  String response_string = readstr;//"AT+CEDRXRDP\r\n+CEDRXRDP: 5,\"0000\",\"0010\",\"0100\"\n\r\n\rOK";

  String answerHeader = "+CSQ: ";
  int answerHeaderLength = answerHeader.length();

  int rssiValue;
  int berValue;

  int startOfAnswer = response_string.indexOf(answerHeader);
  if (startOfAnswer != -1)
  {
    // First Parameter
    int firstComma = response_string.indexOf(',');
    int secondComma =  response_string.indexOf(',', firstComma + 1 );
    int endOfAnswer = response_string.length();

    String firstParameter = response_string.substring(startOfAnswer+answerHeaderLength, firstComma);
    String secondParameter = response_string.substring(firstComma+1, endOfAnswer);

    rssiValue = firstParameter.toInt();
    int rssiValue_dBm = 52 + (31 - rssiValue)*2;

    String rssiValue_String = String(rssiValue, DEC);
    String rssiValue_dBm_String = String(rssiValue_dBm, DEC);
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

    String firstParameter = response_string.substring(startOfAnswer+answerHeaderLength, firstComma);
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

    int answer_edrx_actType = firstParameter.toInt();
    int answer_edrx_requested_eDRX_value = secondParameter.toInt();
    int answer_edrx_nw_provided_eDRX_value = thirdParameter.toInt();
    int answer_edrx_paging_time_window = fourthParameter.toInt();

    String answer_edrx_requested_eDRX_value_String = String(answer_edrx_requested_eDRX_value, BIN);
    String answer_edrx_nw_provided_eDRX_value_String = String(answer_edrx_nw_provided_eDRX_value, BIN);
    String answer_edrx_paging_time_window_String = String(answer_edrx_paging_time_window, BIN);
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

    String firstParameter = response_string.substring(startOfAnswer+answerHeaderLength, firstComma);
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

    int answer_psm_mode = firstParameter.toInt();
    int answer_psm_requested_active_time = secondParameter.toInt();
    int answer_psm_requested_periodic_tau = thirdParameter.toInt();
    int answer_psm_network_active_time = fourthParameter.toInt();
    int answer_psm_network_T3412_ext_value = fifthParameter.toInt();
    int answer_psm_network_T3412_value = sixthParameter.toInt();
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

    String firstParameter = response_string.substring(startOfAnswer+answerHeaderLength, firstComma);
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

void psm_settings()
{
    char T3412_Value_16Bit[17] = {0};
    char T3324_Value_16Bit[17] = {0};
    char T3412_Value_8Bit[9] = {0};
    char T3324_Value_8Bit[9] = {0};
    sprintf (T3324_Value_8Bit, "%08d", Activetime_T3324_Time_Command_int);
    sprintf (T3412_Value_8Bit, "%08d", TAU_T3412_Time_Command_int);
    String T3324_Value_8Bit_String = String(T3324_Value_8Bit);
    String T3412_Value_8Bit_String;

    // Change Times in Binary String Format with correspending Zeros to the Begin
    TAU_T3412_Time_Command_int = TAU_T3412_Time_Command_int + 256;
    itoa(TAU_T3412_Time_Command_int,T3412_Value_16Bit, 2);
    
    for(int d=0;d<8;d++)
    {
      T3412_Value_8Bit[d] = T3412_Value_16Bit[d + 1];
    }

    T3412_Value_8Bit_String = String(T3412_Value_8Bit);

    Activetime_T3324_Time_Command_int = Activetime_T3324_Time_Command_int + 256;
    itoa(Activetime_T3324_Time_Command_int,T3324_Value_16Bit, 2);
    
    for(int d=0;d<8;d++)
    {
      T3324_Value_8Bit[d] = T3324_Value_16Bit[d + 1];
    }

    T3324_Value_8Bit_String = String(T3324_Value_8Bit);

    String OutputString = "AT+CPSMS=1,,,\"" + T3412_Value_8Bit_String + "\",\"" + T3324_Value_8Bit_String + "\"\r\n";

    //Send Command;
    send_at_command("AT+CSQ\r\n", "OK", 1000);


}

void edrx_settings()
{
    char eDRX_Cylcel[9];
    char acessTechnologie; //Todo:  set 4 for LTE CAT M1

    

    send_at_command("AT+CEDRXS=1,5,\"0010\"\r\n", "OK", 1000); // Set Cycle Length
    send_at_command("AT+CEDRX=2,1,3,2\r\n", "OK", 1000); // Read PTW (Pageingtimewindow Values
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

void ping(const String URL)
{
  log("--------------------\r\n");
  send_at_command("AT+SNPDPID=0\r\n", "OK", 1000);    // Take PDP Context 0 for Ping's
  String Conf = "AT+SNPING4=\"" + URL + "\",3,16,1000\r\n";
  send_at_command(Conf, "OK", 8000);      // IPV4 Ping Sending
}

void test1_calculation_for_Opertional()
{
  calc_TAU_Activetime_eDRX();
}

void modem_init()
{
  device.Init(&Serial2, 16, 17);
  //connect_modem();

  calc_TAU_Activetime_eDRX();
  psm_settings();
  edrx_settings();


}