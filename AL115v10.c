#case

#include <main.h>
#include <stdint.h>

#define LFL_DELAY               60      //in seconds
#define LFL_RESTORE_DELAY       30      //in seconds
#define MAINS_HIGH_VOLT 280

#define AL115v20
//#define AL115v10

/*
PIC16F1516 to PIC16F886 change
EEPROM START changed from high endurance location to 0.
read_int16_alarmrom changed to read_int16_eeprom
write_program_eeprom changed to write_int16_eeprom
uncommented #ROM locations.

*/
//#include <internal_eeprom.c>
#include "definitions.c"
#include "rom.c"
#include "checkconditions.c"
#include "functions.c"
#include "int.c"

#include "i2c_display.c"




#if defined (AL115v20) && defined (AL115v10)
   #error Multiple version definitions
#elif defined (AL115v20)
   #undef AL115v10
#elif defined (AL115v10)
   #undef AL115v20
#else
   #error Version Error.
#endif


void main()
{
   uint8_t temp_var;
//   setup_oscillator(OSC_4MHZ);

   setup_adc_ports(sAN0|sAN1|sAN2|sAN3|sAN5|sAN12|sAN13);
   setup_adc(ADC_CLOCK_INTERNAL);
//   setup_adc_reference(VREF_ADC_4v096);
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_8);      //524 ms overflow
 //  setup_wdt(WDT_64S);
   setup_wdt(WDT_OFF);
//!   setup_timer_2(T2_DIV_BY_16,255,16);      //4.0 ms overflow, 65.5 ms interrupt
//!
//!
//!   enable_interrupts(INT_TIMER0);
//!   enable_interrupts(INT_TIMER1);
//!   enable_interrupts(INT_TIMER2);
disable_interrupts(PERIPH);
//enable_interrupts(INT_RTCC);
disable_interrupts(INT_RB);
disable_interrupts(INT_EXT_L2H);
disable_interrupts(INT_EXT_H2L);
disable_interrupts(INT_EXT);
disable_interrupts(INT_AD);
disable_interrupts(INT_TBE);
disable_interrupts(INT_RDA);
disable_interrupts(INT_TIMER1);
disable_interrupts(INT_TIMER2);
disable_interrupts(INT_CCP1);
disable_interrupts(INT_CCP2);
disable_interrupts(INT_SSP);
disable_interrupts(INT_BUSCOL);
//disable_interrupts(INT_EEPROM);
disable_interrupts(INT_TIMER0);
enable_interrupts(INT_OSC_FAIL);
//disable_interrupts(INT_COMP);
//disable_interrupts(INT_COMP2);
//disable_interrupts(INT_ULPWU);
disable_interrupts(INT_RB0);
disable_interrupts(INT_RB1);
disable_interrupts(INT_RB2);
disable_interrupts(INT_RB3);
disable_interrupts(INT_RB4);
disable_interrupts(INT_RB5);
disable_interrupts(INT_RB6);
disable_interrupts(INT_RB7);

   enable_interrupts(INT_SSP);    
  
   output_low(RL_MAINS);
   output_low(RL_DG);
   output_high(RL_BATT);   output_low(LED_BATT);
   
   for(temp_var = 0; temp_var < 64; temp_var++){
      i2c_buffer[temp_var] = 0;
   }


   if(read_int16_alarmrom(0)!=0xAA)
      reset_rom();
   read_rom_values();
   init_settings();

    enable_interrupts(GLOBAL);
   while(TRUE)
   {
      checkconditions();
      enable_interrupts(INT_SSP);
      restart_wdt();
      delay_ms(500);
      restart_wdt();
      enable_interrupts(INT_SSP);
      if(i2c_buffer[62] == TRUE)
      {
         ProcessMasterData();
      }
      
   }

}

