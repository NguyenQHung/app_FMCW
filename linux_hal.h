#ifndef LINUX_HAL_H
#define LINUX_HAL_H

// #include "CMAC_Hal.h" // thư viện CMA
#include "DMA_Data.h" // thư viện DMA
#include "DMA_Hal.h"  // thư viện DMA
#include "Hal_cache.h"
#include "SPI_Hal.h"     // thư viện SPI
#include "XGpioPS_Hal.h" // thư viện GPIO
#include "XUartPS_Hal.h" // thư viện XUartPS
#include "XUart_Hal.h"   // thư viện Uart
#include "xgpio_hal.h"   // thư viện XGPIO
#include "xstatus.h"
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <linux/spi/spidev.h>
#include <poll.h>    // Thêm thư viện này vào đầu file
#include <pthread.h> // thư viện xử lý đa luồng
#include <stdint.h>  // Thêm cho uint8_t, uint32_t, v.v. (thay xil_types.h)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include <stddef.h> // Cho size_t
#include <sys/ioctl.h>

#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h> // Cung cấp biến `errno` để lấy mã lỗi
#include <fcntl.h> // Cung cấp hàm open() và các cờ O_RDWR, O_NOCTTY, O_SYNC
#include <gpiod.h> // Có thể cần include ở đây
#include <math.h>
#include <string.h> // Cung cấp hàm strerror() để chuyển mã lỗi thành chuỗi
#include <sys/socket.h>
#include <termios.h> // Cung cấp các hàm và cấu trúc cho việc điều khiển terminal (tty)
#include <unistd.h> // Cung cấp các hàm POSIX API chuẩn như close(), read(), write()

// --- CÁC HẰNG SỐ CHO MEMORY MAPPING ---
#define MAP_BASE_ADDR 0xA0000000 // Địa chỉ vật lý thấp nhất cần truy cập
#define MAP_SIZE                                                               \
  0x00400000 // Kích thước 4 MB (lớn hơn 2.7MB một chút để an toàn)


// Địa chỉ Base vật lý từ Vivado
extern uint32_t ADDR_CMAC_REG; //     0xA0270000
extern uint32_t ADDR_DMA_CMAC; //     0xA0280000
extern uint32_t ADDR_DMA_ADC;  //      0xA0240000
extern uint32_t ADDR_DMA_FFT;  //     0xA01C0000

extern struct Radar_Struct Radar;
extern struct Radar_Debug_Struct Radar_Debug;
extern struct Frame_Struct Frame;
extern struct PC8_def PC8;
extern struct PC8_def PC8s;
extern struct AFC_Struct AFC1;
extern struct AIS_Struct AIS_Data;
extern struct Erob_Struct Erob_Motor;
extern struct Erob_Control_def Erob_Control;
extern struct AIS_Struct AIS_Data;
extern struct UDP_Struct UDP_Frame;
extern struct CAN_Control_def CAN_Controls;
extern struct UDP_Phase UDP_Phase1;
extern struct CMAC_Struct CMAC_Struct1;

extern uint32_t ADDR_CMAC_REG; //   = 0xA0000000; //     
extern uint32_t ADDR_DMA_CMAC; //   = 0xA0000000; //      
extern uint32_t ADDR_DMA_ADC; //    = 0xA0000000;  //       
extern uint32_t ADDR_DMA_FFT; //    = 0xA0000000;  //      

extern uint32_t PHYS_ADC_BUF; //    = 0xA0000000; //      
extern uint32_t PHYS_FFT_DESC; //   = 0xA0000000;//  
extern uint32_t PHYS_FFT_BUF; //    = 0xA0000000; //      

// Định nghĩa lại UINTPTR cho môi trường Linux
#ifndef UINTPTR
typedef uintptr_t UINTPTR;
#endif

// Định nghĩa lại các kiểu dữ liệu của Xilinx
typedef unsigned char u8_t;
typedef unsigned char u8;
typedef signed char s8_t;
typedef unsigned short u16_t;
typedef signed short s16_t;
typedef unsigned int u32_t;
typedef unsigned int u32;
typedef signed int s32_t;
typedef unsigned long long u64_t;
typedef signed long long s64_t;
typedef s8_t err_t;

// --- Cấu trúc UdpConnection của bạn ---
typedef struct {
  int sockfd;
  struct sockaddr_in client_addr; // Địa chỉ client để "trả lời"
  socklen_t client_len;
  int is_client_addr_valid;

  // Thêm các địa chỉ để "gửi chủ động"
  struct sockaddr_in remote_pc1_addr;
  struct sockaddr_in remote_pc2_addr;
} UdpConnection;

// --- Stub cho BRAM ---
typedef struct {
  uint32_t CtrlBaseAddress;
} XBram_Config;
typedef struct {
  int Dummy;
} XBram;
#define XBram_LookupConfig(Id) ((void *)1)
#define XBram_CfgInitialize(Inst, Cfg, Addr) XST_SUCCESS

// Cấu trúc quản lý từng vùng BRAM
typedef struct {
  const char *name;
  uint32_t phys_addr;
  size_t size;
  void *virt_addr; // Con trỏ ảo dùng để memcpy trực tiếp
} BRAM_Region;

struct dma_buf_descriptor{
    const char *dma_name;
    uint8_t **buffer;
    uint32_t numbuf;
    unsigned long size;
    uint32_t direction;
    int fd;
};
// Định nghĩa Index cho 15 vùng để gọi code cho dễ
enum {
  IDX_PULSE16 = 0,
  IDX_CODE162,
  IDX_CODE163,
  IDX_PULSE,
  IDX_CODE,
  IDX_PHASE_I1,
  IDX_PHASE_Q1,
  IDX_PHASE_I2,
  IDX_PHASE_Q2,
  IDX_CS_FB17,
  IDX_FB17,
  IDX_FB1_16,
  IDX_B17S,
  IDX_FB17I,
  IDX_FB17Q,
  IDX_DMA_ADC,    // Tự động là 15
  IDX_DMA_CMAC,   // Tự động là 16
  IDX_DMA_FFT,    // Tự động là 17
  IDX_CMAC_REG,   // Tự động là 18
  IDX_ADC_BUF,   // Tự động là 18
  IDX_DMA_FFT_DESC,   // Tự động là 18
  IDX_FFT_BUF,
  HAL_TOTAL_COUNT // Sẽ là 20
};

extern BRAM_Region bram_list[HAL_TOTAL_COUNT];

int AXI_DMA_Transfer_S2MM(int dma_idx, uint64_t phys_addr, uint32_t length);
int DMA_ADC_Trigger_Read(uint64_t dest_phys_addr);
int DMA_FFT_Read_Continuous(uint64_t dest_phys_addr);

// Khai báo hàm API
int BRAM_Manager_Init(void);
void BRAM_Manager_Close(void);
uint32_t BRAM_Get_Phys_Addr(int index);
void *DMA_Get_Buffer_Addr(int hal_index, int buffer_index);
int trigger_dma(int hal_index);
void* BRAM_Get_Virt_Addr(int index);
size_t BRAM_Get_Size(int index);
int Linux_memcpy(void *dest_buffer, uint32_t phys_addr, size_t size);

// --- Macro truy cập thanh ghi (Cẩn thận: Linux cần mmap) ---
void CMAC_Status_Monitor();
int hal_init();
int Linux_memcpy(void *dest_buffer, uint32_t phys_addr, size_t size);
void hal_cleanup();
void Xil_Out32(u32 PhysAddr, u32 Value);
u32 Xil_In32(u32 PhysAddr);

void xil_printf(const char *format, ...);
void hal_cleanup();

#endif // LINUX_HAL_H
