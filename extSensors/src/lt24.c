/*
 * lt24.c
 *
 *  Created on: 27-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */


#include "lt24.h"

unsigned short LT24_Init [][2] = {
    {false, 0x00EF},
    {true , 0x0003},
    {true , 0x0080},
    {true , 0x0002},
    {false, 0x00CF},
    {true , 0x0000},
    {true , 0x0081},
    {true , 0x00C0},
    {false, 0x00ED},
    {true , 0x0064},
    {true , 0x0003},
    {true , 0x0012},
    {true , 0x0081},
    {false, 0x00E8},
    {true , 0x0085},
    {true , 0x0001},
    {true , 0x0078},
    {false, 0x00CB},
    {true , 0x0039},
    {true , 0x002C},
    {true , 0x0000},
    {true , 0x0034},
    {true , 0x0002},
    {false, 0x00F7},
    {true , 0x0020},
    {false, 0x00EA},
    {true , 0x0000},
    {true , 0x0000},
    {false, 0x00C0},
    {true , 0x0023},
    {false, 0x00C1},
    {true , 0x0010},
    {false, 0x00C5},
    {true , 0x003E},
    {true , 0x0028},
    {false, 0x00C7},
    {true , 0x0086},
    {false, 0x0036},
    {true , 0x0048},
    {false, 0x003A},
    {true , 0x0055},
    {false, 0x00B1},
    {true , 0x0000},
    {true , 0x001B},
    {false, 0x00B6},
    {true , 0x0008},
    {true , 0x0082},
    {true , 0x0027},
    {false, 0x00F2},
    {true , 0x0000},
    {false, 0x0026},
    {true , 0x0001},
    {false, 0x00E0},
    {true , 0x000F},
    {true , 0x0031},
    {true , 0x002B},
    {true , 0x000C},
    {true , 0x000E},
    {true , 0x0008},
    {true , 0x004E},
    {true , 0x00F1},
    {true , 0x0037},
    {true , 0x0007},
    {true , 0x0010},
    {true , 0x0003},
    {true , 0x000E},
    {true , 0x0009},
    {true , 0x0000},
    {false, 0x00E1},
    {true , 0x0000},
    {true , 0x000E},
    {true , 0x0014},
    {true , 0x0003},
    {true , 0x0011},
    {true , 0x0007},
    {true , 0x0031},
    {true , 0x00C1},
    {true , 0x0048},
    {true , 0x0008},
    {true , 0x000F},
    {true , 0x000C},
    {true , 0x0031},
    {true , 0x0036},
    {true , 0x000f},
    {false, 0x00B1},
    {true , 0x0000},
    {true , 0x0001},
    {false, 0x00F6},
    {true , 0x0001},
    {true , 0x0010},
    {true , 0x0000},
    {false, 0x0011},
};

/* LT24 Init */
void LT24_INIT(){

	GPIOIP_INIT(GPIO1IP, GPIOBIDIR, 0, 0);

	GPIOIP_setDirection(GPIO1IP, LT_PIO);

	GPIOIP_setOutput(GPIO1IP, DISABLE);

	LT24_POWERCONFIG(GPIO1IP, ENABLE);
	delay(1);
	LT24_POWERCONFIG(GPIO1IP, DISABLE);
	delay(10);
	LT24_POWERCONFIG(GPIO1IP, ENABLE);
	delay(120);

	for(uint32_t i = 0; i < 92; i++)	LT24_WRITE(GPIO1IP, LT24_Init[i][0], LT24_Init[i][1]);

	delay(120);

	LT24_WRITE(GPIO1IP, false, 0x0029);

	LT24_DISPLAY(GPIO1IP, 0x0000);

	boot_img();

}

/* LT24 Set up the power config */
void LT24_POWERCONFIG(GPIOIP_TypeDef* gpio, bool state){

	if(state)
		gpio->opip.output = ((LT_RESET | LT_LCD_ON) & (LT_RESET | LT_LCD_ON)) |
								 (gpio->opip.output & ~(LT_RESET | LT_LCD_ON));
	else
        gpio->opip.output = (gpio->opip.output & ~(LT_RESET | LT_LCD_ON));

}

/* LT24 Write data */
void LT24_WRITE(GPIOIP_TypeDef* gpio, bool isData, uint16_t val)
{
    uint32_t regVal = (uint32_t)val;

    if (isData) regVal |= (LT_RS | LT_RDN);
    else        regVal |= LT_RDN;

    gpio->opip.output = (gpio->opip.output & ~LT_CMDDATA) | (regVal & LT_CMDDATA);

    regVal |= LT_WRN;

    gpio->opip.output = (gpio->opip.output & ~LT_CMDDATA) | (regVal & LT_CMDDATA);
}

/* LT24 Display Colours */
void LT24_DISPLAY(GPIOIP_TypeDef* gpio, uint16_t colr){

	LT24_SETWINDOW(gpio, MAX_WIDTH, MAX_HEIGHT, 0, 0);

	for(uint32_t it = 0; it < (MAX_WIDTH * MAX_HEIGHT); it++)	LT24_WRITE(gpio, true, colr);

}


/* LT24  Set the Wind for Coordinates */
void LT24_SETWINDOW(GPIOIP_TypeDef* gpio, uint16_t width, uint16_t height, uint16_t x_left, uint16_t y_top){

	uint32_t x_right = x_left + width - 1;
	uint32_t y_bottom = y_top + height - 1;

	if(x_right >= MAX_WIDTH) 	return;
	if(y_bottom >= MAX_HEIGHT)  return;


	if(x_left > x_right)	return;
	if(y_top > y_bottom)	return;

	/* Left and Right*/
	LT24_WRITE(gpio, false, 0x002A);
	LT24_WRITE(gpio, true, (x_left >> 8) & 0xFF);
	LT24_WRITE(gpio, true, x_left & 0xFF);
	LT24_WRITE(gpio, true, (x_right >> 8) & 0xFF);
	LT24_WRITE(gpio, true, x_right & 0xFF);

	/* Top and Bottom*/
	LT24_WRITE(gpio, false, 0x002B);
	LT24_WRITE(gpio, true, (y_top >> 8) & 0xFF);
	LT24_WRITE(gpio, true, y_top & 0xFF);
	LT24_WRITE(gpio, true, (y_bottom >> 8) & 0xFF);
	LT24_WRITE(gpio, true, y_bottom & 0xFF);

	LT24_WRITE(gpio, false, 0x002C);

}
