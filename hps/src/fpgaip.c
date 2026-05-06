/*
 * fpgaip.c
 *
 *  Created on: 31-Mar-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#include "fpgaip.h"

uint32_t cnt = 0;

bool swsw = false;


/* FPGA IP Block LEDIP Write Pin Function */
void LEDIP_WritePin(LEDIP_TypeDef* ledip, uint8_t led, uint8_t state){
	if(state)	ledip->output |= (1U << led);
	else		ledip->output &= ~(1U << led);

}

/* FPGA IP Block LEDIP Toggle Pin Function */
void LEDIP_TogglePin(LEDIP_TypeDef* ledip, uint8_t led){
	ledip->output ^= (1U << led);
}

/* FPGA IP Block SWITCHIP read Pin Function */
bool SWITCHIP_ReadPin(SWITCHIP_TypeDef* switchip, uint8_t Sswitch){
	return (switchip->input & (1UL << Sswitch));
}

/* FPGA IP Block BUTTONIP read Pin Function */
bool BUTTONIP_ReadPin(BUTTONIP_TypeDef* buttonip, uint8_t Sbutton){
	return (buttonip->input & (1UL << Sbutton));
}

/* FPGA IP Block HEXIP Write data */
void HEXIP_WriteData(HEXIP_TypeDef* hexip, uint8_t numberinHex){
	hexip->output = numberinHex;
}

/* FPGA IP Block HEXIP Print Text */
void HEXIP_Text(HexContent_Typedef content){

	if(content == WRITE_READY){
		HEX5IP->output = R; //R - 0x50
		HEX4IP->output = E; //E
		HEX3IP->output = A; //A
		HEX2IP->output = D; //D
		HEX1IP->output = Y; //Y
		HEXIP_ROT(HEX0IP); // HEX0
	}
	else if(content == WRITE_HOLD){
		HEX5IP->output = H; //H
		HEX4IP->output = O; //O
		HEX3IP->output = L; //L
		HEX2IP->output = D; //D
		HEX1IP->output = BLANK; //0
		HEX0IP->output = BLANK; //0
	}
	else if(content == WRITE_TDONE){
		HEX5IP->output = T; //T
		HEX4IP->output = HY; //-
		HEX3IP->output = D; //D
		HEX2IP->output = O; //O
		HEX1IP->output = N; //N
		HEX0IP->output = E; //E
	}
	else if(content == WRITE_RDONE){
		HEX5IP->output = R; //R
		HEX4IP->output = HY; //-
		HEX3IP->output = D; //D
		HEX2IP->output = O; //O
		HEX1IP->output = N; //N
		HEX0IP->output = E; //E
	}
	else if(content == WRITE_TST){
		HEX5IP->output = T; //T
		HEX4IP->output = HY; //-
		HEX3IP->output = S; //S
		HEX2IP->output = T; //T
		HEX1IP->output = BLANK; //0
		HEX0IP->output = BLANK; //0
	}
	else if(content == WRITE_FULL){
		HEX5IP->output = F; //F
		HEX4IP->output = U; //U
		HEX3IP->output = L; //L
		HEX2IP->output = L; //L
		HEX1IP->output = BLANK; //0
		HEX0IP->output = BLANK; //0
	}
	else if(content == WRITE_LO){
		HEX5IP->output = L; //L
		HEX4IP->output = O; //O
		HEX3IP->output = BLANK; //0
		HEX2IP->output = BLANK; //0
		HEX1IP->output = BLANK; //0
		HEX0IP->output = BLANK; //0
	}
	else if(content == WRITE_PLAY){
		HEX5IP->output = P; //P
		HEX4IP->output = L; //L
		HEX3IP->output = A; //A
		HEX2IP->output = Y; //Y
		HEX1IP->output = BLANK; //0
		HEX0IP->output = BLANK; //0
	}

}

/* FPGA IP Block HEXIP Reset Text to blank */
void HEXIP_Reset(HEXIP_TypeDef* hexip){
	hexip->output |= BLANK;
}

/* FPGA IP Block HEXIP Reset all ip blocks to blank */
void HEXIP_ResetALL(){
	HEX5IP->output = BLANK;
	HEX4IP->output = BLANK;
	HEX3IP->output = BLANK;
	HEX2IP->output = BLANK;
	HEX1IP->output = BLANK;
	HEX0IP->output = BLANK;
}

/* FPGA IP Block HEXIP Blink */
void HEXIP_Blink(HexContent_Typedef content, uint32_t speed){

	HEXIP_Text(content);

    delay(speed);

    HEXIP_ResetALL();

    delay(speed);
}

/* FPGA IP Block HEXIP Rotate the text */
void HEXIP_ROT(HEXIP_TypeDef* hexip){
	if(swsw == false){
		switch(cnt){
			case 0:
				hexip->output = 0x01;
				cnt++;
			break;
			case 1:
				hexip->output = 0x02;
				cnt++;
			break;
			case 2:
				hexip->output = 0x04;
				cnt++;
			break;
			case 3:
				hexip->output = 0x08;
				cnt++;
			break;
			case 4:
				hexip->output = 0x10;
				cnt++;
			break;
			case 5:
				hexip->output = 0x20;
				cnt++;
				swsw = true;
			break;
			default:
			cnt = 0;
			break;
		}

	}else{
		switch(cnt){
			case 0:
				hexip->output = 0x20;
				cnt++;
			break;
			case 1:
				hexip->output = 0x10;
				cnt++;
			break;
			case 2:
				hexip->output = 0x08;
				cnt++;
			break;
			case 3:
				hexip->output = 0x04;
				cnt++;
			break;
			case 4:
				hexip->output = 0x02;
				cnt++;
			break;
			case 5:
				hexip->output = 0x01;
				cnt++;
				swsw = false;
			break;
			default:
			cnt = 0;
			break;
		}

	}

}

/* FPGA IP Block CODECIP Clear the FIFO */
void CODECIP_CLEARFIFO(CODECIP_TypeDef* codec, uint8_t adc, uint8_t dac){

	codec->control |=  ((adc << 2U) | (dac << 3U));
	codec->control &= ~((adc << 2U) | (dac << 3U));

}

/* FPGA IP Block CODECIP Get the FIFO Space */
void CODECIP_GETFIFOSPACE(CODECIP_TypeDef* codec, uint32_t* fifo){

	uint32_t fill = codec->fifospace;
	uint32_t rspace = (fill >> 16) & 0xFFU;
	uint32_t lspace = (fill >> 24) & 0xFFU;

	*fifo = (rspace > lspace) ? lspace : rspace;

}

/* FPGA IP Block CODECIP Write Data */
void CODECIP_WRITE(CODECIP_TypeDef* codec, uint32_t left, uint32_t right){

	codec->leftdata = left;
	codec->rightdata = right;

}

/* Initalize the GPIOIP Block */
void GPIOIP_INIT(GPIOIP_TypeDef* gpio, uint32_t piotype,uint32_t dir, uint32_t port){

	gpio->int_mask  = 0x00000000UL;

	gpio->int_flags = 0xFFFFFFFFUL;

	if(piotype == GPIOBIDIR)	gpio->dirinp.direction = dir;

	if(piotype & GPIOOPDIR)		gpio->opip.output = port;

}

/* Set the GPIOIP Block Direction */
void GPIOIP_setDirection(GPIOIP_TypeDef* gpio, uint32_t val){

	gpio->dirinp.direction |= val;

}

/* Set the GPIOIP Block Set Output */
void GPIOIP_setOutput(GPIOIP_TypeDef* gpio, uint32_t val){

    uint32_t value = LT_CSN | LT_WRN | LT_RDN;

    if(val)	value |= LT_HP_OPT_ON;

    gpio->opip.output = (value & LT_PIO) | (gpio->opip.output & ~LT_PIO);

}
