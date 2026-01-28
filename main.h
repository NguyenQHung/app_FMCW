/*
 * main.h
 *
 *  Created on: Sep 17, 2017
 *      Author: johnmcd
 */

#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_

/***************************** Include Files ********************************/
// --- CÁC THƯ VIỆN HỆ THỐNG CỦA LINUX ---
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>  // Thêm cho uint8_t, uint32_t, v.v. (thay xil_types.h)
#include <unistd.h>
#include <sys/socket.h>
#include <math.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h> // Cần thiết cho việc nhận dữ liệu không blocking
#include <pthread.h>  // Nếu cần thread cho network/processing

// --- CÁC FILE HEADER CỦA DỰ ÁN ---
#include "linux_hal.h"      // Lớp trừu tượng phần cứng (sysfs GPIO)

// Comment các include Xilinx chưa port (thay bằng driver Linux nếu có)
// #include "xil_types.h"  // Đã thay bằng stdint.h
 #include "xrfdc.h"      // Comment: Cần driver Linux cho RFDC (ví dụ: libiio)
 #include "xrfclk.h"     // Comment: Clock config qua sysfs hoặc devicetree

//#include "LMK_display.h"    // Giữ nếu không phụ thuộc Xilinx (kiểm tra code)
//#include "LMX_display.h"  // Giữ tạm, nhưng có thể cần port
//#include "LMX2820.h"      // Comment nếu dùng SPI Xilinx
#include <complex.h>        // Giữ cho complex types

#define Data_length					62000 //15460
#define Config_length				50
#define TEST_BUFFER_SIZE 			16

#include "User_Device.h" 		// cac device: GPIO, DMA, SPI
#include "User_Struct.h"		// cac struct du lieu: UDP, PC8..
#include "User_Hyper_IO.h"		// GPIO sieu mo rong
#include "User_RAM.h"			// khai bao RAM luu tru
#include "User_Buffer.h"		// khai bao cac Array_1D[] luu tru
#include "User_VCO_Pin.h"		// khai bao du lieu cho VCO
#include "User_Ethenet_Data.h"	// khia bao du lieu cho Ethenet

extern  struct metal_device *device;
extern  struct metal_io_region *io;

// Khai báo socket
extern  UdpConnection udp_conn;
extern  ssize_t bytes_sent;
extern  pthread_mutex_t radar_data_mutex; // Khai báo mutex toàn cục
extern  int trigger_adc; // Cờ báo hiệu cần đọc ADC
extern  int trigger_idle;
extern  int DMA_cmd;
extern  int DMA_cnt;

extern  LinuxSpiDevice  Pl_Spi0;
extern  LinuxSpiDevice  Pl_Spi1;
extern  LinuxSpiDevice  Pl_Spi2;
extern  LinuxSpiDevice  Pl_Spi3;
extern  LinuxSpiDevice  Pl_Spi4;

extern   XGpioPs  		psgpio;
extern   XGpio  		GPIO_ENDFRAME_IRQ;		//	XPAR_ENDFRAME_IRQ_DEVICE_ID
extern   XGpio  		GPIO_ENDFRAME_IRQ1;		//	XPAR_ENDFRAME_IRQ_DEVICE_ID
extern   XGpio  		GPIO_HS1;				//	GPIO_HS1
extern   XGpio  		GPIO_HS2;				//	GPIO_HS2
extern   XGpio  		GPIO_HS3;				//	GPIO_HS3
extern   XGpio  		GPIO_SYNC_CMAC;
extern   XGpio          GPIO_TX_PREAMBLEIN;
extern   XGpio          GPIO_RX_PREAMBLEOUT;

extern   XUartLite 		Uart0;					/* Instance of the UartLite Device */
extern   XUartLite 		Uart1;					/* Instance of the UartLite Device */
extern   XUartLite 		Uart2;					/* Instance of the UartLite Device */
extern   XUartLite 		Uart3;					/* Instance of the UartLite Device */
extern   XUartLite 		Uart4;					/* Instance of the UartLite Device */

extern   LinuxCanBus Erob_Can0;
extern   LinuxCanBus Erob_Can1;

void hal_FFT_IRQ();
// extern  struct can_frame frame; // Buffer để nhận một CAN frame
// extern  struct pollfd fds[2];   // Mảng để theo dõi 2 socket: can0 và can1

#endif /* SRC_MAIN_H_ */
