/*
 * Radar_HW.c
 *
 *  Created on: Jun 30, 2020
 *      Author: TienNam
 */
#include "Radar_HW.h"
#include "RF_SOC.h"
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>

extern struct Radar_Struct Radar;
extern struct Frame_Struct Frame;
extern struct PC8_def PC8;
extern struct PC8_def PC8s;
extern struct AFC_Struct AFC1;
extern struct Radar_Debug_Struct Radar_Debug;
extern struct AD9957_config_def AD9957_config;
extern struct AD9957_config_def AD9957_config0;

extern u8 SendBuffer0[TEST_BUFFER_SIZE]; /* Buffer for Transmitting Data */
extern u8 RecvBuffer0[TEST_BUFFER_SIZE]; /* Buffer for Receiving Data */

// Khởi tạo toàn bộ UART
void Radar_Innit_Uart(void) {
  // UART_Generic_Init(&Uart0, "/dev/ttyUL1", "Uart0"); // Mapping tùy theo Device Tree
  UART_Generic_Init(&Uart1, "/dev/ttyUL2", "Uart1");
  UART_Generic_Init(&Uart2, "/dev/ttyUL3", "Uart2");
  UART_Generic_Init(&Uart3, "/dev/ttyUL4", "Uart3");
  // UART_Generic_Init(&Uart4, "/dev/ttyUL5", "Uart4");
}

// --- CÁC HÀM GỬI FRAME TỐI ƯU ---
// Sử dụng write() trực tiếp giúp gửi cả khối dữ liệu cực nhanh

void UART0_Send_Frame(unsigned char Length) {
  // if (Uart0.fd >= 0)
  //   write(Uart0.fd, Radar.UART_Frame0, Length);
  //   else  printf("Uart0 fail\n");
}

void UART1_Send_Frame(unsigned char Length) {
  if (Uart1.fd >= 0)
    write(Uart1.fd, Radar.UART_Frame1, Length);
    else  printf("Uart1 fail\n");
}

void UART2_Send_Frame(unsigned char Length) {
  if (Uart2.fd >= 0)
    write(Uart2.fd, Radar.UART_Frame2, Length);
    else  printf("Uart2 fail\n");
}

void UART3_Send_Frame(unsigned char Length) {
  if (Uart3.fd >= 0)
    write(Uart3.fd, Radar.UART_Frame3, Length);
    else  printf("Uart3 fail\n");
}

void UART4_Send_Frame(unsigned char Length) {
  // if (Uart4.fd >= 0) write(Uart4.fd, Radar.UART_Frame4, Length);
}

// =============================================================================
//                   HÀM Radar_Innit_SPI VIẾT LẠI 100%
// =============================================================================
void Radar_Innit_SPI(void) {
  int all_spi_ok = 1;

  // Cấu hình chung cho các chip LMK, LMX (SPI_MODE_0, 10MHz)
  uint8_t lmk_lmx_mode = SPI_MODE_0;
  uint32_t lmk_lmx_speed = 10000000;

  // Cấu hình chung cho các chip ADF (SPI_MODE_0, 1MHz)
  uint8_t adf_mode = SPI_MODE_0;
  uint32_t adf_speed = 1000000;

  // --- Khởi tạo từng thiết bị SPI ---
  // Dựa trên bảng mapping đã xác định:
  // Pl_Spi0 -> LMK04832
  if (SPI_Init_Linux(&Pl_Spi0, "/dev/spidev1.0", lmk_lmx_mode, lmk_lmx_speed) !=
      XST_SUCCESS) {
    all_spi_ok = 0;
  }

  // Pl_Spi1 -> LMX2820 PLL1
  if (SPI_Init_Linux(&Pl_Spi1, "/dev/spidev2.0", lmk_lmx_mode, lmk_lmx_speed) !=
      XST_SUCCESS) {
    all_spi_ok = 0;
  }

  // Pl_Spi2 -> LMX2820 PLL2
  if (SPI_Init_Linux(&Pl_Spi2, "/dev/spidev3.0", lmk_lmx_mode, lmk_lmx_speed) !=
      XST_SUCCESS) {
    all_spi_ok = 0;
  }

  // Pl_Spi3 -> ADF4360
  if (SPI_Init_Linux(&Pl_Spi3, "/dev/spidev4.0", adf_mode, adf_speed) !=
      XST_SUCCESS) {
    all_spi_ok = 0;
  }

  // Pl_Spi4 -> ADF4159
  if (SPI_Init_Linux(&Pl_Spi4, "/dev/spidev5.0", adf_mode, adf_speed) !=
      XST_SUCCESS) {
    all_spi_ok = 0;
  }

  // --- Báo cáo kết quả ---
  if (all_spi_ok) {
    printf("All SPI devices initialized successfully.\n");
  } else {
    printf("Error: One or more SPI devices failed to initialize.\n");
    // Dọn dẹp các SPI đã mở nếu có lỗi
    SPI_Close_Linux(&Pl_Spi0);
    SPI_Close_Linux(&Pl_Spi1);
    SPI_Close_Linux(&Pl_Spi2);
    SPI_Close_Linux(&Pl_Spi3);
    SPI_Close_Linux(&Pl_Spi4);
  }
}

void Radar_Innit_GPIO(void) {
  int Status0;
  int Status;
  // XGpioPs_Config *ConfigPtr;

  // ConfigPtr = XGpioPs_LookupConfig(GPIO_PS_DEVICE_ID);
  Status0 = XGpioPs_CfgInitialize(&psgpio, NULL, 0); // ConfigPtr->BaseAddr);
  if (Status0 == XST_SUCCESS) {
    xil_printf("XGpioPs_Setup devices\n\r");

    XGpioPs_SetDirectionPin(&psgpio, LMK_RESET_IO, 1);
    XGpioPs_SetDirectionPin(&psgpio, LMK_CLKIN_SEL0_IO, 1);
    XGpioPs_SetDirectionPin(&psgpio, LMK_CLKIN_SEL1_IO, 1);

    XGpioPs_SetDirectionPin(&psgpio, SFP_LPMODE, 1);
    XGpioPs_SetDirectionPin(&psgpio, SFP_MODE_SEL, 1);
    XGpioPs_SetDirectionPin(&psgpio, SFP_RESET, 1);

    XGpioPs_SetDirectionPin(&psgpio, LMX_MUTE1_IO, 1);
    XGpioPs_SetDirectionPin(&psgpio, LMX_CE1_IO, 1);
    XGpioPs_SetDirectionPin(&psgpio, LMX_MUTE2_IO, 1);
    XGpioPs_SetDirectionPin(&psgpio, LMX_CE2_IO, 1);

    XGpioPs_SetDirectionPin(&psgpio, ADF4159_CE, 1);
    XGpioPs_SetDirectionPin(&psgpio, ADF4360_CE, 1);

    xil_printf("psgpio ok\r\n");
  }

  //   xil_printf("SFP_LPMODE = 0\r\n");
  //   XGpioPs_WritePin(&psgpio, SFP_LPMODE, 0);
  //   xil_printf("SFP_MODE_SEL = 0\r\n");
  //   XGpioPs_WritePin(&psgpio, SFP_MODE_SEL, 0);
  //   xil_printf("SFP_RESET = 1\r\n");
  //   XGpioPs_WritePin(&psgpio, SFP_RESET, 1);

  Status0 = XGpio_Initialize(&GPIO_ENDFRAME_IRQ, ENDFRAME_IRQ_PORT_ID);
  if (Status0 == XST_SUCCESS) {
    // cau hinh DIR
    XGpio_SetDataDirection(&GPIO_ENDFRAME_IRQ, 1, 0x3);
    xil_printf("GPIO_ENDFRAME_IRQ ok\r\n");
  }

  // do GPIO_ENDFRAME_IRQ1: đang được dùng để sử dụng ngắt, nên không được cấu
  // hình nữa Status0 = XGpio_Initialize(&GPIO_ENDFRAME_IRQ1,
  // ENDFRAME_IRQ1_PORT_ID); if (Status0 == XST_SUCCESS)
  // {
  // 	// cau hinh DIR
  // 	XGpio_SetDataDirection(&GPIO_ENDFRAME_IRQ1, 1, 0x1);
  // 	xil_printf("GPIO_ENDFRAME_IRQ1 ok\r\n");
  // }

  Status0 = XGpio_Initialize(&GPIO_HS1, GPIO_HS1_ID);
  if (Status0 == XST_SUCCESS) {
    // cau hinh DIR
    XGpio_SetDataDirection(&GPIO_HS1, 1, 0x000);
    XGpio_SetDataDirection(&GPIO_HS1, 2, 0x00000000);
    xil_printf("GPIO_HS1 ok\r\n");
  }

  Status0 = XGpio_Initialize(&GPIO_HS2, GPIO_HS2_ID);
  if (Status0 == XST_SUCCESS) {
    // cau hinh DIR
    XGpio_SetDataDirection(&GPIO_HS2, 1, 0x000);
    XGpio_SetDataDirection(&GPIO_HS2, 2, 0xffffffff);
    xil_printf("GPIO_HS2 ok\r\n");
  }

  Status0 = XGpio_Initialize(&GPIO_HS3, GPIO_HS3_ID);
  if (Status0 == XST_SUCCESS) {
    // cau hinh DIR
    XGpio_SetDataDirection(&GPIO_HS3, 1, 0x00);
    XGpio_SetDataDirection(&GPIO_HS3, 2, 0xfff);
    xil_printf("GPIO_HS3 ok\r\n");
  }

  Status0 = XGpio_Initialize(&GPIO_SYNC_CMAC, GPIO_SYNC_CMAC_ID);
  if (Status0 == XST_SUCCESS) {
    // cau hinh DIR
    XGpio_SetDataDirection(&GPIO_SYNC_CMAC, 1, 0xFF); // dang su dung kenh 1
    //XGpio_SetDataDirection(&GPIO_SYNC_CMAC, 2, 0x00); // khong co kenh 2
    xil_printf("GPIO_SYNC_CMAC ok\r\n");

    //XGpio_DiscreteWrite(&GPIO_SYNC_CMAC, 2, 0);
  }
}

void Radar_Set_Z_encoder(unsigned char Val) {
  // XGpio_DiscreteWrite(&GPIO_ENC, 2, Val);
}

void Radar_Write_Uart0(unsigned char Data, unsigned char Data1,
                       unsigned char Data2, unsigned char Data3,
                       unsigned char Data4) {

  Radar.UART_Frame0[0] = Data;
  Radar.UART_Frame0[1] = Data1;
  Radar.UART_Frame0[2] = Data2;
  Radar.UART_Frame0[3] = Data3;
  Radar.UART_Frame0[4] = Data4;

  UART0_Send_Frame(5);

  // XUartLite_SendByte(XPAR_UARTLITE_0_BASEADDR, Data);
  // XUartLite_SendByte(XPAR_UARTLITE_0_BASEADDR, Data1);
  // XUartLite_SendByte(XPAR_UARTLITE_0_BASEADDR, Data2);
  // XUartLite_SendByte(XPAR_UARTLITE_0_BASEADDR, Data3);
  // XUartLite_SendByte(XPAR_UARTLITE_0_BASEADDR, Data4);
}
void Radar_Check_motor(void) {
  if (Radar.Beta_SW % 150 == 0) {
    PC8.Alpha_A1 = PC8.Alpha_A0;
    PC8.Alpha_A0 = PC8.Alpha_ENC;
    if (PC8.Alpha_A0 == PC8.Alpha_A1)
      PC8.Alpha_Stop = 1;
    else
      PC8.Alpha_Stop = 0;

    if (PC8.Beta_Calib == 0) {
      PC8.Beta_A1 = PC8.Beta_A0;
      PC8.Beta_A0 = PC8.Beta_ENC;
      if (PC8.Beta_A0 == PC8.Beta_A1)
        PC8.Beta_Stop = 1;
      else
        PC8.Beta_Stop = 0;
    } else if (PC8.Beta_Calib == 2) {
      if (PC8.Calib_counter < 10)
        PC8.Calib_counter++;

      PC8.Beta_A1 = PC8.Beta_A0;
      PC8.Beta_A0 = PC8.Beta_ENC;

      if (PC8.Calib_counter >= 5) {
        if (PC8.Beta_A0 == PC8.Beta_A1) {
          PC8.Beta_Stop = 1;
          PC8.Beta_Calib = 0;
        } else {
          PC8.Beta_Stop = 0;
        }
      }
    }

    if (PC8.Beta_Stop == 1) {
      if (PC8.Quay_Quat_Active == 1) {
        if ((PC8.Anten_start <= 9) || (PC8.Anten_start >= 3590)) {
          if ((PC8.Beta_Angle >= (3599 - PC8.Quat_Error)) ||
              (PC8.Beta_Angle <= PC8.Quat_Error)) {
            PC8.Quay_Quat_Active = 2;
            PC8.Auto_Rotate = 1;
          }
        } else {
          if ((PC8.Beta_Angle >= (PC8.Anten_start - PC8.Quat_Error)) &&
              (PC8.Beta_Angle <= (PC8.Anten_start + PC8.Quat_Error))) {
            PC8.Quay_Quat_Active = 2;
            PC8.Auto_Rotate = 1;
          }
        }
      } else if (PC8.Quay_Quat_Active == 2) {
        if ((PC8.Anten_stop <= 9) || (PC8.Anten_stop >= 3590)) {
          if ((PC8.Beta_Angle >= (3599 - PC8.Quat_Error)) ||
              (PC8.Beta_Angle <= PC8.Quat_Error)) {
            PC8.Quay_Quat_Active = 3;
            PC8.Auto_Rotate = 1;
          }
        } else {
          if ((PC8.Beta_Angle >= (PC8.Anten_stop - PC8.Quat_Error)) &&
              (PC8.Beta_Angle <= (PC8.Anten_stop + PC8.Quat_Error))) {
            PC8.Quay_Quat_Active = 3;
            PC8.Auto_Rotate = 1;
          }
        }
      } else if (PC8.Quay_Quat_Active == 3) {
        if ((PC8.Anten_start <= 9) || (PC8.Anten_start >= 3590)) {
          if ((PC8.Beta_Angle >= (3599 - PC8.Quat_Error)) ||
              (PC8.Beta_Angle <= PC8.Quat_Error)) {
            PC8.Quay_Quat_Active = 2;
            PC8.Auto_Rotate = 1;
          }
        } else {
          if ((PC8.Beta_Angle >= (PC8.Anten_start - PC8.Quat_Error)) &&
              (PC8.Beta_Angle <= (PC8.Anten_start + PC8.Quat_Error))) {
            PC8.Quay_Quat_Active = 2;
            PC8.Auto_Rotate = 1;
          }
        }
      }
    }
  }
}
