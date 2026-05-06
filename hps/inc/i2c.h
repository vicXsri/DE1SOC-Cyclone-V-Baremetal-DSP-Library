/*
 * i2c.h
 *
 *  Created on: 07-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef HPS_INC_I2C_H_
#define HPS_INC_I2C_H_

#include "main.h"

/* Typedef */
typedef struct
{
    __IO uint32_t con;
    __IO uint32_t tar;
    __IO uint32_t sar;
    uint32_t      RESERVED0;
    __IO uint32_t data_cmd;
    __IO uint32_t ss_scl_hcnt;
    __IO uint32_t ss_scl_lcnt;
    __IO uint32_t fs_scl_hcnt;
    __IO uint32_t fs_scl_lcnt;
    uint32_t      RESERVED1[2];
    __I  uint32_t intr_stat;
    __IO uint32_t intr_mask;
    __I  uint32_t raw_intr_stat;
    __IO uint32_t rx_tl;
    __IO uint32_t tx_tl;
    __I  uint32_t clr_intr;
    __I  uint32_t clr_rx_under;
    __I  uint32_t clr_rx_over;
    __I  uint32_t clr_tx_over;
    __I  uint32_t clr_rd_req;
    __I  uint32_t clr_tx_abrt;
    __I  uint32_t clr_rx_done;
    __I  uint32_t clr_activity;
    __I  uint32_t clr_stop_det;
    __I  uint32_t clr_start_det;
    __I  uint32_t clr_gen_call;
    __IO uint32_t enable;
    __I  uint32_t status;
    __I  uint32_t txflr;
    __I  uint32_t rxflr;
    __IO uint32_t sda_hold;
    __IO uint32_t tx_abrt_source;
    __IO uint32_t slv_data_nack_only;
    __IO uint32_t dma_cr;
    __IO uint32_t dma_tdlr;
    __IO uint32_t dma_rdlr;
    __IO uint32_t sda_setup;
    __IO uint32_t ack_general_call;
    __I  uint32_t enable_status;
    __IO uint32_t fs_spklen;
    uint32_t      RESERVED2[20];
    __I  uint32_t comp_param_1;
    __I  uint32_t comp_version;
    __I  uint32_t comp_type;
} I2C_TypeDef;

typedef struct{
	uint16_t SlaveAddress;
	uint16_t Mode;
	uint16_t Speed;
	uint16_t AddressMode;
	uint16_t MasterRestart;
}I2C_InitTypeDef;

typedef struct{
	I2C_TypeDef *Instance;
	I2C_InitTypeDef Init;
}I2C_HandleTypeDef;

/* Defines */
#define I2C0							((I2C_TypeDef *) I2C0_BASE)
#define I2C1							((I2C_TypeDef *) I2C1_BASE)
#define I2C2							((I2C_TypeDef *) I2C2_BASE)
#define I2C3							((I2C_TypeDef *) I2C3_BASE)


#define I2C_ENABLE						(0x01U)
#define I2C_DISABLE						(0x02U)

#define I2C_SPEED100					(0x01U << 1U)
#define I2C_SPEED400					(0x02U << 1U)

#define I2C_SLAVE7BIT					0U
#define I2C_SLAVE10BIT					(0x01U << 3U)

#define I2C_MASTER7BIT					0U
#define I2C_MASTER10BIT					(0x01U << 4U)

#define I2C_MODEMASTER					(0x01U << 0U)
#define I2C_MODESLAVE					(0x00U << 6U)

#define I2C_MASTERRESTARTENABLE			(0x01U << 5U)
#define I2C_MASTERRESTARTDISABLE		0U

#define I2C_SLAVEDISABLE				(0x01U << 6U)

#define I2C_RESTART						(0x01UL << 10U)

#define I2C_WRITE						0U
#define I2C_READ						(0x01UL << 8U)

/* Funtions */
Status_TypeDef I2C_Init(I2C_HandleTypeDef* i2c);
void I2C_WriteByte(I2C_HandleTypeDef* i2c, uint8_t address, uint8_t value);
Status_TypeDef I2C_WriteMultiBytes(I2C_HandleTypeDef* i2c, const uint8_t *data, uint32_t len);
void I2C_ReadByte(I2C_HandleTypeDef* i2c, uint8_t address, uint8_t *value);
void I2C_ReadMultiBytes(I2C_HandleTypeDef* i2c, uint8_t address, uint8_t value[], uint8_t len);

void I2C_MspInit(I2C_HandleTypeDef* i2c);

void ADXL345Init(I2C_HandleTypeDef* i2c);
void ADXL345GETId(I2C_HandleTypeDef* i2c, uint8_t *id);
void ADXL345XYZRead(I2C_HandleTypeDef* i2c, int16_t* data16);

bool ADXL345IsRxAvailable(I2C_HandleTypeDef* i2c);


Status_TypeDef WM8731_INIT(I2C_HandleTypeDef* i2c);
void WM8731_CODEC_WRITE(I2C_HandleTypeDef* i2c, uint8_t reg, uint16_t val);
#endif /* HPS_INC_I2C_H_ */
