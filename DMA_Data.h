#ifndef DMA_DATA_H
#define DMA_DATA_H

#include "linux_hal.h"

#include <stdint.h> // Thêm cho uint8_t, uint32_t, v.v. (thay xil_types.h)

// --- BIẾN ĐIỀU KHIỂN LUỒNG ---
extern int trigger_adc;
extern int trigger_FFT;

// Các định nghĩa về thanh ghi DMA AXI
#define MM2S_DMACR      0x00
#define MM2S_DMASR      0x04
#define S2MM_DMACR      0x30
#define S2MM_DMASR      0x34
#define S2MM_CURDESC    0x38
#define S2MM_TAILDESC   0x40

#define S2MM_BD_STS_IOC_IX   0x80000000 // Bit 31: IOC (Interrupt On Complete)

typedef struct {
    uint32_t next_desc_ptr_low;     // 0x00
    uint32_t next_desc_ptr_high;    // 0x04
    uint32_t reserved1[2];          // 0x08, 0x0C
    uint32_t buf_addr_low;          // 0x10
    uint32_t buf_addr_high;         // 0x14
    uint32_t reserved2[2];          // 0x18, 0x1C
    uint32_t ctrl;                  // 0x20
    uint32_t status;                // 0x24
    uint32_t reserved3[6];          // 0x28 -> 0x3F (Pad to 64 bytes)
} __attribute__((aligned(64))) AXI_DMA_Descriptor;

// Kích thước của 1 chu kỳ FFT (giả định)
#define FFT_FRAME_LEN_BYTES FFT_PACKET_SIZE // (4096 * sizeof(int32_t) * 2) // Ví dụ: 4096 điểm FFT complex (I/Q) * 4 bytes/word * 2 (complex)

// Khai báo con trỏ DMA (giả định đã được lấy từ BRAM_Get_Virt_Addr)
//static volatile uint32_t *FFT_DMA_Reg = NULL;
//static AXI_DMA_Descriptor *FFT_Desc_Ring = NULL;
//static uint8_t *FFT_Data_Buf = NULL;

// Các hàm giả định
extern void hal_write(volatile uint32_t *addr, uint32_t value);
extern uint32_t hal_read(volatile uint32_t *addr);
// extern void hal_dcache_flush(void *addr, size_t len);      // Cần thiết cho Descriptor
// extern void hal_dcache_invalidate(void *addr, size_t len); // Cần thiết cho Data Buffer

// Số lượng Descriptor trong Ring Buffer
#define FFT_NUM_DESCRIPTORS 16 

// Biến trạng thái để theo dõi Descriptor đang được CPU xử lý
static int current_desc_idx = 0;

// ADC (Simple Mode)
int DMA_ADC_Read_Once(uint64_t phys_addr, uint32_t length);

// Workers
void *ADC_Worker_Thread(void *arg);
void *FFT_Monitor_SG_S2MM_Thread(void *arg);

#endif // DMA_DATA_H