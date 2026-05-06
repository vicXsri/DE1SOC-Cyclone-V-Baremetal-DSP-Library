/*
 * main.c
 *
 *  Created on: 31-Mar-2026
 *      Author: Srivisweswara Mohan Santhi
 */

/*
 * Works Need to be completed:
 * Map Interrupts to Funtions ! -> Done
 * Private Timer Delay -> Done
 * Next SD CARD using SDMMC & FatFs -> used internal libraries, tried my best to create the baremetal, did some baremetal derivers
 * Work on DMA -> Done
 * Uart Recieve, Ring Buffer Needs to Be updated !!! -> not required !
 * GitHub - Forking & Merging !
 * Start Working on the Algorithm -> done remaining, image and sensor filters !
 * Start looking to creating an rtos or integrating one along with the project -> not required for now !
 * Check if we need the DDR3 Ram - Done
 * */

/*
 * sw0 -> SDR_FIR
 * sw1 -> SDR_FFT
 * sw2 -> AUDIO_FIR
 * sw3 -> SENSOR_KALMAN
 * sw4 -> AUDIO Select
 * sw5 -> IMAGE_MEDIAN
 * sw6 -> filtered Image
 * sw7 -> unfiltered Image
 * sw8 -> Play Filtered Audio
 * sw9 -> PLay Unfiltered Audio
 * Key0 -> Volume Decrease
 * Key1 -> Volume Increase
 */

#include "main.h"

/* User Defined Defines Begin */

/* User Defined Defines End */

/* User Defined Handles Begin */

UART_HandleTypeDef huart0;

I2C_HandleTypeDef 	hi2c0;

AUDIO_BufferTypeDef rabbit;
AUDIO_BufferTypeDef audiodata;
AUDIO_BufferTypeDef audiodatafil;

BMPImage	minionraw;
BMPImage	minionfil;

/* User Defined Handles End */

/* User Defined Functions Begin */

void M_CLOCK_Init(void);
void M_GIC_Init(void);
void M_PTIM_Init(void);
void M_GPIO_Init(void);
void M_UART_Init(void);
void M_I2C_Init(void);
void M_SDMMC_Init(void);

void error_handler(uint8_t num);
void debugAddress(void);

/* User Defined Functions End */

/* User Defined Variables Begin */

uint8_t id = 0;

float xl345Data[3] = {0};
bool picState = false;

/* User Defined Variables End */

/* User Code 0 Begin */

void FilterRun(void){

	dsp_uart_protocol_init(&huart0);

	if(SWITCHIP_ReadPin(SWITCHIP, SDR_FIR_SW))												SDR_FIR_WAV_IQ_Parser();
	if(SWITCHIP_ReadPin(SWITCHIP, SDR_FFT_SW))												SDR_FFT_WAV_IQ_Parser();
	if(SWITCHIP_ReadPin(SWITCHIP, AUDIO_FIR_SW))											AUDIO_FIR_WAV_IQ_Parser();
	if(SWITCHIP_ReadPin(SWITCHIP, KALMAN_SW) && !SWITCHIP_ReadPin(SWITCHIP, I2C_MUX_SW)) 	KalmanFilter_Init();
	if(SWITCHIP_ReadPin(SWITCHIP, MEDIAN_IMAGE)) 											MEDIAN_IMAG_BMP_IQ_Parser();

}

void AudioSel(void){

	AUDIOToRAM("audiodata.wav", &audiodata);
	AUDIOToRAM("audiodatafil.wav", &audiodatafil);

}

void ImageSel(void){

	BMPToRAM("minion.bmp", &minionraw);
	BMPToRAM("minionfil.bmp", &minionfil);

}

/* User Code 0 End */

int main(void) {

/* User Code 1 Begin */

//	debugAddress();

	/* Intialize Clock */
//	M_CLOCK_Init();

	HEXIP_Text(WRITE_HOLD);

	/* Intialize Interrupts */
	M_GIC_Init();

	/* Intialize Private Timer */
	M_PTIM_Init();

	/* Intialize GPIO */
	M_GPIO_Init();

	/* Intialize UART */
	M_UART_Init();

	/* Intialize I2C */
	M_I2C_Init();

	/* Intialize SDMMC */
//	M_SDMMC_Init();

	FATFS fs;
	FRESULT res;

	/* Mount the SD CARD */
	res = 																					f_mount(&fs, "", 1);
	if (res != FR_OK)																		error_handler(5);

	/* Check if Switch[3] is high and  Switch[4] low and init the ADXL345 Acclerometer */
	if(SWITCHIP_ReadPin(SWITCHIP, KALMAN_SW   ) && !SWITCHIP_ReadPin(SWITCHIP, I2C_MUX_SW)) ADXL345Init(&hi2c0);

	/* Check if Switch[3] is high and  Switch[4] low and get the ADXL345 Sensor ID */
	if(SWITCHIP_ReadPin(SWITCHIP, KALMAN_SW   ) && !SWITCHIP_ReadPin(SWITCHIP, I2C_MUX_SW)) ADXL345GETId(&hi2c0, &id);

	/* Verify the ADXL345 ID */
	if(id == DEVICE_ID                        )												printf("ADXL345 Device Id Verified\n");

	/* Check if Switch[4] is high and  Switch[3] low init WM8731 CODEC */
	if(SWITCHIP_ReadPin(SWITCHIP, I2C_MUX_SW  ) && !SWITCHIP_ReadPin(SWITCHIP, KALMAN_SW)) 	WM8731_INIT(&hi2c0);

	/* Check if Switch[5] is high and intialize the LT24 Display */
	if(SWITCHIP_ReadPin(SWITCHIP, MEDIAN_IMAGE)											 )	LT24_INIT();

	/* Check if Switch[0] is high or Switch[1] is high or Switch[2] is high or Switch[3] is high or Switch[5] is high and run the filters accordingly */
	if(SWITCHIP_ReadPin(SWITCHIP, SDR_FIR_SW  ) || SWITCHIP_ReadPin(SWITCHIP, SDR_FFT_SW )
	|| SWITCHIP_ReadPin(SWITCHIP, AUDIO_FIR_SW) || SWITCHIP_ReadPin(SWITCHIP, KALMAN_SW  )
	|| SWITCHIP_ReadPin(SWITCHIP, MEDIAN_IMAGE)											 )	FilterRun();

	/* Check if Switch[4] is high and  Switch[3] low load the audio from the sd card to ram */
	if(!SWITCHIP_ReadPin(SWITCHIP, KALMAN_SW  ) && SWITCHIP_ReadPin(SWITCHIP, I2C_MUX_SW)) 	AudioSel();

	/* Check if Switch[5] is high load the image from the sd card to ram */
	if(SWITCHIP_ReadPin(SWITCHIP, MEDIAN_IMAGE)											 )	ImageSel();

/* User Code 1 End */

	while(1){

/* User Code 2 Begin */

		/* Verify the picState and display the group image accordingly */
		if(picState == false)																													displayGroup();

		/* Run kalman filter if selected and print the output */
		if(SWITCHIP_ReadPin(SWITCHIP, KALMAN_SW)  && !SWITCHIP_ReadPin(SWITCHIP, I2C_MUX_SW))		if(Kalman_ReadXYZ(xl345Data))				printf("X = %.1f g\r\n Y = %0.1f g\r\n Z = %0.1f g\r\nRoll -> %.02f  |  Pitch -> %.02f\r\n", xl345Data[0], xl345Data[1], xl345Data[2], ADXL345CalcRoll(xl345Data), ADXL345CalcPitch(xl345Data));

		/* Play Unfiltered Audio */
		if(SWITCHIP_ReadPin(SWITCHIP, PLAY_UNFIL) && SWITCHIP_ReadPin(SWITCHIP, AUDIO_FIR_SW) && SWITCHIP_ReadPin(SWITCHIP, I2C_MUX_SW))		AUDIO_PLAY(&audiodata, PLAY_UNFIL);

		/* Play Filtered Audio */
		if(SWITCHIP_ReadPin(SWITCHIP, PLAY_FIL)   && SWITCHIP_ReadPin(SWITCHIP, AUDIO_FIR_SW) && SWITCHIP_ReadPin(SWITCHIP, I2C_MUX_SW))		AUDIO_PLAY(&audiodatafil, PLAY_FIL);

		/* Display the Unfiltered Image */
		if(SWITCHIP_ReadPin(SWITCHIP, IMAGE_UNFIL))																								BMPImage_Display(&minionraw);

		/* Display the Filtered Image */
		if(SWITCHIP_ReadPin(SWITCHIP, IMAGE_FIL))																								BMPImage_Display(&minionfil);

		/* Send Unfiltered SDR || Audio Data */
		if (dsp_uart_raw_data_requested())																										getRaw();

		/* Send Filtered SDR || Audio Data */
        if (dsp_uart_filtered_data_requested())																									getFiltered();

        /* Display the text ready if the  no process running for conectivty of GUI */
        HEXIP_Blink(WRITE_READY, 50);

	}

/* User Code 2 End */

}

/* User Code 3 Begin */

void M_CLOCK_Init(void){

	CLOCK_InitTypedef CLOCK_InitStruct ={0};

	CLOCK_InitStruct.mainclkmgr.safemode = 						CLKSAFEMODEEN;
	CLOCK_InitStruct.mainclkmgr.safemodedebug = 				CLKDEBUGAFEMODEEN;
	CLOCK_InitStruct.mainclkmgr.debugclk = 						CLKDEBUGOSC1;

	CLOCK_InitStruct.mainpll.enableMainVco = 					ENABLE;
	CLOCK_InitStruct.mainpll.numerator = 						72-1;
	CLOCK_InitStruct.mainpll.denominator = 						0x00;
	CLOCK_InitStruct.mainpll.saten = 			   				ENABLE;
	CLOCK_InitStruct.mainpll.bwadj = 							0x01;
	CLOCK_InitStruct.mainpll.mpuclkdiv = 		   				0x00;
	CLOCK_InitStruct.mainpll.mainclkdiv = 		   				0x00;
	CLOCK_InitStruct.mainpll.dbgatclkdiv = 		   				0x00;
	CLOCK_InitStruct.mainpll.mainqspiclkdiv = 	   				0x04;
	CLOCK_InitStruct.mainpll.mainnandsdmmcclkdiv = 				0x1FF;
	CLOCK_InitStruct.mainpll.cfgs2fuser0clkdiv =   				0x11;
	CLOCK_InitStruct.mainpll.l3mpclkdiv =   					DIV2;
	CLOCK_InitStruct.mainpll.l3spclkdiv =   					DIV2;
	CLOCK_InitStruct.mainpll.l4mpclkdiv =   					DIV2;
	CLOCK_InitStruct.mainpll.l4spclkdiv =   					DIV2;
	CLOCK_InitStruct.mainpll.dbgatclk =   						DIV1;
	CLOCK_InitStruct.mainpll.dbgclk =   						DIV2;
	CLOCK_InitStruct.mainpll.traceclk =   						DIV1;
	CLOCK_InitStruct.mainpll.l4mpsrc =   						PERIBASECLK;
	CLOCK_InitStruct.mainpll.l4spsrc =   						PERIBASECLK;

	CLOCK_InitStruct.peripheralpll.enablePeripheralVco =   		ENABLE;
	CLOCK_InitStruct.peripheralpll.numerator =   				40-1;
	CLOCK_InitStruct.peripheralpll.denominator =   				0x00;
	CLOCK_InitStruct.peripheralpll.saten =   					ENABLE;
	CLOCK_InitStruct.peripheralpll.bwadj =   					0x01;
	CLOCK_InitStruct.peripheralpll.emac0div =   				0x1FF;
	CLOCK_InitStruct.peripheralpll.emac1div =   				0x03;
	CLOCK_InitStruct.peripheralpll.perqspidiv =   				0x1FF;
	CLOCK_InitStruct.peripheralpll.pernandsdmmcdiv =   			0x04;
	CLOCK_InitStruct.peripheralpll.perbasediv =   				0x04;
	CLOCK_InitStruct.peripheralpll.s2fuser1div =   				0x1FF;
	CLOCK_InitStruct.peripheralpll.usbclkdiv =   				DIV2;
	CLOCK_InitStruct.peripheralpll.spimclkdiv =   				DIV2;
	CLOCK_InitStruct.peripheralpll.can0clkdiv =   				DIV2;
	CLOCK_InitStruct.peripheralpll.can1clkdiv =   				DIV2;
	CLOCK_InitStruct.peripheralpll.gpiodbclkdiv =   			0x1869;
	CLOCK_InitStruct.peripheralpll.sdmmcclk =   				PERIPHNANDSDMMCCLK;
	CLOCK_InitStruct.peripheralpll.nandclk =   					PERIPHNANDSDMMCCLK;
	CLOCK_InitStruct.peripheralpll.qspiclk =   					PERIPHNANDSDMMCCLK;

	CLOCK_InitStruct.sdrampll.enableSdramVco =					ENABLE;
	CLOCK_InitStruct.sdrampll.numerator =						32-1;
	CLOCK_InitStruct.sdrampll.denominator =						0x00;
	CLOCK_InitStruct.sdrampll.bwadj =							0x01;
	CLOCK_InitStruct.sdrampll.saten =							ENABLE;

	if(	CLOCK_Init(CLOCK, &CLOCK_InitStruct) != ARM_OK){
		error_handler(0);
	}
}

void M_GIC_Init(void){

	/*Masking Priority Setup*/

	if(	GIC_Init(GIC, IRQDIS) != ARM_OK){
		error_handler(1);
	}
}

void M_PTIM_Init(void){

	PTim_InitTypeDef PTIM_InitStruct = {0};

	PTIM_InitStruct.Prescaler = 224;
	PTIM_InitStruct.ReloadValue = 1000 - 1;

	if(	PTIM_Init(PTIM, &PTIM_InitStruct) != ARM_OK){ 	// 1ms
		error_handler(2);
	}
	PTIM_ActivateNotification(PTIM);
}

void M_GPIO_Init(void){

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__CLOCK_GPIO_ENABLE();

	/* I2C0 Pin for Audio*/
	if(SWITCHIP_ReadPin(SWITCHIP, SW_PIN_4) && !SWITCHIP_ReadPin(SWITCHIP, SW_PIN_3)){

		GPIO_InitStruct.Mode = GPIO_OUTPUT;
		GPIO_InitStruct.Pin = I2C_MUX_HIGH;

		if(	GPIO_Init(GPIO1, &GPIO_InitStruct) != ARM_OK)	error_handler(3);
		else												{ GPIO_WritePin(GPIO1, I2C_MUX_HIGH, HIGH);		LEDIP_WritePin(LEDIP, GPIO_PIN_4, HIGH); }

	}

	GPIO_InitStruct.Mode = GPIO_OUTPUT;
	GPIO_InitStruct.Pin = USER_LED;

	if(	GPIO_Init(GPIO1, &GPIO_InitStruct) != ARM_OK){
		error_handler(3);
	}
	GPIO_InitStruct.Mode = GPIO_INPUT;
	GPIO_InitStruct.Pin = USER_BUTTON;

	if(	GPIO_Init(GPIO1, &GPIO_InitStruct) != ARM_OK){
		error_handler(3);
	}
}

void M_UART_Init(void){

	huart0.Instance = UART0;

	huart0.Init.BaudRate = 115200;//625000
	huart0.Init.DataLength = UART_DL_8B;
	huart0.Init.parity = UART_PARITY_NONE;
	huart0.Init.StopBits = UART_STOP_BIT1;

	if(UART_Init(&huart0) != ARM_OK){
		error_handler(4);
	}

	/*Enable UART Interrupt*/
	UART_ActivateNotification(&huart0);
}

void M_I2C_Init(void){

	if(SWITCHIP_ReadPin(SWITCHIP, SW_PIN_4) && !SWITCHIP_ReadPin(SWITCHIP, SW_PIN_3)){
		/* Audio */
		hi2c0.Instance = I2C0;

		hi2c0.Init.Mode = I2C_MODEMASTER;
		hi2c0.Init.Speed = I2C_SPEED100;
		hi2c0.Init.AddressMode = I2C_MASTER10BIT;
		hi2c0.Init.MasterRestart = I2C_MASTERRESTARTENABLE;
	}
	else{
		/* Acc */
		hi2c0.Instance = I2C0;

		hi2c0.Init.Mode = I2C_MODEMASTER;
		hi2c0.Init.Speed = I2C_SPEED400;
		hi2c0.Init.AddressMode = I2C_MASTER7BIT;
		hi2c0.Init.MasterRestart = I2C_MASTERRESTARTENABLE;
		hi2c0.Init.SlaveAddress = 0x53;
	}

	if(I2C_Init(&hi2c0) != ARM_OK){
		error_handler(5);
	}
}

void M_SDMMC_Init(void){

	if(SDMMC_Init(SDMMC) != ARM_OK){
		error_handler(6);
	}

	SDMMC_ActivateNotification(SDMMC);

}

/* Function To handle error handling, toggles LED's from FPGA IP Block */
void error_handler(uint8_t num){
	while(1){
		LEDIP_TogglePin(LEDIP, num);
		delay(1000);
	}
}

/* Function to verify the Address of peripherals, to check if the typedef register address are correct */
void debugAddress(void){

	printf("Addr ic_fs_spklen: 0x%08X\n", (unsigned int)&I2C0->fs_spklen);
	printf("Addr ic_comp_type: 0x%08X\n", (unsigned int)&I2C0->comp_type);
//	printf("Addr bufaddr5: 0x%08X\n", (unsigned int)&SDMMC->bufaddr);
//	printf("Addr back_end_power_r: 0x%08X\n", (unsigned int)&SDMMC->back_end_power_r);
//	printf("Addr data: 0x%08X\n", (unsigned int)&SDMMC->data);
//	printf("SYSMGR base: 0x%08X\n", (unsigned int)SYSMGR);
//	printf("Addr parityinj: 0x%08X\n", (unsigned int)&SYSMGR->parityinj);
//	printf("Addr hwctrl: 0x%08X\n", (unsigned int)&SYSMGR->hwctrl);
//	printf("Addr bootromswstate: 0x%08X\n", (unsigned int)&SYSMGR->bootromswstate);
//	printf("Addr ecc_sdmmc: 0x%08X\n", (unsigned int)&SYSMGR->ecc_sdmmc);
//	printf("Addr EMACIO0: 0x%08X\n", (unsigned int)&SYSMGR->EMACIO0);
//	printf("Addr FLASHIO11: 0x%08X\n", (unsigned int)&SYSMGR->FLASHIO11);
//	printf("Addr GENERALIO1: 0x%08X\n", (unsigned int)&SYSMGR->GENERALIO1);
//	printf("Addr GPLMUX16: 0x%08X\n", (unsigned int)&SYSMGR->GPLMUX16);
//	printf("Addr GPLMUX70: 0x%08X\n", (unsigned int)&SYSMGR->GPLMUX70);
//	printf("Addr SPIM0USEFPGA: 0x%08X\n", (unsigned int)&SYSMGR->SPIM0USEFPGA);
//	printf("UART0 base: 0x%p\n", &(huart0.Instance->srr));
}

/* User Code 3 End */
