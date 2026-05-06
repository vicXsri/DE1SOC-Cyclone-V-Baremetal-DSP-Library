/*
 * sdmmc.h
 *
 *  Created on: 06-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef INC_SDMMC_H_
#define INC_SDMMC_H_

#include "main.h"
/* Typedef */
typedef struct
{
    __IO uint32_t ctrl;
    __IO uint32_t pwren;
    __IO uint32_t clkdiv;
    __IO uint32_t clksrc;
    __IO uint32_t clkena;
    __IO uint32_t tmout;
    __IO uint32_t ctype;
    __IO uint32_t blksiz;
    __IO uint32_t bytcnt;
    __IO uint32_t intmask;
    __IO uint32_t cmdarg;
    __IO uint32_t cmd;
    __I  uint32_t resp0;
    __I  uint32_t resp1;
    __I  uint32_t resp2;
    __I  uint32_t resp3;
    __I  uint32_t mintsts;
    __IO uint32_t rintsts;
    __I  uint32_t status;
    __IO uint32_t fifoth;
    __I  uint32_t cdetect;
    __I  uint32_t wrtprt;
    uint32_t RESERVED0;
    __I  uint32_t tcbcnt;
    __I  uint32_t tbbcnt;
    __IO uint32_t debnce;
    __IO uint32_t usrid;
    __I  uint32_t verid;
    __I  uint32_t hcon;
    __IO uint32_t uhs_reg;
    __IO uint32_t rst_n;
    uint32_t RESERVED1;
    __IO uint32_t bmod;
    __O  uint32_t pldmnd;
    __IO uint32_t dbaddr;
    __IO uint32_t idsts;
    __IO uint32_t idinten;
    __I  uint32_t dscaddr;
    __I  uint32_t bufaddr;
    uint32_t RESERVED2[25];
    __IO uint32_t cardthrctl;
    __IO uint32_t back_end_power_r;
    uint32_t RESERVED3[62];
    __IO uint32_t data;
} SDMMC_TypeDef;

typedef enum{
	CMD_BASIC,
	ACMD
}SDMMC_CMDType;

/*From SD CARD Offical Site Documents*/
typedef enum{
    SDMMC_GO_IDLE_STATE          = 0,
	SDMMC_ALL_SEND_CID			 = 2,
	SDMMC_SET_RELATIVE_ADDR  	 = 3,
	SDMMC_SWITCH           		 = 6,
	SDMMC_SEL_DES_CARD           = 7,
    SDMMC_IF_COND                = 8,
    SDMMC_SEND_CSD               = 9,
    SDMMC_SEND_CID               = 10,
    SDMMC_STOP_TRANSMISSION      = 12,
	SDMMC_SEND_STATUS			 = 13,
    SDMMC_SET_BLOCKLEN           = 16,
    SDMMC_READ_SINGLE_BLOCK      = 17,
    SDMMC_READ_MULTIPLE_BLOCK    = 18,
    SDMMC_WRITE_BLOCK            = 24,
    SDMMC_WRITE_MULTIPLE_BLOCK   = 25,
    SDMMC_TAG_SECTOR_START       = 32,
    SDMMC_TAG_SECTOR_END         = 33,
    SDMMC_ERASE                  = 38,
    SDMMC_APP_CMD                = 55,
    SDMMC_READ_OCR               = 58,

    SDMMC_STANDART_CMD_ALL       = 60,

    MMC_SEND_OP_COND             = 1,
    SD_SET_BUS_WIDTH             = 6,
    SD_SD_STATUS                 = 13,
    SD_SEND_OP_COND              = 41,
    SD_SEND_SCR                  = 51,

	SDMMC_CLK_INDEX 			 = -1,

    SDMMC_CMD_ALL                = SDMMC_STANDART_CMD_ALL + 1


}SDMMC_CMDINDEXType;

/* Refer SDMMC CMD Register !*/
typedef struct{

	uint32_t cmd_index						:6; /* 5 - 0 */
	uint32_t response_expect				:1; /* 6 */
	uint32_t response_length				:1; /* 7 */
	uint32_t check_response_crc				:1; /* 8 */
	uint32_t data_expected					:1; /* 9 */
	uint32_t read_write						:1; /* 10 */
	uint32_t transfer_mode					:1; /* 11 */
	uint32_t send_auto_stop					:1; /* 12 */
	uint32_t wait_prvdata_complete			:1; /* 13 */
	uint32_t stop_abort_cmd					:1; /* 14 */
	uint32_t send_initialization			:1; /* 15 */
	uint32_t card_number					:5; /* 20 - 16 */
	uint32_t update_clock_registers_only	:1; /* 21 */
	uint32_t read_ceata_device				:1; /* 22 */
	uint32_t ccs_expected					:1; /* 23 */
	uint32_t enable_boot					:1; /* 24 */
	uint32_t expect_boot_ack				:1; /* 25 */
	uint32_t disable_boot					:1; /* 26 */
	uint32_t boot_mode						:1; /* 27 */
	uint32_t volt_switch					:1; /* 28 */
	uint32_t use_hold_reg					:1; /* 29 */
	uint32_t res							:1; /* Reserved */
	uint32_t start_cmd						:1; /* 31 */

}SDMMC_CMD_CONFIGType;

typedef enum{
	SDMMC_TMOD_READ		= 0,
	SDMMC_TMOD_WRITE	= 1
}SDMMC_TMODType;

typedef struct{

	union
	{
		struct
			{
				uint32_t			:1; /* Reserved */

				uint32_t dic		:1; /* When set to 1, this bit prevents the setting of the TI/RI
										bit of the internal DMA controller status register (idsts)
										for the data that ends in the buffer pointed to by this
										descriptor */

				uint32_t ld			:1; /* When set to 1, this bit indicates that the buffers pointed
										to by this descriptor are the last buffers of the data. */

				uint32_t fs			:1; /* When set to 1, this bit indicates that this descriptor
										contains the first buffer of the data. If the size of the first
										buffer is 0, next descriptor contains the beginning of the
										data. */

				uint32_t ch			:1; /* When set to 1, this bit indicates that the second address
										in the descriptor is the next descriptor address rather
										than the second buffer address. When this bit is set to 1,
										BS2 (DES1[25:13]) must be all zeros. */

				uint32_t er			:1; /* When set to 1, this bit indicates that the descriptor list
										reached its final descriptor. The internal DMA controller
										returns to the base address of the list, creating a
										descriptor ring. ER is meaningful for only a dual-buffer
										descriptor structure. */

				uint32_t 			:24; /* Reserved */

				uint32_t ces		:1; /* The CES bit indicates whether a transaction error
										occurred. The CES bit is the logical OR of the following
										error bits in the rintsts register.
										• End-bit error (ebe)
										• Response timeout (rto)
										• Response CRC (rcrc)
										• Start-bit error (sbe)
										• Data read timeout (drto)
										• Data CRC for receive (dcrc)
										• Response error (re) */

				uint32_t own		:1; /* When set to 1, this bit indicates that the descriptor is
										owned by the internal DMA controller.
										When this bit is set to 0, it indicates that the descriptor is
										owned by the host. The internal DMA controller resets
										this bit to 0 when it completes the data transfer. */
			}fld;

		uint32_t raw;

	}des0;

	union
	{
		struct
			{
				uint32_t bs1		:13; /* Indicates the data buffer byte size, which must be a
										multiple of four bytes. When the buffer size is not a
										multiple of four, the resulting behavior is undefined. If
										this field is 0, the DMA ignores the buffer and proceeds
										to the next descriptor for a chain structure, or to the
										next buffer for a dual-buffer structure.
										If there is only one descriptor and only one buffer to be
										programmed, you need to use only buffer 1 and not
										buffer 2 */

				uint32_t bs2		:13; /* This field indicates the second data buffer byte size. The
										buffer size must be a multiple of four. When the buffer
										size is not a multiple of four, the resulting behavior is
										undefined. This field is not valid if DES0[4] is set to 1. */

				uint32_t 			:6; /* Reserved */
			}fld;

		uint32_t raw;

	}des1;

	union
	{
		struct
			{
				uint32_t bap1		:32;
			}fld;

		uint32_t raw;

	}des2;

	union
	{
		struct
			{
				uint32_t bap2_next	:32;
			}fld;

		uint32_t raw;

	}des3;


}SDMMC_DMA_BUFF_t;

/* We can Find this in the intmask Register of SDMCC */
typedef enum{
	SDMMC_INT_CD			= (0x01UL << 0U), /* Card Detected */
	SDMMC_INT_RE			= (0x01UL << 1U), /* Response error */
	SDMMC_INT_CMD			= (0x01UL << 2U), /* Command Done */
	SDMMC_INT_DTO			= (0x01UL << 3U), /* Data transfer */
	SDMMC_INT_TXDR			= (0x01UL << 4U), /* Transmit FIFO */
	SDMMC_INT_RXDR			= (0x01UL << 5U), /* Receive FIFO */
	SDMMC_INT_RCRC			= (0x01UL << 6U), /* Response CRC */
	SDMMC_INT_DCRC			= (0x01UL << 7U), /* Data CRC */
	SDMMC_INT_RTO			= (0x01UL << 8U), /* Response timeout */
	SDMMC_INT_DRT			= (0x01UL << 9U), /* Data read timeout */
	SDMMC_INT_HTO			= (0x01UL << 10U), /* Data starvation by host timeout */
	SDMMC_INT_FRUN			= (0x01UL << 11U), /* FIFO underrun/overru */
	SDMMC_INT_HLE			= (0x01UL << 12U), /* Hardware locked write */
	SDMMC_INT_SBE			= (0x01UL << 13U), /* start-bit error */
	SDMMC_INT_ACD			= (0x01UL << 14U), /* Auto command done */
	SDMMC_INT_EBE			= (0x01UL << 15U), /* End-bit error */
	SDMMC_INT_SDIO_INT_MASK	= (0x01UL << 16U),  /* 	SDIO Interrup*/
	SDMMC_INT_STATUS_ALL	= 0x1FFFF /* ALL The interrupts */
}SDMMC_INT_StatusTypedef;


/* Defines */
#define    DEFAULT_CMD_INDEX                         0
#define    DEFAULT_RESPONSE_EXPECT                   false
#define    DEFAULT_RESPONSE_LENGTH_LONG              false
#define    DEFAULT_CHECK_RESPONSE_CRC                false
#define    DEFAULT_DATA_EXPECTED                     0
#define    DEFAULT_WRITE_ACTIVE                      false
#define    DEFAULT_STREAM_MODE_ACTIVE                false
#define    DEFAULT_SEND_AUTO_STOP                    false
#define    DEFAULT_WAIT_PRVDATA_COMPLETE             false
#define    DEFAULT_STOP_ABORT_CMD                    false
#define    DEFAULT_SEND_INITIALIZATION               false
#define    DEFAULT_CARD_NUMBER                       0
#define    DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY       false
#define    DEFAULT_READ_CEATA_DEVICE                 false
#define    DEFAULT_CSS_EXPECTED                      false
#define    DEFAULT_ENABLE_BOOT                       false
#define    DEFAULT_EXPECT_BOOT_ACK                   false
#define    DEFAULT_DISABLE_BOOT                      false
#define    DEFAULT_BOOT_MODE                         false
#define    DEFAULT_VOLT_SWITCH                       false
#define    DEFAULT_USE_HOLD_REG                      false
#define    DEFAULT_RESERVED                          0
#define    DEFAULT_START_BIT                         0


static SDMMC_CMD_CONFIGType cmd_default_cfg[] =
{
    {
        SDMMC_WRITE_MULTIPLE_BLOCK, /* .cmd_index */
        true, /* .response_expect */
        DEFAULT_RESPONSE_LENGTH_LONG,
        DEFAULT_CHECK_RESPONSE_CRC,
        1, /* .data_expected  */
        true, /* .write_active */
        DEFAULT_STREAM_MODE_ACTIVE,
        true, /* .send_auto_stop */
        true, /* .wait_prvdata_complete */
        DEFAULT_STOP_ABORT_CMD,
        DEFAULT_SEND_INITIALIZATION,
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    },

    {
        SDMMC_READ_MULTIPLE_BLOCK, /* .cmd_index */
        true, /* .response_expect */
        DEFAULT_RESPONSE_LENGTH_LONG,
        DEFAULT_CHECK_RESPONSE_CRC,
        1, /* .data_expected */
        DEFAULT_WRITE_ACTIVE,
        DEFAULT_STREAM_MODE_ACTIVE,
        true, /* .send_auto_stop */
        true, /* .wait_prvdata_complete */
        DEFAULT_STOP_ABORT_CMD,
        DEFAULT_SEND_INITIALIZATION,
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    },
    {
        SDMMC_WRITE_BLOCK, /* .cmd_index */
        true, /* .response_expect */
        DEFAULT_RESPONSE_LENGTH_LONG,
        DEFAULT_CHECK_RESPONSE_CRC,
        true, /* .data_expected */
        true, /* .write_active */
        DEFAULT_STREAM_MODE_ACTIVE,
        DEFAULT_SEND_AUTO_STOP,
        true, /* .wait_prvdata_complete */
        DEFAULT_STOP_ABORT_CMD,
        DEFAULT_SEND_INITIALIZATION,
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    },
    {
        SDMMC_READ_SINGLE_BLOCK, /* .cmd_index */
        true, /* .response_expect */
        DEFAULT_RESPONSE_LENGTH_LONG,
        DEFAULT_CHECK_RESPONSE_CRC,
        true, /* .data_expected */
        DEFAULT_WRITE_ACTIVE,
        DEFAULT_STREAM_MODE_ACTIVE,
        DEFAULT_SEND_AUTO_STOP,
        true, /* .wait_prvdata_complete */
        DEFAULT_STOP_ABORT_CMD,
        DEFAULT_SEND_INITIALIZATION,
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    },
    {
        SDMMC_SET_BLOCKLEN, /* .cmd_index */
        true, /* .response_expect */
        DEFAULT_RESPONSE_LENGTH_LONG,
        DEFAULT_CHECK_RESPONSE_CRC,
        DEFAULT_DATA_EXPECTED,
        DEFAULT_WRITE_ACTIVE,
        DEFAULT_STREAM_MODE_ACTIVE,
        DEFAULT_SEND_AUTO_STOP,
        true, /* .wait_prvdata_complete */
        DEFAULT_STOP_ABORT_CMD,
        DEFAULT_SEND_INITIALIZATION,
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    },
    {
    	SDMMC_SEL_DES_CARD, /* .cmd_index */
        true, /* .response_expect */
        DEFAULT_RESPONSE_LENGTH_LONG,
        DEFAULT_CHECK_RESPONSE_CRC,
        DEFAULT_DATA_EXPECTED,
        DEFAULT_WRITE_ACTIVE,
        DEFAULT_STREAM_MODE_ACTIVE,
        DEFAULT_SEND_AUTO_STOP,
        true, /* .wait_prvdata_complete */
        DEFAULT_STOP_ABORT_CMD,
        DEFAULT_SEND_INITIALIZATION,
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    },
    {
        SDMMC_APP_CMD, /* .cmd_index */
        true, /* .response_expect */
        DEFAULT_RESPONSE_LENGTH_LONG,
        DEFAULT_CHECK_RESPONSE_CRC,
        DEFAULT_DATA_EXPECTED,
        DEFAULT_WRITE_ACTIVE,
        DEFAULT_STREAM_MODE_ACTIVE,
        DEFAULT_SEND_AUTO_STOP,
        true, /* .wait_prvdata_complete */
        DEFAULT_STOP_ABORT_CMD,
        DEFAULT_SEND_INITIALIZATION,
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    },
    {
        SDMMC_IF_COND, /* .cmd_index */
        true, /* .response_expect */
        DEFAULT_RESPONSE_LENGTH_LONG,
        DEFAULT_CHECK_RESPONSE_CRC,
        DEFAULT_DATA_EXPECTED,
        DEFAULT_WRITE_ACTIVE,
        DEFAULT_STREAM_MODE_ACTIVE,
        DEFAULT_SEND_AUTO_STOP,
        true, /* .wait_prvdata_complete */
        DEFAULT_STOP_ABORT_CMD,
        DEFAULT_SEND_INITIALIZATION,
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    },
    {
    	SDMMC_SET_RELATIVE_ADDR, /* .cmd_index */
        true, /* .response_expect */
        DEFAULT_RESPONSE_LENGTH_LONG,
        DEFAULT_CHECK_RESPONSE_CRC,
        DEFAULT_DATA_EXPECTED,
        DEFAULT_WRITE_ACTIVE,
        DEFAULT_STREAM_MODE_ACTIVE,
        DEFAULT_SEND_AUTO_STOP,
        true, /* .wait_prvdata_complete */
        DEFAULT_STOP_ABORT_CMD,
        DEFAULT_SEND_INITIALIZATION,
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    },
    {
        SDMMC_SEND_STATUS, /* .cmd_index */
        true, /* .response_expect */
        DEFAULT_RESPONSE_LENGTH_LONG,
        DEFAULT_CHECK_RESPONSE_CRC,
        DEFAULT_DATA_EXPECTED,
        DEFAULT_WRITE_ACTIVE,
        DEFAULT_STREAM_MODE_ACTIVE,
        DEFAULT_SEND_AUTO_STOP,
        DEFAULT_WAIT_PRVDATA_COMPLETE,
        DEFAULT_STOP_ABORT_CMD,
        DEFAULT_SEND_INITIALIZATION,
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    },
    {
        SDMMC_ALL_SEND_CID, /* .cmd_index */
        true, /* .response_expect */
        true, /* .response_length_long */
        DEFAULT_CHECK_RESPONSE_CRC,
        DEFAULT_DATA_EXPECTED,
        DEFAULT_WRITE_ACTIVE,
        DEFAULT_STREAM_MODE_ACTIVE,
        DEFAULT_SEND_AUTO_STOP,
        true, /* .wait_prvdata_complete */
        DEFAULT_STOP_ABORT_CMD,
        DEFAULT_SEND_INITIALIZATION,
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    },
    {
        SDMMC_SEND_CID, /* .cmd_index */
        true, /* .response_expect */
        true, /* .response_length_long */
        DEFAULT_CHECK_RESPONSE_CRC,
        DEFAULT_DATA_EXPECTED,
        DEFAULT_WRITE_ACTIVE,
        DEFAULT_STREAM_MODE_ACTIVE,
        DEFAULT_SEND_AUTO_STOP,
        true, /* .wait_prvdata_complete */
        DEFAULT_STOP_ABORT_CMD,
        DEFAULT_SEND_INITIALIZATION,
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    },
    {
        SDMMC_SEND_CSD, /* .cmd_index */
        true, /* .response_expect */
        true, /* .response_length_long */
        DEFAULT_CHECK_RESPONSE_CRC,
        DEFAULT_DATA_EXPECTED,
        DEFAULT_WRITE_ACTIVE,
        DEFAULT_STREAM_MODE_ACTIVE,
        DEFAULT_SEND_AUTO_STOP,
        true, /* .wait_prvdata_complete */
        DEFAULT_STOP_ABORT_CMD,
        DEFAULT_SEND_INITIALIZATION,
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    },
    {
        SDMMC_STOP_TRANSMISSION, /* .cmd_index */
        DEFAULT_RESPONSE_EXPECT,
        DEFAULT_RESPONSE_LENGTH_LONG,
        DEFAULT_CHECK_RESPONSE_CRC,
        DEFAULT_DATA_EXPECTED,
        DEFAULT_WRITE_ACTIVE,
        DEFAULT_STREAM_MODE_ACTIVE,
        DEFAULT_SEND_AUTO_STOP,
        true, /* .wait_prvdata_complete */
        true, /* .stop_abort_cmd */
        DEFAULT_SEND_INITIALIZATION,
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    },
    {
        SDMMC_GO_IDLE_STATE, /* .cmd_index */
        DEFAULT_RESPONSE_EXPECT,
        DEFAULT_RESPONSE_LENGTH_LONG,
        DEFAULT_CHECK_RESPONSE_CRC,
        DEFAULT_DATA_EXPECTED,
        DEFAULT_WRITE_ACTIVE,
        DEFAULT_STREAM_MODE_ACTIVE,
        DEFAULT_SEND_AUTO_STOP,
        true, /* .wait_prvdata_complete */
        DEFAULT_STOP_ABORT_CMD,
        true, /* .send_initialization */
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    },
    {
        SDMMC_SWITCH, /* .cmd_index */
        true, /* .response_expect */
        DEFAULT_RESPONSE_LENGTH_LONG,
        DEFAULT_CHECK_RESPONSE_CRC,
        true, /* .data_expected */
        DEFAULT_WRITE_ACTIVE,
        DEFAULT_STREAM_MODE_ACTIVE,
        DEFAULT_SEND_AUTO_STOP,
        true, /* .wait_prvdata_complete */
        DEFAULT_STOP_ABORT_CMD,
        DEFAULT_SEND_INITIALIZATION,
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    }
};

/*  Default configurations of used ACMDs*/
static SDMMC_CMD_CONFIGType acmd_default_cfg[] =
{
    {
        SD_SEND_SCR, /* .cmd_index */
        true, /* .response_expect */
        DEFAULT_RESPONSE_LENGTH_LONG,
        DEFAULT_CHECK_RESPONSE_CRC,
        true, /* .data_expected */
        DEFAULT_WRITE_ACTIVE,
        DEFAULT_STREAM_MODE_ACTIVE,
        DEFAULT_SEND_AUTO_STOP,
        DEFAULT_WAIT_PRVDATA_COMPLETE,
        DEFAULT_STOP_ABORT_CMD,
        DEFAULT_SEND_INITIALIZATION,
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    },
    {
        SD_SEND_OP_COND, /* .cmd_index */
        true, /* .response_expect */
        DEFAULT_RESPONSE_LENGTH_LONG,
        DEFAULT_CHECK_RESPONSE_CRC,
        DEFAULT_DATA_EXPECTED,
        DEFAULT_WRITE_ACTIVE,
        DEFAULT_STREAM_MODE_ACTIVE,
        DEFAULT_SEND_AUTO_STOP,
        true, /* .wait_prvdata_complete */
        DEFAULT_STOP_ABORT_CMD,
        DEFAULT_SEND_INITIALIZATION,
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    },
    {
        SD_SET_BUS_WIDTH, /* .cmd_index */
        true, /* .response_expect */
        DEFAULT_RESPONSE_LENGTH_LONG,
        DEFAULT_CHECK_RESPONSE_CRC,
        DEFAULT_DATA_EXPECTED,
        DEFAULT_WRITE_ACTIVE,
        DEFAULT_STREAM_MODE_ACTIVE,
        DEFAULT_SEND_AUTO_STOP,
        true, /* .wait_prvdata_complete */
        DEFAULT_STOP_ABORT_CMD,
        DEFAULT_SEND_INITIALIZATION,
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    },
    {
    	SD_SD_STATUS, /* .cmd_index */
        true, /* .response_expect */
        DEFAULT_RESPONSE_LENGTH_LONG,
        DEFAULT_CHECK_RESPONSE_CRC,
        true, /* .data_expected */
        DEFAULT_WRITE_ACTIVE,
        DEFAULT_STREAM_MODE_ACTIVE,
        DEFAULT_SEND_AUTO_STOP,
        DEFAULT_WAIT_PRVDATA_COMPLETE,
        DEFAULT_STOP_ABORT_CMD,
        DEFAULT_SEND_INITIALIZATION,
        0, /* .card_number */
        DEFAULT_UPDATE_CLOCK_REGISTERS_ONLY,
        DEFAULT_READ_CEATA_DEVICE,
        DEFAULT_CSS_EXPECTED,
        DEFAULT_ENABLE_BOOT,
        DEFAULT_EXPECT_BOOT_ACK,
        DEFAULT_DISABLE_BOOT,
        DEFAULT_BOOT_MODE,
        DEFAULT_VOLT_SWITCH,
        true, /* .use_hold_reg */
        DEFAULT_RESERVED,
        DEFAULT_START_BIT
    }
};
static SDMMC_CMD_CONFIGType cmd_clock_cfg =
{
    DEFAULT_CMD_INDEX,
    DEFAULT_RESPONSE_EXPECT,
    DEFAULT_RESPONSE_LENGTH_LONG,
    DEFAULT_CHECK_RESPONSE_CRC,
    DEFAULT_DATA_EXPECTED,
    DEFAULT_WRITE_ACTIVE,
    DEFAULT_STREAM_MODE_ACTIVE,
    DEFAULT_SEND_AUTO_STOP,
    true, /* .wait_prvdata_complete */
    DEFAULT_STOP_ABORT_CMD,
    DEFAULT_SEND_INITIALIZATION,
    DEFAULT_CARD_NUMBER,
    true, /* .update_clock_registers_only */
    DEFAULT_READ_CEATA_DEVICE,
    DEFAULT_CSS_EXPECTED,
    DEFAULT_ENABLE_BOOT,
    DEFAULT_EXPECT_BOOT_ACK,
    DEFAULT_DISABLE_BOOT,
    DEFAULT_BOOT_MODE,
    DEFAULT_VOLT_SWITCH,
    DEFAULT_USE_HOLD_REG,
    DEFAULT_RESERVED,
    DEFAULT_START_BIT
};


/* Defines */

#define SDMMC	((SDMMC_TypeDef *) SDMMC_BASE)

#define SDMMC_RESETTIMEOUT		8192UL
#define SDMMC_DMA_DESC_COUNT	128U
#define  SDMMC_TMO_WAITER       1000000UL

/* Funtions */
Status_TypeDef SDMMC_Init(SDMMC_TypeDef* sdmmc);
void SDMMC_MspInit(SDMMC_TypeDef* sdmmc);
Status_TypeDef SDMMC_Reset(SDMMC_TypeDef* sdmmc);
Status_TypeDef SDMMC_CMD_Send(SDMMC_TypeDef* sdmmc, SDMMC_CMDType cmd_typ, SDMMC_CMDINDEXType cmd, uint32_t cmd_arg, uint32_t* response);
static Status_TypeDef SDMMC_DMA_DescChainInit();

bool SDMMC_IsPowerOn(SDMMC_TypeDef* sdmmc);
Status_TypeDef SDMMC_PowerOn(SDMMC_TypeDef* sdmmc);
Status_TypeDef SDMMC_PowerOff(SDMMC_TypeDef* sdmmc);

void SDMMC_CMDArgSet(SDMMC_TypeDef* sdmmc, uint32_t cmdarg);
void SDMMC_CMDSet(SDMMC_TypeDef* sdmmc, const SDMMC_CMDINDEXType cmd_index, const SDMMC_CMD_CONFIGType *cmd_cfg, bool start_cmd);

Status_TypeDef SDMMC_IsBusy(SDMMC_TypeDef* sdmmc);

void SDMMC_ActivateNotification(SDMMC_TypeDef* sdmmc);
void SDMMC_intClear(SDMMC_TypeDef* sdmmc, const uint32_t mask);
void SDMMC_intDisable(SDMMC_TypeDef* sdmmc, const uint32_t mask);
void SDMMC_intEnable(SDMMC_TypeDef* sdmmc, const uint32_t mask);

#endif /* INC_SDMMC_H_ */
