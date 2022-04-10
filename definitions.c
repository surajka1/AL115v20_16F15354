

/***********************Analog Inputs******************************************/

//The channels are different for PIC16F15354

#define DG_SELECT()         set_adc_channel(5)

#define BATT_SELECT()      set_adc_channel(0)

//#define DGBATT_SELECT()   set_adc_channel(11)

//#define TEMP_SELECT()      set_adc_channel(4)

#ifdef AL115v10
    #define VOLT_ADJ_SELECT()   set_adc_channel(12)
    #define BAT_ADJ_SELECT()   set_adc_channel(13)
    #define MAINS_R_SELECT()   set_adc_channel(2)
    #define MAINS_Y_SELECT()   set_adc_channel(3)
    #define MAINS_B_SELECT()   set_adc_channel(1)
#else
    #define VOLT_ADJ_SELECT()   set_adc_channel(13)
    #define BAT_ADJ_SELECT()   set_adc_channel(12)
    #define MAINS_R_SELECT()   set_adc_channel(3)
    #define MAINS_Y_SELECT()   set_adc_channel(1)
    #define MAINS_B_SELECT()   set_adc_channel(2)
#endif
//#define DGBAT_ADJ_SELECT() set_adc_channel(10)

//#define TEMP_ADJ_SELECT()   set_adc_channel(9)

/***********************Digital Inputs*****************************************/

//#define FIRE_INPUT()   input(PIN_B6)

//#define LFL_INPUT()      !input(PIN_E3)

/***********************Outputs************************************************/

#define RL_MAINS     PIN_C2

#define RL_DG        PIN_C7

#define RL_BATT      PIN_C6

#define LED_BATT   PIN_C5

#ifdef AL115v10
    #define LED_R  PIN_B0
    #define LED_Y  PIN_B1
    #define LED_B  PIN_B2
#else
    #define LED_R  PIN_B2
    #define LED_Y  PIN_B1
    #define LED_B  PIN_B0
#endif
/******************************************************************************/

#define DELAY 1000

/******************************************************************************/

//!#define CR   0x0D
//!#define LF   0x0A
//!#define BS   0x08
//!#define ESC  0x1B
//!#define SP   0x20
//!#define DEL  0x7F

/******************************************************************************/


/******************************************************************************/

uint8_t i2c_buffer[66];

/******************************* Variables ************************************/

uint16_t bat_volt_lu=0;// dgbat_volt_lu=0; //temperature;

uint16_t r_phase_volt=0, y_phase_volt=0, b_phase_volt=0;
uint16_t dg_volt=0;
//float bat_volt=0;

uint16_t volt_adj_reading=0;


//19 reading for every volt;
uint16_t bat_adj_reading=209;// dgbat_adj_reading=209, temp_adj_reading=0;


short lowbat=0, bat_low_already_detected=0;
//short dgbat_low_already_detected=0, dglowbat =0;
//short temp_high_already_detected =0, hightemp =0;
uint16_t phase_adjusted_volt;
short mains_ok=1;
//short dgon = 0;
short r_ok = 0, y_ok =0, b_ok =0;



uint16_t rcalibvoltreading=220, ycalibvoltreading   =   220;
uint16_t bcalibvoltreading=220, dgrcalibvoltreading=   220;
float rvoltratio=1.01, yvoltratio=1.01,bvoltratio=1.01, dgrvoltratio=1.01;
//unsigned long mainscalibvolt=220;
uint16_t calibvolt=220;

float btsbatvoltratio=9.7;
//float dgbatvoltratio=11.1;
//float tempratio = 10;

uint16_t btsbatcalibvoltreading=441;
uint16_t btsbatcalibvolt = 4800;


//uint16_t dgbatcalibvoltreading=539;
//uint16_t dgbatcalibvolt = 24;

float v_adj_ratio = .093, b_adj_ratio =.186;
//float db_adj_ratio = .093, t_adj_ratio =.186;

uint16_t v_range_high=0, b_range_high=0;// db_range_high=0, t_range_high =0;

//////////////for introducing alarm trigger delay


//////////for fire delay/////////
//short fire_previous = 0, fire_current =0;


/************************ Function Definitions ********************************/

void clrscr(void);

#separate
void settings(void);

#separate
char timed_getc(void);


void reset_rom(void);

#separate
uint16_t UART_InUDec(void);

#separate
void write_calib_data(void);
#separate
void get_ac_readings(void);
#separate
void init_settings(void);

#separate
void read_rom_values(void);
#separate
void print_readings(void);

volatile uint16_t bat_row_an;