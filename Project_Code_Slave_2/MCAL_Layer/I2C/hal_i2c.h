/* 
 * File:   hal_i2c.h
 * Author: Aya Ali
 *
 * Created on December 11, 2023, 8:16 PM
 */

#ifndef HAL_I2C_H
#define	HAL_I2C_H
/*Section : Includes*/
#include "../../../../Embeded-Compiler/pic/include/proc/pic18f4620.h"
#include "../GPIO/hal_gpio.h"
#include "../interrupt/mcal_internal_interrupt.h"
#include "../mcal_std_type.h"
#include "../device_config.h"

/*Section : Macro Declaration*/
#define I2C_SLAW_RATE_ENABLE    0
#define I2C_SLAW_RATE_DISABLE   1

#define I2C_SMBUS_ENABLE        1
#define I2C_SMBUS_DISABLE       0

#define I2C_LAST_RT_BYTE_DATA      1
#define I2C_LAST-RT_BYTE_ADDRESS   0

#define STOP_BIT_DETECTED       1
#define STOP_BIT_NOT_DETECTED   0

#define START_BIT_DETECTED       1
#define START_BIT_NOT_DETECTED   0

#define MMSP_I2C_MASTER_MODE     1
#define MMSP_I2C_SLAVE_MODE      0

#define I2C_SLAVE_MODE_7BIT_ADDRESS      0x06U
#define I2C_SLAVE_MODE_10BIT_ADDRESS     0x07U
#define I2C_SLAVE_MODE__7BIT_ADDRESS_SS_INT_ENABLE   0x0EU
#define I2C_SLAVE_MODE__10BIT_ADDRESS_SS_INT_ENABLE  0x0FU
#define I2C_MASTER_MODE_DEFINED_CLOCK                0x08U
#define I2C_MASTER_MODE_FIREWARE_CONTROLLED          0x0BU

#define I2C_GENERAL_CALL_ENABLE       1
#define I2C_GENERAL_CALL_DISABLE      0

#define I2C_MASTER_RECEIVE_ENABLE     1
#define I2C_MASTER_RECEIVE_DISABLE    0

#define I2C_MASTER_SEND_ACK     0
#define I2C_MASTER_SEND_NACK    1

#define I2C_ACK_RECEVED_FROM_SLAVE    0
#define I2C_ACK_NOT_RECEVED_FROM_SLAVE    1
/*Section : Macro Function Declaration*/
#define I2C_SLEW_RATE_DISABLE_CFG()         (SSPSTATbits.SMP = 1)
#define I2C_SLEW_RATE_ENABLE_CFG()          (SSPSTATbits.SMP = 0)

#define I2C_SMBUS_DISABLE_CFG()             (SSPSTATbits.CKE = 0)
#define I2C_SMBUS_ENABLE_CFG()              (SSPSTATbits.CKE = 1)

#define I2C_GENERAL_CALL_DISABLE_CFG()     (SSPCON2bits.GCEN = 0) 
#define I2C_GENERAL_CALL_ENABLE_CFG()      (SSPCON2bits.GCEN = 1) 

#define I2C_MASTER_RECEIVE_DISABLE_CFG()    (SSPCON2bits.RCEN = 0)
#define I2C_MASTER_RECEIVE_ENABLE_CFG()     (SSPCON2bits.RCEN = 1)

#define MSSP_MODULE_ENABLE_CFG()            (SSPCON1bits.SSPEN = 1)
#define MSSP_MODULE_DISABLE_CFG()           (SSPCON1bits.SSPEN = 0)

#define I2C_CLOCK_STRETCH_ENABLE()           (SSPCON1bits.CKP = 0)
#define I2C_CLOCK_RELEASE_DISABLE()          (SSPCON1bits.CKP = 1)
/*Section : Data Type Declaration*/
typedef struct {
uint_8 i2c_mode_cfg;
uint_8 i2c_slave_address;
uint_8 i2c_mode:1;
uint_8 i2c_slew_rate:1;
uint_8 i2c_SMBus_Control:1;
uint_8 i2c_general_call:1;
uint_8 i2c_master_rec_mode:1;
uint_8 i2c_reseved:3;
#ifdef MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE   
    Interrupt_Periority_cfg MSSP_I2C_Interrupt_Priority;
    Interrupt_Periority_cfg MSSP_I2C_BC_Interrupt_Priority;
#endif    
}i2c_configur_t;

typedef struct {
    uint_32 i2c_clock;
#ifdef MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE    
    void(*I2c_Write_Collision)(void);
    void(*I2C_Defualt_Interrupt_Hundeler)(void);
    void(*I2c_Receive_Overflow)(void);
#endif    

    i2c_configur_t i2c_cfg;

    
}mssp_i2c_t;

/*Section : Function Declaration*/
Std_ReturnType MSSP_I2C_Init(const mssp_i2c_t *i2c_obj);
Std_ReturnType MSSP_I2C_DeInit(const mssp_i2c_t *i2c_obj);
Std_ReturnType MSSP_I2C_Master_Send_Start(const mssp_i2c_t *i2c_obj);
Std_ReturnType MSSP_I2C_Master_Send_Repeated_Start(const mssp_i2c_t *i2c_obj);
Std_ReturnType MSSP_I2C_Master_Send_Stop(const mssp_i2c_t *i2c_obj);
Std_ReturnType MSSP_I2C_Master_Write_Blocking(const mssp_i2c_t *i2c_obj, uint_8 i2c_data, uint_8 *ack);
Std_ReturnType MSSP_I2C_Master_Read_Blocking(const mssp_i2c_t *i2c_obj, uint_8 ack ,uint_8 *i2c_data);
#endif	/* HAL_I2C_H */

