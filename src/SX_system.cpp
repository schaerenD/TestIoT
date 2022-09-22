#include "Ticker.h"
#include "SX_system.h"
#include "SX_energy.h"

Ticker standartTimer;
Ticker currentTimer;
uint8_t seconds = 0;
uint32_t connection_timeout = 86400;  // Name of the Vriabel is not so good choosen

void ticker_standart_callback()
{
  seconds++;
  connection_timeout--;
}

void ticker_init()
{
  standartTimer.attach_ms(1000,ticker_standart_callback);
  currentTimer.attach_ms(50,ticker_current_callback);
}