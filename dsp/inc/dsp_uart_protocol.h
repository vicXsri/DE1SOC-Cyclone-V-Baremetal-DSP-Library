/*
 * dsp_uart_protocol.h
 *
 *  Created on: 17-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef DSP_UART_PROTOCOL_H
#define DSP_UART_PROTOCOL_H

#include "main.h"
#include <stdbool.h>
#include <stddef.h>





/* Defines */
#define DSP_UART_MAX_PARAMS 16U
#define DSP_UART_HEADER_SYNC_0 0xAAU
#define DSP_UART_HEADER_SYNC_1 0x55U
#define DSP_UART_HEADER_SIZE_BYTES 13U

/* Typedef */
typedef enum
{
    DSP_UART_CMD_START_CONFIG     = 0xF0U,
    DSP_UART_CMD_CONFIG_DONE      = 0xF1U,
    DSP_UART_CMD_REQUEST_RAW      = 0xF2U,
    DSP_UART_CMD_REQUEST_FILTERED = 0xF3U,
    DSP_UART_CMD_RESET_STATE      = 0xF4U,
    DSP_UART_CMD_PING             = 0xF5U,
    DSP_UART_CMD_CONNECT          = 0xF6U,
    DSP_UART_CMD_DISCONNECT       = 0xF7U
} dsp_uart_command_t;

typedef enum
{
    DSP_UART_APP_SDR     = 0x01U,
    DSP_UART_APP_AUDIO   = 0x03U,
} dsp_uart_application_t;

typedef enum
{
    DSP_UART_FILTER_SDR_FFT           = 0x10U,
    DSP_UART_FILTER_SDR_FIR           = 0x11U,
    DSP_UART_FILTER_AUDIO_LOWPASS     = 0x30U,
    DSP_UART_FILTER_AUDIO_HIGHPASS    = 0x31U,
    DSP_UART_FILTER_AUDIO_BANDPASS    = 0x32U,

} dsp_uart_filter_t;

typedef enum
{
    DSP_UART_PACKET_RAW_FLOAT      = 0x01U,
    DSP_UART_PACKET_FILTERED_FLOAT = 0x02U
} dsp_uart_packet_type_t;

typedef enum
{
    DSP_UART_STATE_IDLE = 0,
    DSP_UART_STATE_WAIT_APPLICATION,
    DSP_UART_STATE_WAIT_FILTER,
    DSP_UART_STATE_WAIT_PARAM_COUNT,
    DSP_UART_STATE_WAIT_PARAMS
} dsp_uart_parser_state_t;

typedef void (*dsp_uart_tx_byte_fn)(uint8_t byte, void *user_context);
typedef void (*dsp_uart_led_set_fn)(bool on, void *user_context);

typedef struct
{
    uint8_t current_application;
    uint8_t current_filter;
    uint8_t expected_param_count;
    uint8_t received_param_count;
    uint8_t params[DSP_UART_MAX_PARAMS];

    bool link_active;
    bool config_ready;
    bool raw_data_requested;
    bool filtered_data_requested;

    dsp_uart_parser_state_t parser_state;
    dsp_uart_tx_byte_fn tx_byte;
    dsp_uart_led_set_fn set_led;
    void *user_context;
} dsp_uart_context_t;

/* Funtions */
void dsp_uart_init(
    dsp_uart_context_t *ctx,
    dsp_uart_tx_byte_fn tx_byte,
    dsp_uart_led_set_fn set_led,
    void *user_context);

void dsp_uart_reset_config(dsp_uart_context_t *ctx);
void dsp_uart_handle_rx_byte(dsp_uart_context_t *ctx, uint8_t rx_byte);

void dsp_uart_send_float_packet(
    dsp_uart_context_t *ctx,
    dsp_uart_packet_type_t packet_type,
    const float *samples,
    uint32_t sample_count,
    uint32_t processing_time_ms);

void dsp_uart_protocol_init(void *uart);
void dsp_uart_protocol_rx_byte(uint8_t rx_byte);
void dsp_uart_protocol_task(void);

bool dsp_uart_raw_data_requested(void);
bool dsp_uart_filtered_data_requested(void);
void dsp_uart_clear_raw_data_request(void);
void dsp_uart_clear_filtered_data_request(void);

void dsp_uart_send_raw_samples(const float *samples, uint32_t sample_count);
void dsp_uart_send_filtered_samples(const float *samples, uint32_t sample_count, uint32_t processing_time_ms);

bool dsp_uart_is_config_complete(void);
uint8_t dsp_uart_get_application(void);
uint8_t dsp_uart_get_filter(void);

void getRaw(void);
void getFiltered(void);

#endif
