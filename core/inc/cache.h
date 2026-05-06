/*
 * cache.h
 *
 *  Created on: 13-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#ifndef CORE_INC_CACHE_H_
#define CORE_INC_CACHE_H_

#include "main.h"

/* Defines */
#define MPUL2_CACHE_SYNC_ADDR   0xFFFEF730UL
#define MPUL2_INV_PA_ADDR 		0xFFFEF770UL
#define MPUL2_CLEAN_INV_PA_ADDR 0xFFFEF7F0UL

#define CACHE_L2_PURGE_ADDR_TIMEOUT 128
#define CACHE_L2_INVALIDATE_ADDR_TIMEOUT 128
#define CACHE_L2_SYNC_TIMEOUT 128

#define CACHE_LINE_SIZE         32


/* Funtions */
Status_TypeDef cache_system_purge(void* vaddress, uint32_t length);
static Status_TypeDef cache_l1_data_cleaner_helper(void* vaddress, uint32_t length);
static Status_TypeDef cache_l2_purge_helper(uint32_t paddress, uint32_t length);
Status_TypeDef cache_l2_sync();
static Status_TypeDef cache_l1_data_invalidate_helper(void* vaddress, uint32_t length);

static inline void write_word(uintptr_t dest, uint32_t value);
static inline uint32_t read_word(uintptr_t addr);

#endif /* CORE_INC_CACHE_H_ */
