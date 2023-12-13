/* 
 * File:   hal_i2c.c
 * Author: Aya Ali
 *
 * Created on December 11, 2023, 8:16 PM
 */
#include "hal_i2c.h"
static inline void MSSP_I2C_Mode_GPIO_CFG(void);
static inline void MSSP_I2C_Clock_CFG(const mssp_i2c_t *i2c_obj);
static inline void MSSP_I2C_Interrupt_Configuration(const mssp_i2c_t *i2c_obj);
static inline void MSSP_I2C_Slave_CFG(const mssp_i2c_t *i2c_obj);
#ifdef MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE    
static void(*I2c_Write_Collision_Interrupt_Hundeler)(void);
static void(*I2C_Defualt_Interrupt_Hundeler)(void);
static void(*I2c_Receive_Overflow_Interrupt_Hundeler)(void);
#endif    


Std_ReturnType MSSP_I2C_Init(const mssp_i2c_t *i2c_obj){
Std_ReturnType ret = E_NOT_OK;
if(i2c_obj == NULL){
ret = E_NOT_OK;
}
else{
 /*Disable MSSP module */
    MSSP_MODULE_DISABLE_CFG();
/*MSSP select mode*/
/*MSSP MASTER Mode Configuration*/    
if(i2c_obj->i2c_cfg.i2c_mode == MMSP_I2C_MASTER_MODE){
/*MSSP clock configuration*/
MSSP_I2C_Clock_CFG(i2c_obj);
}
/*MSSP Slave Mode Configuration*/
else if(i2c_obj->i2c_cfg.i2c_mode == MMSP_I2C_SLAVE_MODE){
/*General call configuration*/
    if(i2c_obj->i2c_cfg.i2c_general_call == I2C_GENERAL_CALL_ENABLE){
        I2C_GENERAL_CALL_ENABLE_CFG();
    }
    else if(i2c_obj->i2c_cfg.i2c_general_call == I2C_GENERAL_CALL_DISABLE){
        I2C_GENERAL_CALL_DISABLE_CFG();
    } 
    else{}  
   /*clear the write collision detected*/
   SSPCON1bits.WCOL = 0;
   /*Clear received overflow indicator*/
   SSPCON1bits.SSPOV = 0;
  /*Release the clock*/
   SSPCON1bits.CKP = 1;
   /*Assign the slave address */
   SSPADD = i2c_obj->i2c_cfg.i2c_slave_address;
   /*Slave configurations*/
   MSSP_I2C_Slave_CFG(i2c_obj);
}
else{}    
/*MSSP GPIO pin direction as input*/    
    MSSP_I2C_Mode_GPIO_CFG();
/*MSSP I2C Slaw rate control*/
    if(i2c_obj->i2c_cfg.i2c_slew_rate == I2C_SLAW_RATE_ENABLE){
        I2C_SLEW_RATE_ENABLE_CFG();
    }
    else if(i2c_obj->i2c_cfg.i2c_slew_rate == I2C_SLAW_RATE_DISABLE){
        I2C_SLEW_RATE_DISABLE_CFG();
    } 
    else{}   
/*MSSP I2C SMBUS control*/    
     if(i2c_obj->i2c_cfg.i2c_SMBus_Control == I2C_SMBUS_ENABLE){
        I2C_SMBUS_ENABLE_CFG();
    }
    else if(i2c_obj->i2c_cfg.i2c_SMBus_Control == I2C_SMBUS_DISABLE){
        I2C_SMBUS_DISABLE_CFG();
    } 
    else{}    
/*Interrupt configurations*/    
#ifdef MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    MSSP_I2C_Interrupt_Configuration(i2c_obj);
#endif    
/*Enable MSSP module */   
    MSSP_MODULE_ENABLE_CFG();
    ret = E_OK;

}
return ret;
}
Std_ReturnType MSSP_I2C_DeInit(const mssp_i2c_t *i2c_obj){
Std_ReturnType ret = E_NOT_OK;
if(i2c_obj == NULL){
ret = E_NOT_OK;
}
else{
    /*Disable MSSP module */
    MSSP_MODULE_DISABLE_CFG();
#ifdef MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE    
MSSP_I2C_INTERRUPT_DISABLE();
MSSP_I2C_BUS_COL_INTERRUPT_DISABLE();
#endif    
    ret = E_OK;
}
return ret;
}
Std_ReturnType MSSP_I2C_Master_Send_Start(const mssp_i2c_t *i2c_obj){
Std_ReturnType ret = E_NOT_OK;
if(i2c_obj == NULL){
ret = E_NOT_OK;
}
else{
   /*Initialize start condition*/
  SSPCON2bits.SEN = 1;
/*wait for the completion of the start condition*/  
  while(SSPCON2bits.SEN);
  /*clear MSSP Interrupt Flag*/
  PIR1bits.SSPIF = 0;
  if(SSPSTATbits.S == START_BIT_DETECTED){
    ret = E_OK;
  }
  else{
     ret = E_NOT_OK;
  }
}
return ret;
}
Std_ReturnType MSSP_I2C_Master_Send_Repeated_Start(const mssp_i2c_t *i2c_obj){
Std_ReturnType ret = E_NOT_OK;
if(i2c_obj == NULL){
ret = E_NOT_OK;
}
else{
 /*Initialize Repeated start condition*/
  SSPCON2bits.RSEN = 1;
/*wait for the completion of the stop condition*/  
  while(SSPCON2bits.RSEN);
  /*clear MSSP Interrupt Flag*/
  PIR1bits.SSPIF = 0;
    ret = E_OK;
}
return ret;
}
Std_ReturnType MSSP_I2C_Master_Send_Stop(const mssp_i2c_t *i2c_obj){
Std_ReturnType ret = E_NOT_OK;
if(i2c_obj == NULL){
ret = E_NOT_OK;
}
else{
 /*Initialize stop condition*/
  SSPCON2bits.PEN = 1;
/*wait for the completion of the stop condition*/  
  while(SSPCON2bits.PEN);
  /*clear MSSP Interrupt Flag*/
  PIR1bits.SSPIF = 0;
  if(SSPSTATbits.P == STOP_BIT_DETECTED){
    ret = E_OK;
  }
  else{
     ret = E_NOT_OK;
  }
    
}
return ret;
}
Std_ReturnType MSSP_I2C_Master_Write_Blocking(const mssp_i2c_t *i2c_obj, uint_8 i2c_data, uint_8 *ack){
Std_ReturnType ret = E_NOT_OK;
if(i2c_obj == NULL){
ret = E_NOT_OK;
}
else{
/*write data to the data register*/    
    SSPBUF = i2c_data;
/*wait for transmite to complete*/    
    while(!PIR1bits.SSPIF);
/*clear MSSP Interrupt Flag*/
  PIR1bits.SSPIF = 0;    
if(SSPCON2bits.ACKSTAT == I2C_ACK_RECEVED_FROM_SLAVE){
    *ack = I2C_ACK_RECEVED_FROM_SLAVE;
}
else{
 *ack = I2C_ACK_NOT_RECEVED_FROM_SLAVE;
}
  ret = E_OK;
}
return ret;
}
Std_ReturnType MSSP_I2C_Master_Read_Blocking(const mssp_i2c_t *i2c_obj, uint_8 ack ,uint_8 *i2c_data){
Std_ReturnType ret = E_NOT_OK;
if((i2c_obj == NULL) || (i2c_data == NULL)){
ret = E_NOT_OK;
}
else{
   /*Enable receiving from slave*/ 
    I2C_MASTER_RECEIVE_ENABLE_CFG();
  /*wait for buffer full flag*/
    while(!SSPSTATbits.BF);
  /*copy data register for buffer variable*/
    *i2c_data = SSPBUF;
 /*send ACK or NACK after read*/
    if(ack == I2C_MASTER_SEND_ACK){
        SSPCON2bits.ACKDT = 0;
        SSPCON2bits.ACKEN = 1;
    }else if(ack == I2C_MASTER_SEND_NACK){
        SSPCON2bits.ACKDT = 1;
        SSPCON2bits.ACKEN = 1;    
    }
    else{}
    ret = E_OK;
}
return ret;
}

void MSSP_I2C_ISR(void){
#ifdef MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE      
MSSP_I2C_CLEAR_FLAG();
if(I2C_Defualt_Interrupt_Hundeler){
I2C_Defualt_Interrupt_Hundeler();
}
#endif
}

void MSSP_I2C_BC_ISR(void){
#ifdef MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE      
MSSP_I2C_BUS_COL_CLEAR_FLAG();
if(I2c_Write_Collision_Interrupt_Hundeler){
   I2c_Write_Collision_Interrupt_Hundeler();
}
#endif
}

static inline void MSSP_I2C_Mode_GPIO_CFG(void){
    TRISCbits.TRISC3 = 1;    /*SCL pin is Input*/
    TRISCbits.TRISC4 = 1;    /*SDA pin is Input*/
}

static inline void MSSP_I2C_Clock_CFG(const mssp_i2c_t *i2c_obj){
    SSPCON1bits.SSPM = i2c_obj->i2c_cfg.i2c_mode_cfg;
    SSPADD = (uint_8)(((_XTAL_FREQ/ 4.0)/i2c_obj->i2c_clock) - 1);
}
static inline void MSSP_I2C_Slave_CFG(const mssp_i2c_t *i2c_obj){
    SSPCON1bits.SSPM = i2c_obj->i2c_cfg.i2c_mode_cfg;
}
static inline void MSSP_I2C_Interrupt_Configuration(const mssp_i2c_t *i2c_obj){
#ifdef MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    MSSP_I2C_INTERRUPT_ENABLE();
    MSSP_I2C_BUS_COL_INTERRUPT_ENABLE();
    MSSP_I2C_CLEAR_FLAG();
    MSSP_I2C_BUS_COL_CLEAR_FLAG();
   I2c_Write_Collision_Interrupt_Hundeler = i2c_obj->I2c_Write_Collision;
   I2C_Defualt_Interrupt_Hundeler = i2c_obj->I2C_Defualt_Interrupt_Hundeler;
   I2c_Receive_Overflow_Interrupt_Hundeler = i2c_obj->I2c_Receive_Overflow;
#ifdef INTERRUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
    INTERRUPT_PRIORITYLEVEL_ENABLE();
    if((i2c_obj->i2c_cfg.MSSP_I2C_Interrupt_Priority == Interrupt_Periority_HIGH)){
        INTERRUPT_GLOBALEINTERRUPTHIGH_ENABLE();   
        MSSP_I2C__HIGH_PRIORITY_SET();
    }
    else if((i2c_obj->i2c_cfg.MSSP_I2C_Interrupt_Priority == Interrupt_Periority_LOW)){
          INTERRUPT_GLOBALEINTERRUPTLOW_ENABLE();
           MSSP_I2C_LOW_PRIORITY_SET();
           MSSP_I2C_BUS_COL_LOW_PRIORITY_SET();
    }
    else{}
   if((i2c_obj->i2c_cfg.MSSP_I2C_BC_Interrupt_Priority == Interrupt_Periority_HIGH)){
        INTERRUPT_GLOBALEINTERRUPTHIGH_ENABLE();   
        MSSP_I2C_BUS_COL_HIGH_PRIORITY_SET();
    } 
     else if((i2c_obj->i2c_cfg.MSSP_I2C_BC_Interrupt_Priority == Interrupt_Periority_LOW)){
          INTERRUPT_GLOBALEINTERRUPTLOW_ENABLE();
           MSSP_I2C_BUS_COL_LOW_PRIORITY_SET();
    }
    else{}
#else
  INTERRUPT_GLOBALEINTERRUPT_ENABLE();
  INTERRUPT_PERIPHERALINTERRUPT_ENABLE();    
#endif
#endif    

}