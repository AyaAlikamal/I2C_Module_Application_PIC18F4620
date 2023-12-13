/* 
 * File:   application.c
 * Author: AyaAli
 * Created on September 22, 2023, 10:24 AM
 */
#include "application.h"

mssp_i2c_t i2c_obj;
static volatile uint_8 slave2_data ;
led_t led1 = {.Port_Name = PORTD_INDEX, .pin = PIN0, .led_statues = LED_OFF};

void MSSP_I2C_Defualt_Interrupt_Hundeler(){
/*Clock Stretch*/    
I2C_CLOCK_STRETCH_ENABLE();
if((SSPSTATbits.R_nW == 0) && (SSPSTATbits.D_nA == 0)){
    uint_8 dummy_buffer = SSPBUF;
    while(!SSPSTATbits.BF);
    slave2_data = SSPBUF;
}
else if((SSPSTATbits.R_nW == 1)){

}
else{}
/*Clock Release*/
I2C_CLOCK_RELEASE_DISABLE(); 
}
void I2C_init(){
Std_ReturnType ret = E_OK;    
   
   i2c_obj.I2C_Defualt_Interrupt_Hundeler = MSSP_I2C_Defualt_Interrupt_Hundeler;
   i2c_obj.I2c_Receive_Overflow = NULL;
   i2c_obj.I2c_Write_Collision = NULL;
   i2c_obj.i2c_cfg.i2c_mode = MMSP_I2C_SLAVE_MODE;
   i2c_obj.i2c_cfg.i2c_mode_cfg = I2C_SLAVE_MODE_7BIT_ADDRESS;
   i2c_obj.i2c_cfg.i2c_SMBus_Control = I2C_SMBUS_DISABLE;
   i2c_obj.i2c_cfg.i2c_slew_rate = I2C_SLAW_RATE_DISABLE;
   i2c_obj.i2c_cfg.i2c_slave_address = 0x62;
   i2c_obj.i2c_cfg.i2c_general_call = I2C_GENERAL_CALL_DISABLE;
   ret = MSSP_I2C_Init(&i2c_obj);  
};

int main() {
Std_ReturnType ret = E_NOT_OK;
I2C_init();
ret = led_initialize(&led1);
while(1){
    if(slave2_data == 'b'){
        ret = led_toggle(&led1);
    }
    else if(slave2_data == 'd'){
        ret = led_turn_off(&led1);
    } 
}
 return (EXIT_SUCCESS);
}
void application_initialize(void){
   Std_ReturnType ret = E_NOT_OK;
}
