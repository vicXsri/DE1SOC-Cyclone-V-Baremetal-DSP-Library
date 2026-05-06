/*
 * cache.c
 *
 *  Created on: 13-Apr-2026
 *      Author: Srivisweswara Mohan Santhi
 */

#include "cache.h"

/* Full Cache Coherency */
Status_TypeDef cache_system_purge(void* vaddress, uint32_t length){

	if(((uint32_t)vaddress & (CACHE_LINE_SIZE - 1)) != 0){
		return ARM_ERROR;
	}

	if((length & (CACHE_LINE_SIZE - 1)) != 0){
		return ARM_ERROR;
	}

	if(length){
		cache_l1_data_cleaner_helper(vaddress, length);
		cache_l2_purge_helper((uint32_t)vaddress, length);
		cache_l2_sync();
		cache_l1_data_invalidate_helper(vaddress, length);
	}

	return ARM_OK;

}

/* Cache Function for cleaning the L1 Cache*/
static Status_TypeDef cache_l1_data_cleaner_helper(void* vaddress, uint32_t length){

	for(uint32_t va = (uint32_t)vaddress; va < (uint32_t)vaddress + length; va += CACHE_LINE_SIZE){
		__asm("MCR p15, 0, %[va], c7, c10, 1": : [va] "r" (va));
	}

	__asm("dsb");

	return ARM_OK;
}

/* Cache Function for cleaning and invalidating the L2 Cache Content */
static Status_TypeDef cache_l2_purge_helper(uint32_t paddress, uint32_t length){

	uint32_t i=0;

	for(uint32_t pa = paddress; pa < paddress + length; pa += CACHE_LINE_SIZE){
		write_word(MPUL2_CLEAN_INV_PA_ADDR ,pa);
	}

	while(read_word(MPUL2_CLEAN_INV_PA_ADDR) & (0x01U << 0U)){
		if(i == CACHE_L2_PURGE_ADDR_TIMEOUT ){
			return ARM_TIMEOUT;
		}
		++i;
	}
	return ARM_OK;
}

/* Cache Function for Synchronising the L2 Cache */
Status_TypeDef cache_l2_sync(){

	uint32_t i = 0;

	write_word(MPUL2_CACHE_SYNC_ADDR,0);

	while(read_word(MPUL2_CACHE_SYNC_ADDR)){
		if(i == CACHE_L2_SYNC_TIMEOUT){
			return ARM_TIMEOUT;
		}
		++i;
	}

	return ARM_OK;
}
/* Cache Function for CACHE L1 Data invalidation */
static Status_TypeDef cache_l1_data_invalidate_helper(void* vaddress, uint32_t length){

	uint32_t va=0;
    for (va = (uint32_t)vaddress; va < (uint32_t)vaddress + length; va += CACHE_LINE_SIZE){
        __asm("MCR p15, 0, %[va], c7, c6, 1" : : [va] "r" (va));
    }
    __asm("dsb");

	return ARM_OK;

}

static inline void write_word(uintptr_t dest, uint32_t value){
    *(volatile uint32_t *)dest = value;
}


static inline uint32_t read_word(uintptr_t addr){
    return *(volatile uint32_t *)addr;
}



