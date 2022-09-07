#include "Ticker.h"
#include "SX_system.h"
#include "SX_energy.h"

Ticker standartTimer;
Ticker currentTimer;
uint8_t seconds = 0;

void ticker_standart_callback()
{
  seconds++;
}

void ticker_init()
{
  standartTimer.attach_ms(1000,ticker_standart_callback);
  currentTimer.attach_ms(50,ticker_current_callback);
}