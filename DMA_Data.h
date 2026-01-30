#ifndef DMA_DATA_H
#define DMA_DATA_H

#include "linux_hal.h"

// --- BIẾN ĐIỀU KHIỂN LUỒNG ---
extern int trigger_adc;
extern int trigger_FFT;
extern pthread_mutex_t radar_data_mutex;

// Định nghĩa các địa chỉ vật lý
#define DMA_FFT_REGS_PHYS   0xA01C0000 // Địa chỉ thanh ghi DMA
#define DMA_FFT_REGS_SIZE   0x10000    // Kích thước vùng thanh ghi (64KB)

#define FFT_MEM_PHYS        0x50000000 // Địa chỉ vùng RAM đã reserved
#define FFT_MEM_SIZE        0x10000000 // Kích thước 256MB

#define DESC_MEM_OFFSET     0x0        // Descriptor bắt đầu tại offset 0
#define BUFFER_MEM_OFFSET   0x1000     // Buffer dữ liệu bắt đầu sau đó (ví dụ 4KB)

// Các offset của thanh ghi S2MM (để đọc dữ liệu)
#define S2MM_DMACR          0x30
#define S2MM_DMASR          0x34
#define S2MM_CURDESC        0x38
#define S2MM_TAILDESC       0x40

#define FFT_PACKET_SIZE     (16384 * 16 * 4) // Ví dụ: 1MB

// Cấu trúc Descriptor, phải khớp với phần cứng
typedef struct {
    uint32_t nxtdesc;
    uint32_t nxtdesc_msb; // Thường là 0 cho Zynq
    uint32_t buffer_addr;
    uint32_t buffer_addr_msb; // Thường là 0
    uint32_t reserved[2];
    uint32_t control;
    uint32_t status;
    uint32_t app[5];
    uint32_t reserved2[3];
} DMA_Descriptor;


// ADC (Simple Mode)
int DMA_ADC_Read_Once(uint64_t phys_addr, uint32_t length);

// Workers
void* ADC_Worker_Thread(void* arg);
void* FFT_Monitor_SG_S2MM_Thread(void* arg);
int DMA_FFT_Read_Once(uint64_t phys_addr, uint32_t length);
void* FFT_Worker_Thread(void* arg);

#endif // DMA_DATA_H