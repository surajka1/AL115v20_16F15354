#separate
uint16_t phasecheck(void)
{   
   uint8_t k=0;
   uint16_t value5=0,value=0;
    for(k=0;k<10;k++)
    {
      delay_us(2000);
      value=read_adc();
      value5=value5+value;
    }     
    value=value5/10;
    return(value);
}

uint16_t phasecheck3(void)
{   
   uint8_t k=0;
   int1 done = adc_done();
   uint32_t value5=0,value=0;
    for(k=0;k<200;k++)
    {
      delay_us(100);
      value=read_adc();
      while(!done) {   
         done = adc_done();
      }

      value5=value5+value;
    }     
    value=value5/200;
    return((uint16_t)value);
}

#separate
uint16_t phasecheck2(void)
{   
   uint16_t k=0;
   uint16_t value5=0,value=0;
    for(k=0;k<10;k++)
    {
      delay_us(2000);
      value=read_adc();
      value5=value5+value;
    }     
    return(value5);
}

#separate
void find_adj_volt()
{

   VOLT_ADJ_SELECT();
   delay_ms(1);
   volt_adj_reading=phasecheck()*10;
   phase_adjusted_volt=   130+ (float)phasecheck()*v_adj_ratio;
   
   BAT_ADJ_SELECT();
//   delay_ms(1);
   bat_adj_reading=(float)phasecheck2()*b_adj_ratio +  4600;   //1v = 100
   
}


void checkconditions()
{
//!   unsigned i=0;
   
////////////////////voltage adjustment select/////////
   find_adj_volt();
   
////////////////mains voltage/////////
   MAINS_R_SELECT();
//   delay_ms(1);
   r_phase_volt=phasecheck()*rvoltratio;
   
   MAINS_Y_SELECT();

   y_phase_volt=phasecheck()*yvoltratio;
   
   MAINS_B_SELECT();

   b_phase_volt=phasecheck()*bvoltratio;
/*
   if(!mains_ok)
      phase_adjusted_volt=phase_adjusted_volt+3;
   mains_ok=((r_phase_volt>phase_adjusted_volt)&&(y_phase_volt>phase_adjusted_volt)&&(b_phase_volt>phase_adjusted_volt));
   if((r_phase_volt>MAINS_HIGH_VOLT)||(y_phase_volt>MAINS_HIGH_VOLT)||(b_phase_volt>MAINS_HIGH_VOLT))
      mains_ok=0;
*/

///r
   if(r_ok ==0)
      r_ok = r_phase_volt > (phase_adjusted_volt + 3);
   else
      r_ok = r_phase_volt > phase_adjusted_volt;

   
///y   
   if(y_ok ==0)
      y_ok = y_phase_volt > (phase_adjusted_volt + 3);
   else
      y_ok = y_phase_volt > phase_adjusted_volt;


///b   
   if(b_ok ==0)
      b_ok = b_phase_volt > (phase_adjusted_volt + 3);
   else
      b_ok = b_phase_volt > phase_adjusted_volt;
   
#if defined (AL115v10)
   output_bit(LED_Y, y_ok);
   output_bit(LED_R, r_ok);
   output_bit(LED_B, b_ok);   
#else 
    output_bit(LED_Y, !y_ok);
   output_bit(LED_R, !r_ok);
   output_bit(LED_B, !b_ok);    
#endif
   
   
  if((r_ok == 1) && (y_ok == 1) && (b_ok == 1))
   mains_ok =1;
  else 
   mains_ok = 0;
   
   find_adj_volt();
   output_bit(RL_MAINS, !mains_ok );
//////////////////dg voltage /////////////////////

   DG_SELECT();
   delay_ms(1);
   dg_volt=phasecheck()*dgrvoltratio;
   output_bit(RL_DG, (dg_volt>120)); 

   
/////////////////////////////BTS battery voltage monitoring/////////////////////
////////////////bts battery adjustment

   
   BATT_SELECT();

   delay_ms(1);
   bat_volt_lu = phasecheck3()*btsbatvoltratio;
   bat_row_an = read_adc();
   if(bat_low_already_detected)
   {
      if(bat_volt_lu<(bat_adj_reading+50))
      {
         lowbat=1;
            
      }
      else 
      {   
         lowbat=0;
         bat_low_already_detected=0;
      }
   }
   else
   {
      if(bat_volt_lu<(bat_adj_reading))
      {
         lowbat=1;
         bat_low_already_detected=1;
      }
      else
      {
         lowbat=0;
         bat_low_already_detected=0;      
      }
   } 
   
   output_bit(RL_BATT, !lowbat);      output_bit(LED_BATT, lowbat);
  
   print_readings();
}


void print_readings(void)
{
   
   /////////// 1 and 2 for mains knob reading
   i2c_buffer[1] = (uint8_t)(phase_adjusted_volt&0x00ff);
   i2c_buffer[2] = (uint8_t)(phase_adjusted_volt>>8);   
   
   /////////// 3 and 4 bts battery knob reading
   i2c_buffer[3] = (uint8_t)(bat_adj_reading&0x00ff);
   i2c_buffer[4] = (uint8_t)((bat_adj_reading>>8)&0x00ff);
   
   /////////// 5 and 6 dg battery knob reading
   i2c_buffer[5] = 0;   //(uint8_t)(dgbat_adj_reading&0x00ff);
   i2c_buffer[6] = 0;   //(uint8_t)((dgbat_adj_reading>>8)&0x00ff);   
   
   /////////// 7 and 8 temperature knob reading
   i2c_buffer[7] = 0;   //(uint8_t)(temp_adj_reading&0x00ff);
   i2c_buffer[8] = 0;   //(uint8_t)((temp_adj_reading>>8)&0x00ff);   
   
   /////////// 9 and 10 bts battery reading
   i2c_buffer[9] = (uint8_t)(bat_volt_lu&0x00ff);
   i2c_buffer[10] = (uint8_t)((bat_volt_lu>>8)&0x00ff);   

   /////////// 11 and 12 dg battery reading
   i2c_buffer[11] = 0;   //(uint8_t)(dgbat_volt_lu&0x00ff);
   i2c_buffer[12] = 0;   //(uint8_t)((dgbat_volt_lu>>8)&0x00ff);   
   
   /////////// 13 and 14 temperature reading
   i2c_buffer[13] = 0;   //(uint8_t)(temperature&0x00ff);
   i2c_buffer[14] = 0;   //(uint8_t)((temperature>>8)&0x00ff);   
   
   /////////// 15 and 16 mains R reading
   i2c_buffer[15] = (uint8_t)(r_phase_volt&0x00ff);
   i2c_buffer[16] = (uint8_t)((r_phase_volt>>8)&0x00ff);   
   
   /////////// 17 and 18 Mains Y reading
   i2c_buffer[17] = (uint8_t)(y_phase_volt&0x00ff);
   i2c_buffer[18] = (uint8_t)((y_phase_volt>>8)&0x00ff);

   /////////// 19 and 20 Mains B reading
   i2c_buffer[19] = (uint8_t)(b_phase_volt&0x00ff);
   i2c_buffer[20] = (uint8_t)((b_phase_volt>>8)&0x00ff);
   
   /////////// 20 and 21 DG Phase reading
   i2c_buffer[21] = (uint8_t)(dg_volt&0x00ff);
   i2c_buffer[22] = (uint8_t)((dg_volt>>8)&0x00ff);   
   

}

