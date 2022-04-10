#include <16F15354.h>
#device ADC=10

#FUSES WDT                      //Watch Dog Timer
#FUSES PUT                      //Power Up Timer
#FUSES PROTECT                  //Code protected from reads
#FUSES BROWNOUT_SW              //Brownout controlled by configuration bit in special file register
#FUSES BORV27                   //Brownout reset at 2.5V

#use delay(internal=4000000,restart_wdt)
#pin_select SDA1=PIN_C4
#pin_select SCL1=PIN_C3
#use i2c(SLAVE, I2C1, address=0xA0)
#use FIXED_IO(C_outputs = PIN_C2, PIN_C5, PIN_C6, PIN_C7)

