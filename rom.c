

#org 0xF80, 0xFFF { }
#define FLASH_EPROM_START 0xF80



//!#define write_int16_alarmrom write_int16_alarmrom
//!#define read_int16_alarmrom read_int16_alarmrom
/*
#ROM 0x2100 = {0xAA}

#ROM 0x2102 = {110}   //2 //rcalibvoltreading  on the channel 
#ROM 0x2104 = {110}   //4 //ycalibvoltreading  on the channel  
#ROM 0x2106 = {110}   //6 //bcalibvoltreading   on the channel
#ROM 0x2108 = {212}   //8 //dgrcalibvoltreading   on the channel

#ROM 0x210A = {227}   //10 // measured voltage at the time of calibration.   calibvolt               

#rom 0x210C ={4800}   //12    //4800 //battery reading, ie 48.00 btsbatcalibvolt
#rom 0x210E ={462}    //14  //reading on the battery channel; batcalibvoltreading 

#rom 0x2110   ={1184}      //16      // //dg battery reading, ie 11.84 dgbatcalibvolt                         
#rom 0x2112 ={202}      //18  //reading on the dg battery channel; dgbatcalibvoltreading 

#rom 0x2114 ={512}    //20      //mains adj full reading
#rom 0x2116 ={512}    //22      //bts bat adj full reading
#rom 0x2118 ={512}    //24      //dgbat adj full reading
#rom 0x211A ={512}    //26      //temperature adj full reading

*/

#separate
void write_int16_alarmrom(uint16_t address, uint16_t data)
{
   write_program_eeprom(FLASH_EPROM_START + address, data);
}

#separate
uint16_t read_int16_alarmrom(uint16_t address)
{
    //return(0);
   return(read_program_eeprom(address + FLASH_EPROM_START));
}

#separate
void read_rom_values(void)
{

   rcalibvoltreading=read_int16_alarmrom(2);
   ycalibvoltreading=read_int16_alarmrom(4);
   bcalibvoltreading=read_int16_alarmrom(6);
   dgrcalibvoltreading=read_int16_alarmrom(8);
   
   btsbatcalibvolt= read_int16_alarmrom(12);
   btsbatcalibvoltreading= read_int16_alarmrom(14);
   
   
   v_range_high = read_int16_alarmrom(20);   //voltage
   
   b_range_high = read_int16_alarmrom(22);     //bts battery  
   

}

//#separate
void write_calib_data(void)
{
   write_int16_alarmrom(10, calibvolt);
   
   write_int16_alarmrom(2, rcalibvoltreading);
   write_int16_alarmrom(4, ycalibvoltreading);
   write_int16_alarmrom(6, bcalibvoltreading);
   write_int16_alarmrom(8, dgrcalibvoltreading);

}

void ResetRomPhase(void)
{

#ifdef AL115v10
   write_int16_alarmrom(2, 119);
   write_int16_alarmrom(4, 119);
   write_int16_alarmrom(6, 119);
#else
   write_int16_alarmrom(2, 216);
   write_int16_alarmrom(4, 216);
   write_int16_alarmrom(6, 216);
#endif
   write_int16_alarmrom(8, 216);
   
   write_int16_alarmrom(10, 227);
}

void ResetRomBattery(void)
{
   write_int16_alarmrom(12, 4800);   //bts battery voltage at calib
#ifdef AL115v10
   write_int16_alarmrom(14, 468);   //bts reading at calib
#else
   write_int16_alarmrom(14, 425);   //bts reading at calib
#endif
    
}

void ResetRomKnob(void)
{
   write_int16_alarmrom(20, 512);   //knob
   write_int16_alarmrom(22, 512);   //knob
   write_int16_alarmrom(24, 512);   //knob
   write_int16_alarmrom(26, 512);   //knob   
}

void reset_rom(void)
{
   write_int16_alarmrom(0, 0xAA);
   
   ResetRomPhase();
   
   ResetRomBattery();
   
   ResetRomKnob();

}



/*

i2c slave data

1, 2   mains knob reading
3, 4   bts knob reading
5, 6   dg knob reading
7, 8   Temperature knob reading
9, 10   bts battery voltage
11, 12   dg battery voltage
13, 14   Temperature
15, 16   mains r voltage
17, 18   mains y voltage
19, 20   mains b voltage
21, 22   dgr voltage

i2c eeprom data

23, 24   mains r calibration voltage
25, 26   mains y calibration voltage
27, 28   mains b calibration voltage
29, 30   dgr calibration voltage
31, 32   mains knob calibration        //31 is all knob decalibration                             
33, 34   dg bat knob calibration
35, 36   bts bat knob calibration
37, 38   temperature knob calibration
39, 40   bts calibration voltage
41, 42   dgbat calibration voltage
43, 44   simulation mains fail
45, 46   simulation dg on
47, 48   simulation dg battery low
49, 50   simulation low fuel
51, 52   simulation high temperature
53, 54   simulation bts battery low
55, 56   simulation fire
57, 58   58 = decalibrate battery   57= decalibrate phases
59, 60   simulation time in minutes
61, 62   i2c write happened.

i2c i/o

63, 64   error code value

*/


