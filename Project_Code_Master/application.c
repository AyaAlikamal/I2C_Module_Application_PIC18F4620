/* 
 * File:   application.c
 * Author: AyaAli
 * Created on September 22, 2023, 10:24 AM
 */
#include "application.h"

mssp_i2c_t i2c_obj;
uint_8 slave_ack;
uint_8 App_counter;
#define SLAVE_1 0x60
#define SLAVE_2 0x62
void MSSP_I2C_Send_1_Byte(uint_8 slave_add, uint_8 data){
Std_ReturnType ret = E_OK;     
ret = MSSP_I2C_Master_Send_Start(&i2c_obj);
ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, slave_add ,&slave_ack);
ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, data ,&slave_ack);
ret = MSSP_I2C_Master_Send_Stop(&i2c_obj);
}
void I2C_init(){
Std_ReturnType ret = E_OK;    
   
//   i2c_obj.I2C_Defualt_Interrupt_Hundeler = NULL;
//   i2c_obj.I2c_Receive_Overflow = NULL;
//   i2c_obj.I2c_Write_Collision = NULL;
   i2c_obj.i2c_clock = 100000;
   i2c_obj.i2c_cfg.i2c_mode = MMSP_I2C_MASTER_MODE;
   i2c_obj.i2c_cfg.i2c_mode_cfg = I2C_MASTER_MODE_DEFINED_CLOCK;
   i2c_obj.i2c_cfg.i2c_SMBus_Control = I2C_SMBUS_DISABLE;
   i2c_obj.i2c_cfg.i2c_slew_rate = I2C_SLAW_RATE_DISABLE;
   ret = MSSP_I2C_Init(&i2c_obj);  
};

int main() {
Std_ReturnType ret = E_NOT_OK;
I2C_init();

while(1){
MSSP_I2C_Send_1_Byte(SLAVE_1, 'a');
__delay_ms(1000);
MSSP_I2C_Send_1_Byte(SLAVE_2, 'b');
__delay_ms(1000);
MSSP_I2C_Send_1_Byte(SLAVE_1, 'c');
__delay_ms(1000);
MSSP_I2C_Send_1_Byte(SLAVE_2, 'd');
__delay_ms(1000);
App_counter++;
}
 return (EXIT_SUCCESS);
}
void application_initialize(void){
   Std_ReturnType ret = E_NOT_OK;
}
