/*
 * wm8731.c
 *
 *  Created on: 24-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#include "wm8731.h"

/* WM8731 Init */
Status_TypeDef WM8731_INIT(I2C_HandleTypeDef* i2c){

	if(i2c == NULL	) return ARM_ERROR;

	WM8731_CODEC_WRITE(i2c, CODEC_PWRCTRL,  	0x12); // Seting the Microphone input and OUTPOD
	WM8731_CODEC_WRITE(i2c, CODEC_LFINCTRL,  	0x17); // Setting the left channel input volume to +4.5dB + unmute[7]
	WM8731_CODEC_WRITE(i2c, CODEC_RTINCTRL,  	0x17); // Setting the right channel input volume to -24dB + unmute[7]
	WM8731_CODEC_WRITE(i2c, CODEC_LTOUTCTRL,    0x70); // Setting the left channel output volume to -24dB + 0000000 to 0101111 = MUTE
	WM8731_CODEC_WRITE(i2c, CODEC_RTOUTCTRL,    0x70); // Setting the right channel output volume to -24dB + 0000000 to 0101111 = MUTE
	WM8731_CODEC_WRITE(i2c, CODEC_ANPACTRL,   	0x12); // Setting MUTEMIC + DACSEL
	WM8731_CODEC_WRITE(i2c, CODEC_DGPACTRL,   	0x06); // Setting DEEMP + ADCHPD
	WM8731_CODEC_WRITE(i2c, CODEC_DATAFMCTRL,   0x4E); // Setting FORMAT - 10(I2S), IWL - 00(16 Bits), MS - Enable
	WM8731_CODEC_WRITE(i2c, CODEC_SMPLMCTRL,    0x00); // Setting FORMAT - Leave it to default and set to '0'
	WM8731_CODEC_WRITE(i2c, CODEC_ACTICTRL,     0x01); // Setting FORMAT Enable the codec
	WM8731_CODEC_WRITE(i2c, CODEC_PWRCTRL,      0x02); // Setting the power output

	return ARM_OK;

}

/* WM8731 Write Data to CODEC */
void WM8731_CODEC_WRITE(I2C_HandleTypeDef* i2c, uint8_t reg, uint16_t val){

    uint16_t cdata;
    uint8_t tx[2];

    cdata = (uint16_t)(((reg & 0x7FU) << 9U) | (val & 0x1FFU));

    tx[0] = (uint8_t)((cdata >> 8) & 0xFFU);
    tx[1] = (uint8_t)(cdata & 0xFFU);

    i2c->Instance->tar = CODEC_ID;
    I2C_WriteMultiBytes(i2c, tx, 2);
}

/* Fucntion to reverse the data for 16 bits */
uint16_t reverse16Data(uint16_t data){
	return ((data & 0xFF00U) >> 8) | ((data & 0x00FFU) << 8);
}
