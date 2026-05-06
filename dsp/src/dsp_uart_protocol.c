/*
 * dsp_uart_protocol.c
 *
 *  Created on: 17-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#include "dsp_uart_protocol.h"

static dsp_uart_context_t g_dsp_uart;
static UART_HandleTypeDef *g_uart = NULL;

extern float SDR_fir_raw_data[SDR_FIR_NUM_SAMPLES * 2];
extern float SDR_fir_filtered_data[SDR_FIR_NUM_SAMPLES * 2];
extern uint32_t sdr_fir_totalTime;

extern float SDR_fft_raw_data[SDR_FFT_NUM_SAMPLES * 2];
extern float SDR_fft_filtered_data[SDR_FFT_NUM_SAMPLES * 2];
extern uint32_t sdr_fft_totalTime;

extern float AUDIO_fir_raw_data[AUDIO_FIR_NUM_SAMPLES];
extern float AUDIO_fir_filtered_data[AUDIO_FIR_NUM_SAMPLES];
extern uint32_t audio_fir_totalTime;

/* Setting the application */
static bool dsp_uart_is_application(uint8_t value)
{
    return value >= DSP_UART_APP_SDR && value <= DSP_UART_APP_AUDIO;
}

/* Check which Filter */
static bool dsp_uart_is_filter(uint8_t value)
{
    switch (value)
    {
    case DSP_UART_FILTER_SDR_FFT:
    case DSP_UART_FILTER_SDR_FIR:

    case DSP_UART_FILTER_AUDIO_LOWPASS:
    case DSP_UART_FILTER_AUDIO_HIGHPASS:
    case DSP_UART_FILTER_AUDIO_BANDPASS:
        return true;
    default:
        return false;
    }
}

/* Send data tx */
static void dsp_uart_tx_byte(uint8_t byte, void *user_context)
{
    UART_HandleTypeDef *uart = (UART_HandleTypeDef *)user_context;
    debug(uart, byte);
}

/* Set the Handshake Led */
static void dsp_uart_led_callback(bool on, void *user_context)
{
    (void)user_context;

    if (on) {
        GPIO_WritePin(GPIO1, USER_LED, 1);
    } else {
        GPIO_WritePin(GPIO1, USER_LED, 0);
    }
}

/* Send UART ACK */
static void dsp_uart_send_ack(dsp_uart_context_t *ctx, uint8_t value)
{
    if ((ctx == NULL) || (ctx->tx_byte == NULL))
    {
        return;
    }

    ctx->tx_byte(value, ctx->user_context);
}

/* Seting the LED */
static void dsp_uart_set_led(dsp_uart_context_t *ctx, bool on)
{
    if ((ctx == NULL) || (ctx->set_led == NULL))
    {
        return;
    }

    ctx->set_led(on, ctx->user_context);
}

/* DSP Protocol UART Init */
void dsp_uart_protocol_init(void *uart)
{
    g_uart = (UART_HandleTypeDef *)uart;
    dsp_uart_init(&g_dsp_uart, dsp_uart_tx_byte, dsp_uart_led_callback, g_uart);
}

/* DSP Transmit RX Data */
void dsp_uart_protocol_rx_byte(uint8_t rx_byte)
{
    dsp_uart_handle_rx_byte(&g_dsp_uart, rx_byte);
}

/* DSP UART Init */
void dsp_uart_init(
    dsp_uart_context_t *ctx,
    dsp_uart_tx_byte_fn tx_byte,
    dsp_uart_led_set_fn set_led,
    void *user_context)
{
    if (ctx == NULL)
    {
        return;
    }

    memset(ctx, 0, sizeof(*ctx));
    ctx->tx_byte = tx_byte;
    ctx->set_led = set_led;
    ctx->user_context = user_context;
    ctx->parser_state = DSP_UART_STATE_IDLE;
}

/* DSP UART reset config */
void dsp_uart_reset_config(dsp_uart_context_t *ctx)
{
    if (ctx == NULL)
    {
        return;
    }

    ctx->current_application = 0U;
    ctx->current_filter = 0U;
    ctx->expected_param_count = 0U;
    ctx->received_param_count = 0U;
    ctx->config_ready = false;
    ctx->raw_data_requested = false;
    ctx->filtered_data_requested = false;
    ctx->parser_state = DSP_UART_STATE_IDLE;

    memset(ctx->params, 0, sizeof(ctx->params));
}

/* DSP UART rx handle byte */
void dsp_uart_handle_rx_byte(dsp_uart_context_t *ctx, uint8_t rx_byte)
{
    if (ctx == NULL)
    {
        return;
    }

    dsp_uart_send_ack(ctx, rx_byte);

    switch (rx_byte)
    {
    case DSP_UART_CMD_CONNECT:
        ctx->link_active = true;
        dsp_uart_set_led(ctx, true);
        return;

    case DSP_UART_CMD_DISCONNECT:
        ctx->link_active = false;
        dsp_uart_set_led(ctx, false);
        dsp_uart_reset_config(ctx);
        return;

    case DSP_UART_CMD_RESET_STATE:
        dsp_uart_reset_config(ctx);
        return;

    case DSP_UART_CMD_START_CONFIG:
        dsp_uart_reset_config(ctx);
        ctx->parser_state = DSP_UART_STATE_WAIT_APPLICATION;
        return;

    case DSP_UART_CMD_CONFIG_DONE:
        if ((ctx->current_application != 0U) &&
            (ctx->current_filter != 0U) &&
            (ctx->received_param_count == ctx->expected_param_count))
        {
            ctx->config_ready = true;
        }
        return;

    case DSP_UART_CMD_PING:
        return;

    case DSP_UART_CMD_REQUEST_RAW:
        ctx->raw_data_requested = true;
        return;

    case DSP_UART_CMD_REQUEST_FILTERED:
        ctx->filtered_data_requested = true;
        return;

    default:
        break;
    }

    switch (ctx->parser_state)
    {
    case DSP_UART_STATE_WAIT_APPLICATION:
        if (dsp_uart_is_application(rx_byte))
        {
            ctx->current_application = rx_byte;
            ctx->parser_state = DSP_UART_STATE_WAIT_FILTER;
        }
        break;

    case DSP_UART_STATE_WAIT_FILTER:
        if (dsp_uart_is_filter(rx_byte))
        {
            ctx->current_filter = rx_byte;
            ctx->parser_state = DSP_UART_STATE_WAIT_PARAM_COUNT;
        }
        break;

    case DSP_UART_STATE_WAIT_PARAM_COUNT:
        if (rx_byte <= DSP_UART_MAX_PARAMS)
        {
            ctx->expected_param_count = rx_byte;
            ctx->received_param_count = 0U;
            ctx->parser_state = (rx_byte == 0U) ? DSP_UART_STATE_IDLE : DSP_UART_STATE_WAIT_PARAMS;
        }
        break;

    case DSP_UART_STATE_WAIT_PARAMS:
        if (ctx->received_param_count < ctx->expected_param_count)
        {
            ctx->params[ctx->received_param_count++] = rx_byte;
            if (ctx->received_param_count >= ctx->expected_param_count)
            {
                ctx->parser_state = DSP_UART_STATE_IDLE;
            }
        }
        break;

    case DSP_UART_STATE_IDLE:
    default:
        break;
    }
}

/* DSP UART send Float packets */
void dsp_uart_send_float_packet(
    dsp_uart_context_t *ctx,
    dsp_uart_packet_type_t packet_type,
    const float *samples,
    uint32_t sample_count,
	uint32_t processing_time_ms)
{
    uint32_t index;

    if ((ctx == NULL) || (ctx->tx_byte == NULL) || (samples == NULL))
    {
        return;
    }

    ctx->tx_byte(DSP_UART_HEADER_SYNC_0, ctx->user_context);
    ctx->tx_byte(DSP_UART_HEADER_SYNC_1, ctx->user_context);
    ctx->tx_byte((uint8_t)packet_type, ctx->user_context);
    ctx->tx_byte(ctx->current_application, ctx->user_context);
    ctx->tx_byte(ctx->current_filter, ctx->user_context);
    ctx->tx_byte((uint8_t)(sample_count & 0x000000FFUL), ctx->user_context);
    ctx->tx_byte((uint8_t)((sample_count >> 8) & 0x000000FFUL), ctx->user_context);
    ctx->tx_byte((uint8_t)((sample_count >> 16) & 0x000000FFUL), ctx->user_context);
    ctx->tx_byte((uint8_t)((sample_count >> 24) & 0x000000FFUL), ctx->user_context);
    ctx->tx_byte((uint8_t)(processing_time_ms & 0x000000FFUL), ctx->user_context);
    ctx->tx_byte((uint8_t)((processing_time_ms >> 8) & 0x000000FFUL), ctx->user_context);
    ctx->tx_byte((uint8_t)((processing_time_ms >> 16) & 0x000000FFUL), ctx->user_context);
    ctx->tx_byte((uint8_t)((processing_time_ms >> 24) & 0x000000FFUL), ctx->user_context);

    for (index = 0U; index < sample_count; ++index)
    {
        union
        {
            float value;
            uint8_t bytes[sizeof(float)];
        } sample;

        sample.value = samples[index];
        ctx->tx_byte(sample.bytes[0], ctx->user_context);
        ctx->tx_byte(sample.bytes[1], ctx->user_context);
        ctx->tx_byte(sample.bytes[2], ctx->user_context);
        ctx->tx_byte(sample.bytes[3], ctx->user_context);
    }
}

/* DSP UART send Float packets */
bool dsp_uart_raw_data_requested(void)
{
    return g_dsp_uart.raw_data_requested;
}

bool dsp_uart_filtered_data_requested(void)
{
    return g_dsp_uart.filtered_data_requested;
}

void dsp_uart_clear_raw_data_request(void)
{
    g_dsp_uart.raw_data_requested = false;
}

void dsp_uart_clear_filtered_data_request(void)
{
    g_dsp_uart.filtered_data_requested = false;
}

void dsp_uart_send_raw_samples(const float *samples, uint32_t sample_count)
{
    dsp_uart_send_float_packet(&g_dsp_uart, DSP_UART_PACKET_RAW_FLOAT, samples, sample_count,0);
}

void dsp_uart_send_filtered_samples(const float *samples, uint32_t sample_count, uint32_t processing_time_ms)
{
    dsp_uart_send_float_packet(&g_dsp_uart, DSP_UART_PACKET_FILTERED_FLOAT, samples, sample_count, processing_time_ms);
}

bool dsp_uart_is_config_complete(void)
{
    return (g_dsp_uart.current_application != 0U) &&
           (g_dsp_uart.current_filter != 0U);
}

uint8_t dsp_uart_get_application(void)
{
    return g_dsp_uart.current_application;
}

uint8_t dsp_uart_get_filter(void)
{
    return g_dsp_uart.current_filter;
}

void getRaw(void){

	HEXIP_Text(WRITE_TST);

	switch(dsp_uart_get_application()){

		case DSP_UART_APP_SDR:
			switch (dsp_uart_get_filter())
				{
					case DSP_UART_FILTER_SDR_FFT:
						dsp_uart_send_raw_samples(SDR_fft_raw_data, SDR_FFT_NUM_SAMPLES*2);
						break;
					case DSP_UART_FILTER_SDR_FIR:
						dsp_uart_send_raw_samples(SDR_fir_raw_data, SDR_FIR_NUM_SAMPLES*2);
						break;
				}
		break;

		case DSP_UART_APP_AUDIO:
			switch(dsp_uart_get_filter())
			{
				case DSP_UART_FILTER_AUDIO_BANDPASS:
					dsp_uart_send_raw_samples(AUDIO_fir_raw_data, AUDIO_FIR_NUM_SAMPLES);
				break;
				case DSP_UART_FILTER_AUDIO_HIGHPASS:
				break;
				case DSP_UART_FILTER_AUDIO_LOWPASS:
				break;
			}
		break;
	}

	dsp_uart_clear_raw_data_request();
	HEXIP_Text(WRITE_TDONE);
	delay(1000);
}
void getFiltered(void){

	HEXIP_Text(WRITE_TST);

	switch(dsp_uart_get_application()){

		case DSP_UART_APP_SDR:
			switch (dsp_uart_get_filter())
				{
					case DSP_UART_FILTER_SDR_FFT:
						dsp_uart_send_filtered_samples(SDR_fft_filtered_data, SDR_FFT_NUM_SAMPLES*2, sdr_fft_totalTime);
						break;
					case DSP_UART_FILTER_SDR_FIR:
						dsp_uart_send_filtered_samples(SDR_fir_filtered_data, SDR_FIR_NUM_SAMPLES*2, sdr_fir_totalTime);
						break;
				}
		break;

		case DSP_UART_APP_AUDIO:
			switch(dsp_uart_get_filter())
			{
				case DSP_UART_FILTER_AUDIO_BANDPASS:
					dsp_uart_send_filtered_samples(AUDIO_fir_filtered_data, AUDIO_FIR_NUM_SAMPLES, audio_fir_totalTime);
				break;
				case DSP_UART_FILTER_AUDIO_HIGHPASS:
				break;
				case DSP_UART_FILTER_AUDIO_LOWPASS:
				break;
			}
		break;
	}

    dsp_uart_clear_filtered_data_request();
	HEXIP_Text(WRITE_TDONE);
	delay(1000);
}

