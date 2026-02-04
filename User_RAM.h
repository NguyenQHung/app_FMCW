// #include "xparameters.h"
#include "linux_hal.h" // Lớp trừu tượng phần cứng (sysfs GPIO)

// Con trỏ toàn cục (extern) để lưu địa chỉ ảo cơ sở.
// Nó sẽ được định nghĩa trong file linux_hal.c
extern void *pl_virt_base;
// Địa chỉ vật lý bắt đầu của vùng PL (nơi chứa các BRAM)
// #define PL_PHYS_BASE_ADDR   MAP_BASE_ADDR
// // Kích thước vùng nhớ cần ánh xạ. 64MB là đủ rộng cho hầu hết thiết kế.
// #define PL_MAP_SIZE        MAP_SIZE

// // Macro để chuyển đổi địa chỉ vật lý sang địa chỉ ảo
// #define PHYS_TO_VIRT(phys_addr) (pl_virt_base + ((phys_addr) -
// PL_PHYS_BASE_ADDR))

#define TEST_ROUNDS 1        /* Number of loops that the Dma transfers run.*/
#define DMA_LENGTH 1305      /* Length of the Dma Transfers */
#define TIMEOUT_LIMIT 0x2000 /* Loop count for timeout */

#define INTC_DEVICE_ID                                                         \
  0 // XPAR_SCUGIC_0_DEVICE_ID	/* AXI_Read_Enable_GPIO IRQ ID */
// #define ENDFRAME_INTERRRUPT_ID
// XPAR_FABRIC_ENDFRAME_IRQ_IP2INTC_IRPT_INTR #define ENDFRAME2_INTERRRUPT_ID
// XPAR_FABRIC_ENDFRAME_IRQ1_IP2INTC_IRPT_INTR #define IP_ENDFRAME_INTERRRUPT_ID
// 124U
// //#define Uart2_INTERRRUPT_ID
// XPAR_FABRIC_AXI_UARTLITE_2_INTERRUPT_INTR #define Uart3_INTERRRUPT_ID
// XPAR_FABRIC_AXI_UARTLITE_3_INTERRUPT_INTR #define Uart4_INTERRRUPT_ID
// XPAR_FABRIC_AXI_UARTLITE_4_INTERRUPT_INTR #define CAN_INTR_VEC_ID
// XPAR_XCANPS_0_INTR #define CAN2_INTR_VEC_ID XPAR_XCANPS_1_INTR

// =============================================================================
// ĐỊNH NGHĨA ĐỊA CHỈ BRAM
// =============================================================================

#define Video_Pulse16_Addr 0  // 0xA0180000
#define Video_Code162_Addr 1  // 0xA01A0000
#define Video_Code163_Addr 2  // 0xA0190000
#define Video_Pulse_Addr 3    // 0xA0220000
#define Video_Code_Addr 4     // 0xA0160000
#define Video_Phase_I1_Addr 5 // 0xA01D0000
#define Video_Phase_Q1_Addr 6 // 0xA01E0000
#define Video_Phase_I2_Addr 7 // 0xA01F0000
#define Video_Phase_Q2_Addr 8 // 0xA0200000
#define Video_CS_fb17_Addr 9  // 0xA0233000
#define Video_fb17_Addr 10    // 0xA0170000
#define Video_fb1_16_Addr 11  // 0xA0210000
#define Video_b17S_Addr 12    // 0xA01B0000
#define Video_fb17i_Addr 13   // 0xA0136000
#define Video_fb17q_Addr 14   // 0xA0137000
#define DMA_ADC_Addr 15       // 0xA0240000, 4096, NULL},
#define DMA_CMAC_Addr 16      // 0xA0280000, 4096, NULL},
#define DMA_FFT_Addr 17       // 0xA01C0000, 4096, NULL},
#define CMAC_REG_Addr 18      // 0xA0270000, 4096, NULL},
#define BUF_ADC_RAM 19        //",  0x40000000, 0x10000000, NULL}, // 256MB
#define BUF_FFT_RAM 20        //",  0x50000000, 0x10000000, NULL}, // 256MB
#define BUF_CMAC_RAM 21       //", 0x60000000, 0x10000000, NULL}, // 256MB
#define DMA_FFT_DESC_Addr                                                      \
  22 // 0x72000000, 4096, NULL} // Vùng RAM cho Descriptor

#define UART0_ID 0       // XPAR_AXI_UARTLITE_0_DEVICE_ID
#define UART1_ID 1       // XPAR_AXI_UARTLITE_1_DEVICE_ID
#define UART2_ID 2       // XPAR_AXI_UARTLITE_2_DEVICE_ID
#define UART3_ID 3       // XPAR_AXI_UARTLITE_3_DEVICE_ID
#define UART4_ID 4       // XPAR_AXI_UARTLITE_4_DEVICE_ID
#define CAN_DEVICE_ID 0  // XPAR_XCANPS_0_DEVICE_ID
#define CAN2_DEVICE_ID 1 // XPAR_XCANPS_1_DEVICE_ID

#define GPIO_HS1_ID 0
#define GPIO_HS2_ID 1
#define GPIO_HS3_ID 2
#define ENDFRAME_IRQ_PORT_ID 3
#define ENDFRAME_IRQ1_PORT_ID 4
#define GPIO_SYNC_CMAC_ID 5

#define GPIO_PS_DEVICE_ID 0 // XPAR_XGPIOPS_0_DEVICE_ID
