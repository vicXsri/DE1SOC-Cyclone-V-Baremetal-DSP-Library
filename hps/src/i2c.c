/*
 * i2c.c
 *
 *  Created on: 07-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#include "i2c.h"

/* Initalize the I2C Pheripheral */
Status_TypeDef I2C_Init(I2C_HandleTypeDef* i2c){

	if(i2c == NULL) return ARM_ERROR;

	I2C_MspInit(i2c);

	i2c->Instance->enable = I2C_DISABLE;

    if(i2c->Init.Mode  == I2C_MODEMASTER){
    	if(SWITCHIP_ReadPin(SWITCHIP, SW_PIN_4) && !SWITCHIP_ReadPin(SWITCHIP, SW_PIN_3))
    		i2c->Instance->con = (1UL << 0)| (1UL << 1U) |(1UL << 5U) | I2C_SLAVEDISABLE ; //0x65 -> 01100101 -> 0, 2, 5, 6
    	else
    		i2c->Instance->con = i2c->Init.Mode | i2c->Init.Speed | i2c->Init.AddressMode |i2c->Init.MasterRestart | I2C_SLAVEDISABLE ; //0x65 -> 01100101 -> 0, 2, 5, 6  -> for the gyro !!!
    }
    else{
		i2c->Instance->con = i2c->Init.Mode | i2c->Init.Speed | i2c->Init.AddressMode;
    }

    i2c->Instance->tar = i2c->Init.SlaveAddress;

    if(i2c->Init.Speed == I2C_SPEED400){
		i2c->Instance->fs_scl_hcnt = 60 + 30;
		i2c->Instance->fs_scl_lcnt = 130 + 30;
    }else{
    	// Not sure about the values, will update this soon
    	if(SWITCHIP_ReadPin(SWITCHIP, SW_PIN_4) && !SWITCHIP_ReadPin(SWITCHIP, SW_PIN_3)){
			i2c->Instance->ss_scl_hcnt = 0x190; // 492
			i2c->Instance->ss_scl_lcnt = 0x1D6; // 500
    	}else{
			i2c->Instance->ss_scl_hcnt = 492; // 492
			i2c->Instance->ss_scl_lcnt = 500; // 500
    	}
    }

    i2c->Instance->enable = I2C_ENABLE;

    return ARM_OK;
}

/* I2C Pheripheral write a byte */
void I2C_WriteByte(I2C_HandleTypeDef* i2c, uint8_t address, uint8_t value){

	/* Set the Address along with the restart bit */
	i2c->Instance->data_cmd = address | I2C_RESTART;
	/* Set the value */
	i2c->Instance->data_cmd = value;

}

/* I2C Pheripheral write multiple bytes */
Status_TypeDef I2C_WriteMultiBytes(I2C_HandleTypeDef* i2c, const uint8_t *data, uint32_t len){

    if(i2c == NULL || data == NULL || len == 0U) return ARM_ERROR;

    while(i2c->Instance->status & (1UL << 5U)) {}

    for(uint32_t i = 0; i < len; i++){

        while(i2c->Instance->txflr >= 16U) {}

        uint32_t datcmd = data[i];

        if(i == (len - 1U)){
            datcmd |= (1UL << 9U);
        }

        i2c->Instance->data_cmd = datcmd;
    }

    while(i2c->Instance->status & (1UL << 5U)) {}

    return ARM_OK;
}

/* I2C Pheripheral read a byte */
void I2C_ReadByte(I2C_HandleTypeDef* i2c, uint8_t address, uint8_t *value){

	/* Set the Address along with the restart bit */
	i2c->Instance->data_cmd = address | I2C_RESTART;

	/* Set the mode to Read */
	i2c->Instance->data_cmd = I2C_READ;

	/* Wait until u recieve the data */
	while(i2c->Instance->rxflr == 0){}

	/* Read the Value */
	*value = i2c->Instance->data_cmd;

}

/* I2C Pheripheral read multiple bytes */
void I2C_ReadMultiBytes(I2C_HandleTypeDef* i2c, uint8_t address, uint8_t value[], uint8_t len){

	uint32_t i=0;
	uint32_t nbyte=0;

	/* Set the Address along with the restart bit */
	i2c->Instance->data_cmd = address | I2C_RESTART;

	for(i=0; i < len; i++)	i2c->Instance->data_cmd = I2C_READ;


	while(len){
		if((i2c->Instance->rxflr) > 0)	{
			value[nbyte] = i2c->Instance->data_cmd;
			nbyte++;
			len--;
		}
	}
}
