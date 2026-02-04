#include "linux_hal.h"
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include "dma-proxy.h"
//static int mem_fd = -1;
extern  int trigger_adc; // Cờ báo hiệu cần đọc ADC
extern  int trigger_FFT; // Cờ báo hiệu cần đọc ADC
extern  int trigger_idle;
extern  XUartLite 			Uart0;					/* Instance of the UartLite Device */
extern  XUartLite 			Uart1;					/* Instance of the UartLite Device */
extern  XUartLite 			Uart2;					/* Instance of the UartLite Device */
extern  XUartLite 			Uart3;					/* Instance of the UartLite Device */
extern  XUartLite 			Uart4;					/* Instance of the UartLite Device */
extern  LinuxSpiDevice    	Pl_Spi0;
extern  LinuxSpiDevice     	Pl_Spi1;
extern  LinuxSpiDevice     	Pl_Spi2;
extern  LinuxSpiDevice     	Pl_Spi3;
extern  LinuxSpiDevice     	Pl_Spi4;
// Khai báo các Instance (phải khớp với khai báo trong main.h/User_VCO_Pin.h)
extern XGpioPs psgpio;
extern XGpio GPIO_ENDFRAME_IRQ;
extern XGpio GPIO_ENDFRAME_IRQ1;
extern XGpio GPIO_HS1;
extern XGpio GPIO_HS2;
extern XGpio GPIO_HS3;
extern XGpio GPIO_SYNC_CMAC;
// extern int trigger_adc;

uint32_t ADDR_CMAC_REG  = 0xA0000000; //     
uint32_t ADDR_DMA_CMAC  = 0xA0000000; //      
uint32_t ADDR_DMA_ADC   = 0xA0000000;  //       
uint32_t ADDR_DMA_FFT   = 0xA0000000;  //      

uint32_t PHYS_ADC_BUF   = 0xA0000000; //      
uint32_t PHYS_FFT_DESC  = 0xA0000000;//  
uint32_t PHYS_FFT_BUF   = 0xA0000000; //      
// =============================================================================
// PHẦN 1: hal_init
// =============================================================================
#define BRAM_BASE_PHYS_ADDR 0xA0000000
// Kích thước 4MB (0x00400000) đủ bao phủ dải 0xA0000000 -> 0xA0310000
#define BRAM_TOTAL_MAP_SIZE  0x00400000

static int mem_fd = -1;
// static void* global_bram_map_base = NULL;
//  Con trỏ toàn cục để lưu địa chỉ ảo đã được map
// void* pl_virt_base = NULL;
//  =============================================================================
//  PHẦN 1: ĐỊNH NGHĨA VÀ ÁNH XẠ THỰC TẾ
//  =============================================================================
static volatile void *mem_map_base = NULL;
struct dma_buf_descriptor dma_buf_list[] = {
    [IDX_DMA_ADC]  = {"/dev/dma_proxy_adc", NULL, 1, 4194304, DMA_PROXY_DEV_TO_MEM, -1},
};
// Định nghĩa lại danh sách vùng nhớ
BRAM_Region bram_list[] = {
    // --- CÁC KHỐI BRAM TRÊN PL (0xA0xxxxxx) ---
    [IDX_PULSE16]   = {"Pulse16",   0xA0250000, 65536, NULL},   // axi_BRAM_ctrl_0
    [IDX_CODE162]   = {"Code162",   0xA0260000, 32768, NULL},   // axi_BRAM_ctrl_5
    [IDX_CODE163]   = {"Code163",   0xA0270000, 65536, NULL},   // axi_BRAM_ctrl_6
    [IDX_PULSE]     = {"Pulse",     0xA0280000, 32768, NULL},   // axi_BRAM_ctrl_7
    [IDX_CODE]      = {"Code",      0xA0290000, 65536, NULL},   // axi_BRAM_ctrl_8
    [IDX_PHASE_I1]  = {"Phase_I1",  0xA02A0000, 65536, NULL},   // axi_BRAM_ctrl_9
    [IDX_PHASE_Q1]  = {"Phase_Q1",  0xA02B0000, 65536, NULL},   // axi_BRAM_ctrl_10
    [IDX_PHASE_I2]  = {"Phase_I2",  0xA02C0000, 65536, NULL},   // axi_BRAM_ctrl_11
    [IDX_PHASE_Q2]  = {"Phase_Q2",  0xA02D0000, 65536, NULL},   // axi_BRAM_ctrl_12
    [IDX_CS_FB17]   = {"CS_fb17",   0xA02E0000, 32768, NULL},   // axi_BRAM_ctrl_13
    [IDX_FB17]      = {"fb17",      0xA02F0000, 4096, NULL},    // axi_BRAM_ctrl_14
    [IDX_FB1_16]    = {"fb1_16",    0xA0300000, 65536, NULL},   // axi_BRAM_ctrl_15
    [IDX_B17S]      = {"b17S",      0xA0310000, 65536, NULL},   // axi_BRAM_ctrl_16
    [IDX_FB17I]     = {"fb17i",     0xA0000000, 4096, NULL},    // axi_BRAM2_ctrl_16
    [IDX_FB17Q]     = {"fb17q",     0xA0010000, 4096, NULL},    // axi_BRAM2_ctrl_17

    // --- THANH GHI ĐIỀU KHIỂN (REGISTERS) ---
    // [IDX_DMA_ADC]   = {"REG_DMA_ADC",  0xA0240000, 65536, NULL},
    // [IDX_DMA_CMAC]  = {"REG_DMA_CMAC", 0xA0280000, 65536, NULL},
    // [IDX_DMA_FFT]   = {"REG_DMA_FFT",  0xA01C0000, 65536, NULL},
    // [IDX_CMAC_REG]  = {"REG_CMAC",     0xA0270000, 65536, NULL},

    // // --- VÙNG RAM CHO ADC (256MB) ---
    // [IDX_ADC_BUF]   = {"BUF_ADC_RAM",  0x40000000, 0x10000000, NULL}, 

    // // --- VÙNG RAM CHO FFT (256MB) ---
    // [IDX_DMA_FFT_DESC] = {"FFT_DESC",  0x50000000, 4096,       NULL}, // Descriptor
    // [IDX_FFT_BUF]      = {"BUF_FFT_RAM", 0x50001000, 0x0FFF0000, NULL}, // Data (phần còn lại)

    // // --- VÙNG RAM CHO CMAC (Chia đôi 256MB thành TX và RX) ---
    // [IDX_CMAC_TX_BUF]  = {"BUF_CMAC_TX", 0x60000000, 0x08000000, NULL}, // 128MB đầu cho TX
    // [IDX_CMAC_RX_BUF]  = {"BUF_CMAC_RX", 0x68000000, 0x08000000, NULL}, // 128MB sau cho RX
};
int trigger_dma(int hal_index){

    if(hal_index < 0 || hal_index >= HAL_TOTAL_COUNT)
        return -100;
    int fd = dma_buf_list[hal_index].fd;
    return ioctl(fd, XFER);

  xil_printf("ADDR_CMAC_REG: 0x%X \n", ADDR_CMAC_REG);
  xil_printf("ADDR_DMA_CMAC: 0x%X \n", ADDR_DMA_CMAC);
  xil_printf("ADDR_DMA_ADC: 0x%X \n", ADDR_DMA_ADC);
  xil_printf("ADDR_DMA_FFT: 0x%X \n", ADDR_DMA_FFT);
  xil_printf("PHYS_ADC_BUF: 0x%X \n", PHYS_ADC_BUF);
  xil_printf("PHYS_FFT_DESC: 0x%X \n", PHYS_FFT_DESC);
  xil_printf("PHYS_FFT_BUF: 0x%X \n", PHYS_FFT_BUF);

}
static void dma_buffer_init()
{
    //init dma buffer for adc

    struct dma_buf_info sg_info;
    int ret;
    for(int i = 0 ; i < HAL_TOTAL_COUNT; i++)
    {
        if(dma_buf_list[i].dma_name == NULL)
            continue;
        int fd = open(dma_buf_list[i].dma_name, O_RDWR);
        if(fd < 0)
            perror("Failed to open device\n");
        else{
            dma_buf_list[i].fd = fd;
            sg_info.size = dma_buf_list[i].size;
            sg_info.numsg = dma_buf_list[i].numbuf;
            sg_info.direction = dma_buf_list[i].direction;
            ret = ioctl(fd, ALLOC_SG, &sg_info);
            if(ret < 0)
            {
                perror("Failed to allocate buffer");
                close(fd);
                dma_buf_list[i].fd = -1;
            }else
            {
                dma_buf_list[i].buffer = (uint8_t**)malloc(sizeof(uint8_t*)*dma_buf_list[i].numbuf);
                if(dma_buf_list[i].buffer == NULL)
                {
                    printf("Cannot allocate buffer");
                    close(fd);
                    dma_buf_list[i].fd = -1;
                    continue;
                }
                for(int k = 0; i < dma_buf_list[i].numbuf; k++)
                {
                    dma_buf_list[i].buffer[k] = mmap(NULL, dma_buf_list[i].size, PROT_READ | PROT_WRITE, MAP_SHARED, fd,0);
                    if(dma_buf_list[i].buffer[k] == NULL)
                    {
                        printf("failed to allocate buffer %d order\n",k);
                    }
                }
            }
        }
    }


}
int hal_init() 
{
    BRAM_Manager_Init();
    dma_buffer_init();

    // Địa chỉ Base vật lý từ Vivado
    // ADDR_CMAC_REG  =    BRAM_Get_Phys_Addr(IDX_CMAC_REG);
    // ADDR_DMA_CMAC  =    BRAM_Get_Phys_Addr(IDX_DMA_CMAC);
    // ADDR_DMA_ADC   =    BRAM_Get_Phys_Addr(IDX_DMA_ADC);
    // ADDR_DMA_FFT   =    BRAM_Get_Phys_Addr(IDX_DMA_FFT);

    // // Các vùng RAM Reserved (Vật lý)
    // PHYS_ADC_BUF   =    BRAM_Get_Phys_Addr(IDX_ADC_BUF);
    // PHYS_FFT_DESC  =    BRAM_Get_Phys_Addr(IDX_DMA_FFT_DESC); // Đặt Descriptor tại đầu vùng FFT RAM
    // PHYS_FFT_BUF   =    BRAM_Get_Phys_Addr(IDX_FFT_BUF);
    // PHYS_CMAC_BUF  =    BRAM_Get_Phys_Addr(IDX_CMAC_TX_BUF);

    // xil_printf("ADDR_CMAC_REG: 0x%X \n", ADDR_CMAC_REG);
    // xil_printf("ADDR_DMA_CMAC: 0x%X \n", ADDR_DMA_CMAC);
    // xil_printf("ADDR_DMA_ADC: 0x%X \n", ADDR_DMA_ADC);
    // xil_printf("ADDR_DMA_FFT: 0x%X \n", ADDR_DMA_FFT);
    // xil_printf("PHYS_ADC_BUF: 0x%X \n", PHYS_ADC_BUF);
    // xil_printf("PHYS_FFT_DESC: 0x%X \n", PHYS_FFT_DESC);
    // xil_printf("PHYS_FFT_BUF: 0x%X \n", PHYS_FFT_BUF);
    // xil_printf("PHYS_CMAC_BUF: 0x%X \n", PHYS_CMAC_BUF);

    // xil_printf("[HAL] Successfully mapped physical address 0x%X -> virtual memory.\n", MAP_BASE_ADDR);
    return 0;
}

void hal_cleanup() {
  xil_printf("[HAL] Cleaning up memory map...\n");
  BRAM_Manager_Close();
  // ... phần cleanup cho GPIO sysfs (nếu có) ...
  // Đừng quên gọi hàm dọn dẹp trước khi thoát
  XGpio_CleanupAll();
}

int BRAM_Manager_Init() {
  // 1. Mở file descriptor cho truy cập NON-CACHED (BRAM/Registers/Descriptors)
  int fd_sync = open("/dev/mem", O_RDWR | O_SYNC); // Non-Cached
  
  // 2. Mở file descriptor cho truy cập CACHED (DDR Data Buffers)
  int fd_cache = open("/dev/mem", O_RDWR); // Cached

  // Đóng file descriptor không cần thiết
  // int mem_fd = open("/dev/mem", O_RDWR | O_SYNC); // <--- Xóa dòng này, đã dùng fd_sync

  if (fd_sync < 0 || fd_cache < 0) {
    perror("[HAL ERROR] Failed to open /dev/mem");
    if (fd_sync >= 0) close(fd_sync);
    if (fd_cache >= 0) close(fd_cache);
    return -1;
  }

  // --- 1. Ánh xạ vùng Registers/BRAM (0xA0xxxxxx) ---
  // Map dải 0xA0000000 dùng fd_sync (Non-cached)
  mem_map_base = mmap(NULL, BRAM_TOTAL_MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
                      fd_sync, BRAM_BASE_PHYS_ADDR); // Sử dụng hằng số đã định nghĩa

  if (mem_map_base == MAP_FAILED) {
    perror("[HAL ERROR] Failed to mmap BRAM/Registers");
    close(fd_sync);
    close(fd_cache);
    return -1;
  }

  // --- 2. Lặp qua danh sách và ánh xạ từng vùng ---
  for (int i = 0; i < HAL_TOTAL_COUNT; i++) {
    if (bram_list[i].phys_addr == 0)
      continue;

    // --- A. Vùng BRAM/Registers (0xA0xxxxxx) ---
    if (bram_list[i].phys_addr >= BRAM_BASE_PHYS_ADDR) {
      // Các vùng BRAM dùng fd_sync (Non-cached), tính offset từ base map
      uint32_t offset = bram_list[i].phys_addr - BRAM_BASE_PHYS_ADDR;
      bram_list[i].virt_addr = (uint8_t *)mem_map_base + offset;
      
      // Kiểm tra ranh giới
      if (bram_list[i].phys_addr + bram_list[i].size > BRAM_BASE_PHYS_ADDR + BRAM_TOTAL_MAP_SIZE) {
          xil_printf("[HAL WARNING] BRAM/REG index %d outside mapped region!\n", i);
      }
    } 
    // --- B. Vùng DDR RAM (0x7Cxxxxxx/0x7Exxxxx) ---
    else if (bram_list[i].phys_addr >= 0x7C000000 && bram_list[i].phys_addr < 0x80000000) {
      
      if (i == IDX_DMA_FFT_DESC) {
        // Descriptor (0x7E000000): BẮT BUỘC NON-CACHED (dùng fd_sync)
        bram_list[i].virt_addr =
            mmap(NULL, bram_list[i].size, PROT_READ | PROT_WRITE, MAP_SHARED,
                 fd_sync, bram_list[i].phys_addr);
      } else {
        // Data Buffers (ADC/FFT): NÊN CACHED (dùng fd_cache)
        bram_list[i].virt_addr =
            mmap(NULL, bram_list[i].size, PROT_READ | PROT_WRITE, MAP_SHARED,
                 fd_cache, bram_list[i].phys_addr);
      }
      
      // Xử lý lỗi mmap cho các vùng DDR riêng biệt
      if (bram_list[i].virt_addr == MAP_FAILED) {
        perror("[HAL ERROR] Failed to mmap DDR RAM region");
        // Có thể chọn thoát hoặc tiếp tục, tạm thời in lỗi và tiếp tục
      }
    }
    // else { 
    //   // Xử lý các vùng nhớ khác (nếu có)
    // }
  }

  // Chú ý: Cần lưu các fd_sync và fd_cache để gọi close() trong hal_cleanup()
  // và cần lưu con trỏ map_base cho munmap(). Bạn cần khai báo các biến static
  // tương ứng để lưu giữ chúng.

  return 0;
}

// Sửa lại hàm này: nhận vào Index của vùng muốn đọc
int BRAM_Read_To_Buffer(void *dest_buffer, int region_index, size_t size) {
  void *src_virt_addr = BRAM_Get_Virt_Addr(region_index);

  if (src_virt_addr == NULL) {
    printf("Loi: Index %d chua duoc map!\n", region_index);
    return -1;
  }

  // Bảo vệ chống tràn vùng nhớ
  size_t max_size = BRAM_Get_Size(region_index);
  if (size > max_size)
    size = max_size;

  memcpy(dest_buffer, src_virt_addr, size);
  return 0;
}

// Giữ lại tên cũ nếu bạn đang dùng ở các file khác, nhưng sửa logic
int Linux_memcpy(void *dest_buffer, uint32_t region_index, size_t size) {
  return BRAM_Read_To_Buffer(dest_buffer, (int)region_index, size);
}

void BRAM_Manager_Close() {
  // 1. Unmap các vùng DDR/Descriptors đã được mmap riêng lẻ
  for (int i = 0; i < HAL_TOTAL_COUNT; i++) {
    // Chỉ unmap các vùng không thuộc dải 0xA0xxxxxx
    if (bram_list[i].virt_addr != NULL &&
        bram_list[i].phys_addr < BRAM_BASE_PHYS_ADDR) {
      // Lấy size chính xác từ danh sách BRAM_Region
      munmap(bram_list[i].virt_addr, bram_list[i].size);
      bram_list[i].virt_addr = NULL;
    }
  }

  // 2. Unmap vùng 0xA0xxxxxx (BRAM/Regs)
  if (mem_map_base && mem_map_base != MAP_FAILED) {
    munmap((void *)mem_map_base, BRAM_TOTAL_MAP_SIZE);
    mem_map_base = NULL;
  }

  // 3. Đóng tất cả File Descriptors (Nếu bạn đã refactor chúng thành toàn cục)
  // Hiện tại: Bạn chỉ đóng mem_fd (FD thứ 3 được mở)
  if (mem_fd >= 0) {
    close(mem_fd);
    mem_fd = -1;
  }
}

void *BRAM_Get_Virt_Addr(int index) {
  if (index < 0 || index >= HAL_TOTAL_COUNT)
    return NULL;
  return bram_list[index].virt_addr;
}
void *DMA_Get_Buffer_Addr(int hal_index, int buffer_index)
{
    if(hal_index < 0 || hal_index >= HAL_TOTAL_COUNT) return NULL;
    if(buffer_index < 0 || buffer_index >= dma_buf_list[hal_index].numbuf) return NULL;
    return dma_buf_list[hal_index].buffer[buffer_index]; 
}
uint32_t BRAM_Get_Phys_Addr(int index) {
  if (index < 0 || index >= HAL_TOTAL_COUNT)
    return NULL;
  return bram_list[index].phys_addr;
}

size_t BRAM_Get_Size(int index) {
  if (index < 0 || index >= HAL_TOTAL_COUNT)
    return 0;
  return bram_list[index].size;
}

void Xil_Out32(u32 PhysAddr, u32 Value) {
  if (!mem_map_base)
    return;

  // Kiểm tra xem địa chỉ có nằm trong vùng đã map không (tùy chọn nhưng an
  // toàn)
  if (PhysAddr < MAP_BASE_ADDR || PhysAddr >= (MAP_BASE_ADDR + MAP_SIZE)) {
    xil_printf("[HAL ERROR] Xil_Out32: Address 0x%X is out of mapped range!\n",
               PhysAddr);
    return;
  }

  // Tính toán offset so với địa chỉ base đã map và cộng vào con trỏ ảo
  *(volatile u32 *)(mem_map_base + (PhysAddr - MAP_BASE_ADDR)) = Value;
}

u32 Xil_In32(u32 PhysAddr) {
  if (!mem_map_base)
    return 0;

  // Kiểm tra xem địa chỉ có nằm trong vùng đã map không (tùy chọn nhưng an
  // toàn)
  if (PhysAddr < MAP_BASE_ADDR || PhysAddr >= (MAP_BASE_ADDR + MAP_SIZE)) {
    xil_printf("[HAL ERROR] Xil_In32: Address 0x%X is out of mapped range!\n",
               PhysAddr);
    return 0;
  }

  // Tính toán offset so với địa chỉ base đã map và cộng vào con trỏ ảo
  return *(volatile u32 *)(mem_map_base + (PhysAddr - MAP_BASE_ADDR));
}

void xil_printf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  fflush(stdout);
}
