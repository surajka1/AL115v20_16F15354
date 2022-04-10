#separate
void init_settings(void)
{
   read_rom_values();
   rvoltratio=((float)calibvolt)/((float)rcalibvoltreading);
   yvoltratio=((float)calibvolt)/((float)ycalibvoltreading);
   bvoltratio=((float)calibvolt)/((float)bcalibvoltreading);
   dgrvoltratio = ((float)calibvolt)/((float)dgrcalibvoltreading);   
   
   btsbatvoltratio=((float)btsbatcalibvolt)/((float)btsbatcalibvoltreading);
   
   v_adj_ratio = 60.0/((float)v_range_high);
   b_adj_ratio = 60.0/((float)b_range_high);

          
}


void get_ac_readings()
{
   MAINS_R_SELECT();
   rcalibvoltreading=phasecheck();
   
   MAINS_Y_SELECT();
   ycalibvoltreading=phasecheck();
   
   MAINS_B_SELECT();
   bcalibvoltreading=phasecheck();
   
   DG_SELECT();
   dgrcalibvoltreading=phasecheck();
}


#separate
uint16_t CalculatePhaseValue(uint16_t phase_reading, uint16_t input_value)
{
   phase_reading = (uint16_t)((float)(phase_reading)*((float)(calibvolt)/(float)(input_value)));
   return(phase_reading);
}

void ProcessMasterData(void)
{

   uint16_t temp_value, temp_value2;
   
//////////////////////// Mains R calibration      ////////////////////////////////

   if((i2c_buffer[23] != 0) || (i2c_buffer[24] != 0))   {        //mains r calibration
   
      temp_value = make16(i2c_buffer[24], i2c_buffer[23]) ;
      
      MAINS_R_SELECT();
      rcalibvoltreading=phasecheck();
      rcalibvoltreading = CalculatePhaseValue(rcalibvoltreading, temp_value);
      write_int16_alarmrom(2, rcalibvoltreading);
      i2c_buffer[23] = 0;   i2c_buffer[24] = 0;
      
   } else {}
   
//////////////////////// Mains Y calibration      ////////////////////////////////

   if((i2c_buffer[26] != 0) || (i2c_buffer[25] != 0))   {        //mains y calibration
   
      temp_value = make16(i2c_buffer[26], i2c_buffer[25]) ;
      
      MAINS_Y_SELECT();
      ycalibvoltreading=phasecheck();
      ycalibvoltreading = CalculatePhaseValue(ycalibvoltreading, temp_value);
      write_int16_alarmrom(4, ycalibvoltreading);
      i2c_buffer[26] = 0;   i2c_buffer[25] = 0;
      
   } else {}
   
//////////////////////// Mains B calibration      ////////////////////////////////

   if((i2c_buffer[28] != 0) || (i2c_buffer[27] != 0))   {        //mains b calibration
   
      temp_value = make16(i2c_buffer[28], i2c_buffer[27]) ;
      
      MAINS_B_SELECT();
      bcalibvoltreading=phasecheck();
      bcalibvoltreading = CalculatePhaseValue(bcalibvoltreading, temp_value);
      write_int16_alarmrom(6, bcalibvoltreading);
      i2c_buffer[28] = 0;   i2c_buffer[27] = 0;
      
   } else {}   
   
/////////////////////////// DGR calibration      ////////////////////////////////

   if((i2c_buffer[29] != 0) || (i2c_buffer[30] != 0))   {        //dgr calibration
   
      temp_value = make16(i2c_buffer[30], i2c_buffer[29]) ;
      
      DG_SELECT();
      dgrcalibvoltreading=phasecheck();
      dgrcalibvoltreading = CalculatePhaseValue(dgrcalibvoltreading, temp_value);
      write_int16_alarmrom(8, dgrcalibvoltreading);
      i2c_buffer[29] = 0;   i2c_buffer[30] = 0;
      
   } else {}      
   

///////////////////////// bts battery calibraiton //////////////////////////////
   
   if((i2c_buffer[40] != 0) || (i2c_buffer[39] != 0))   {        //bts calibration  
   
      temp_value = make16(i2c_buffer[40], i2c_buffer[39]) ;
      
      if((temp_value<1000)||(temp_value>9000))
      {
         i2c_buffer[64] = 1;         //bts battery calibration input value error
      }   else {}
      BATT_SELECT();
      temp_value2=phasecheck3();
      if(temp_value2<50||temp_value>700)
      {
         i2c_buffer[64] = 2;         //bts battery calibration input voltage error
      }

      write_int16_alarmrom(12, temp_value);
      write_int16_alarmrom(14, temp_value2);
      
      i2c_buffer[63] = 1;            //bts battery calibration successful
      read_rom_values();   
      i2c_buffer[40] = 0;i2c_buffer[39] = 0;
      
   } else {}


////////////////////////// knob mains uv calibration ///////////////////////////

   if(i2c_buffer[32] == TRUE)   { 
   
          VOLT_ADJ_SELECT();
          write_int16_alarmrom(20, phasecheck());   //mains knob
          i2c_buffer[32] = 0;
          read_rom_values(); 

   }

////////////////////////// knob bts battery calibration ////////////////////////

   if(i2c_buffer[36] == TRUE)   { 
   
            BAT_ADJ_SELECT();
            write_int16_alarmrom(22, phasecheck());   //bts knob
            i2c_buffer[36] = 0;
            read_rom_values(); 

   }


////////////////////////// All Knob decalibration  /////////////////////////////

   if(i2c_buffer[31] == TRUE)   { 

            ResetRomKnob();
            i2c_buffer[31] = 0;
            read_rom_values(); 

   }   
   
/////////////////////// All Battery decalibration  /////////////////////////////

   if(i2c_buffer[58] == TRUE)   { 

            ResetRomBattery();
            i2c_buffer[58] = 0;
            read_rom_values(); 

   }   

/////////////////////// All Phases decalibration  /////////////////////////////

   if(i2c_buffer[57] == TRUE)   { 

            ResetRomPhase();
            i2c_buffer[57] = 0;
            read_rom_values(); 

   }

////////////////////////////////////////////////////////////////////////////////
   //read_rom_values();
   init_settings();
   i2c_buffer[62] = 0;
   
}


