#ifndef DMA_HAL_H
#define DMA_HAL_H

#include "linux_hal.h"

// --- API FUNCTIONS ---
void cmac_setup_for_dma_loopback();
void run_full_dma_loopback_test(uint64_t tx_phys_addr, uint64_t rx_phys_addr, uint32_t pkt_size);
void test_external_loopback_cable();
void* CMAC_Monitor_Thread(void* arg);
void* CMAC_TX_External_Worker_Thread(void* arg);
//int dma_only_test_send(uint64_t phys_addr, uint32_t size);
int dma_only_test_send(uint64_t phys_addr, uint32_t size, int check_tready);
void* DMA_Isolation_Test_Thread(void* arg);
void cmac_dma_test_send_and_read();
void* CMAC_TX_Worker_Thread(void* arg);
void* CMAC_TX_Worker_Thread2(void* arg);
void cmac_dma_run_send(uint64_t src_phys, uint32_t pkt_size);
// CMAC Loopback Tests
void cmac_dma_run_transfer(uint32_t src_phys, uint32_t dst_phys, uint32_t pkt_size);


#endif