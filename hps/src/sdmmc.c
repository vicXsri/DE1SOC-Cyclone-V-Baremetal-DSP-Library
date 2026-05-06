/*
 * sdmmc.c
 *
 *  Created on: 06-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */


#include "sdmmc.h"

uint32_t SDMMC_CLK_FREQ = 0;

SDMMC_DMA_BUFF_t dma_Descriptors[SDMMC_DMA_DESC_COUNT] __attribute__((aligned(32)));

SDMMC_DMA_BUFF_t *dma_curr_Descriptors __attribute__((aligned(32)));

#define SDMMC_DMA_BUF_DESC_CACHE_SIZE (((SDMMC_DMA_DESC_COUNT*sizeof(SDMMC_DMA_BUFF_t)) + CACHE_LINE_SIZE - 1) & ~(CACHE_LINE_SIZE-1))

/* SDMMC Init */
Status_TypeDef SDMMC_Init(SDMMC_TypeDef* sdmmc){

	if(sdmmc == NULL) return ARM_ERROR;

	SDMMC_MspInit(sdmmc);

	if(CLOCK_IsEnabled(CLK_SDMMC) != true) return ARM_ERROR;

	CLOCK_GetFrequency(CLK_SDMMC, &SDMMC_CLK_FREQ);

	SDMMC_Reset(sdmmc);

	return ARM_OK;
}

/* SDMMC Card Reset */
Status_TypeDef SDMMC_Reset(SDMMC_TypeDef* sdmmc){

	bool status = SDMMC_IsPowerOn(sdmmc);

	if(status)	SDMMC_PowerOff(sdmmc);

	RSTMGR_Pheripherals(RSTMGR, RST_SDMMC, SDMMC_RESETTIMEOUT);

	if(status)	SDMMC_PowerOn(sdmmc);

	SDMMC_DMA_DescChainInit();


//	sdmmc->clkena |= (0x03 << 16); // need to verify this and debug !
	SDMMC_PowerOn(sdmmc);
	SDMMC_CMD_Send(sdmmc, CMD_BASIC, SDMMC_CLK_INDEX, 0x00, NULL);

	return ARM_OK;
}


/* SDMMC DMA DESC CHAIN Init */
static Status_TypeDef SDMMC_DMA_DescChainInit(){

	SDMMC_DMA_BUFF_t* dma_desc = dma_Descriptors;

	for(uint32_t cnt = 0; cnt < SDMMC_DMA_DESC_COUNT; cnt++){

		dma_desc[cnt].des0.fld.own 	= 0;
		dma_desc[cnt].des0.fld.ch  	= 1;
		dma_desc[cnt].des0.fld.er  	= 0;
		dma_desc[cnt].des1.fld.bs1 	= 0;
		dma_desc[cnt].des2.fld.bap1 = 0;

		if(cnt == (SDMMC_DMA_DESC_COUNT - 1))
			dma_desc[cnt].des3.fld.bap2_next = (uint32_t) dma_desc;
		else
			dma_desc[cnt].des3.fld.bap2_next = (uint32_t) (&dma_desc[cnt + 1]);
	}

	dma_curr_Descriptors = dma_desc;

	return cache_system_purge(dma_Descriptors, SDMMC_DMA_BUF_DESC_CACHE_SIZE);

}

/* SDMMC Command Send */
Status_TypeDef SDMMC_CMD_Send(SDMMC_TypeDef* sdmmc, SDMMC_CMDType cmd_typ, SDMMC_CMDINDEXType cmd, uint32_t cmd_arg, uint32_t* response){

	const SDMMC_CMD_CONFIGType *cmdCfg = NULL;

	uint32_t cntr = 0 ;
	uint32_t arrcntr = 0;
	bool found = false;
	uint32_t int_mask = SDMMC_INT_RE 	|
						SDMMC_INT_RTO	|
						SDMMC_INT_CD 	|
						SDMMC_INT_HLE   |
						SDMMC_INT_CMD;

	Status_TypeDef status =0;

	SDMMC_CMD_CONFIGType *default_cfg;

	if(cmd_typ == CMD_BASIC){
		default_cfg = cmd_default_cfg;
		arrcntr = sizeof(cmd_default_cfg) / sizeof(cmd_default_cfg[0]);
	}else if(cmd_typ == ACMD){
		default_cfg = acmd_default_cfg;
		arrcntr = sizeof(acmd_default_cfg) / sizeof(acmd_default_cfg[0]);
	}else{
		return ARM_ERROR;
	}

	if(cmd == SDMMC_CLK_INDEX){
		cmdCfg = &cmd_clock_cfg;
		found = true;
	}

	for(cntr = 0; cntr < arrcntr; cntr++){
		if(found == true){
			break;
		}
		if(default_cfg[cntr].cmd_index == cmd){
			cmdCfg = &default_cfg[cntr];
			found = true;
		}
	}

	if(found == false) return ARM_ERROR;

	if(cmdCfg->wait_prvdata_complete){
		uint32_t timeout = SDMMC_TMO_WAITER;
		while(SDMMC_IsBusy(sdmmc) && timeout--);
	}


	if(cmdCfg->data_expected == true){
		int_mask  = SDMMC_INT_DTO 	|
				SDMMC_INT_RCRC		|
				SDMMC_INT_DCRC		|
				SDMMC_INT_HTO		|
				SDMMC_INT_FRUN		|
				SDMMC_INT_EBE;
		if(cmdCfg->read_write == SDMMC_TMOD_WRITE){
			int_mask  = SDMMC_INT_TXDR 	|
						SDMMC_INT_HLE;
		}else{
			int_mask  = SDMMC_INT_RXDR 	|
						SDMMC_INT_SBE;
		}
	}


	SDMMC_intDisable(sdmmc, SDMMC_INT_STATUS_ALL);

	/* Reset all the interrupts */
	SDMMC_intClear(sdmmc, SDMMC_INT_STATUS_ALL);

	/* Interrupts Enable */
	SDMMC_intEnable(sdmmc, int_mask);

	/* Setup the Argument Resgister and send CMD */
	SDMMC_CMDArgSet(sdmmc, cmd_arg);

	/* Set Command Configuration */
	SDMMC_CMDSet(sdmmc, cmd, cmdCfg, false);

	/* Send Command */
	SDMMC_CMDSet(sdmmc, cmd, cmdCfg, true);



	return ARM_OK;
}

/* SDMMC Interrupt Enable */
void SDMMC_ActivateNotification(SDMMC_TypeDef* sdmmc){

	/*Enable Interrupt*/
	sdmmc->ctrl 	|= (0x01U << 4U);
	IRQ_Init(IRQ_SDMMC, SDMMC_IRQHandler);

}

/* Setting the specific Command Argument for sd card */
void SDMMC_CMDArgSet(SDMMC_TypeDef* sdmmc, uint32_t cmdarg){
	sdmmc->cmdarg =  cmdarg;
}

/* Setting the specific Command for sd card */
void SDMMC_CMDSet(SDMMC_TypeDef* sdmmc, const SDMMC_CMDINDEXType cmd_index, const SDMMC_CMD_CONFIGType *cmd_cfg, bool start_cmd){

	uint32_t cmdReg = ((cmd_index & 0x3FUL) << 0U)									|
					  ((cmd_cfg->response_expect & 0x40UL) << 6U)					|
					  ((cmd_cfg->response_length & 0x80UL) << 7U)					|
					  ((cmd_cfg->check_response_crc & 0x100UL) << 8U)				|
					  ((cmd_cfg->data_expected & 0x200UL) << 9U)					|
					  ((cmd_cfg->read_write & 0x400UL) << 10U)						|
					  ((cmd_cfg->transfer_mode & 0x800UL) << 11U)					|
					  ((cmd_cfg->send_auto_stop & 0x1000UL) << 12U)					|
					  ((cmd_cfg->wait_prvdata_complete & 0x2000UL) << 13U)			|
					  ((cmd_cfg->stop_abort_cmd & 0x4000UL) << 14U)					|
					  ((cmd_cfg->send_initialization & 0x8000UL) << 15U)			|
//					  ((cmd_cfg->card_number & 0x1F0000UL) << 16U)					|
					  ((cmd_cfg->update_clock_registers_only & 0x200000UL) << 21U)	|
					  ((cmd_cfg->read_ceata_device & 0x00400000UL) << 22U)			|
					  ((cmd_cfg->ccs_expected & 0x00800000UL) << 23U)				|
					  ((cmd_cfg->enable_boot & 0x01000000UL) << 24U)				|
					  ((cmd_cfg->expect_boot_ack & 0x02000000UL) << 25U)			|
					  ((cmd_cfg->disable_boot & 0x04000000UL) << 26U)				|
					  ((cmd_cfg->boot_mode & 0x08000000UL) << 27U)					|
					  ((cmd_cfg->volt_switch &  0x08000000UL) << 28U)				|
					  ((cmd_cfg->use_hold_reg & 0x20000000UL) << 29U)				|
					  ((start_cmd & 0x20000000UL) << 31U);

	sdmmc->cmd = cmdReg;
}

/* SDMMC Interrupt Clear */
void SDMMC_intClear(SDMMC_TypeDef* sdmmc, const uint32_t mask){
	sdmmc->rintsts = mask;
}

/* SDMMC Interrupt Disable */
void SDMMC_intDisable(SDMMC_TypeDef* sdmmc, const uint32_t mask){
	sdmmc->intmask &= ~mask;
}

/* SDMMC Interrupt Enable */
void SDMMC_intEnable(SDMMC_TypeDef* sdmmc, const uint32_t mask){

	if(mask & 0x1FFFF){
		sdmmc->ctrl |= (0x01UL << 4);
		sdmmc->intmask |= mask;

	}

}

/* SDMMC Card PWR On Status*/
bool SDMMC_IsPowerOn(SDMMC_TypeDef* sdmmc){
	return (sdmmc->pwren & (0x01U << 0));
}

/* SDMMC Card PWR On */
Status_TypeDef SDMMC_PowerOn(SDMMC_TypeDef* sdmmc){
	sdmmc->pwren |= (0x01U << 0U);
	return ARM_OK;
}

/* SDMMC Card PWR Off */
Status_TypeDef SDMMC_PowerOff(SDMMC_TypeDef* sdmmc){
	sdmmc->pwren &= ~(0x01U << 0U);
	return ARM_OK;
}

Status_TypeDef SDMMC_IsBusy(SDMMC_TypeDef* sdmmc){

	return (sdmmc->status & (0x01UL << 9U));

}

/* SDMMC Card Identify */
/* SDMMC Card Bus Width Length SET */
/* SDMMC Card Fifo Param SET */
/* SDMMC Card misc get */
/* SDMMC Card DMA */
