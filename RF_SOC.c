#include "RF_SOC.h"
#include "main.h"
#include <stdarg.h>
#include <stdio.h>

// #include "xparameters.h"
// #include "xil_types.h"
// #include "xil_io.h"
// #include "sleep.h"
// #include "xstatus.h"
// #include "xil_printf.h"
#include "LMK_display.h"
#include "LMX_display.h"
#include "xrfclk.h"
#include "xrfdc.h"

// #include <metal/log.h>
// #include <metal/sys.h>
#include "cli.h"

// #include <sys/ioctl.h>
// #include <linux/spi/spidev.h>

// #include "platform.h"
// #include "psgpio_emio.h"
// #include "xspi.h"		/* SPI device driver */
// #include "xspi_l.h"
#include "LMX2820.h"
#include "linux_hal.h"

#include "metal/device.h"
#include "metal/io.h"

// Includes for user added CLI functions used in this file
#include "cmd_func_mem.h"
#include "rfdc_cmd.h"
#include "rfdc_poweron_status.h"

// #include "adc_CalCoefficients.h"
#include "LMK04832.h"
#include "adcDisableCoeffOvrd.h"
#include "adcGetCalCoefficients.h"
#include "adcLoadCalCoefficients.h"
#include "adcSaveCalCoefficients.h"
#include "adc_FreezeCal.h"
#include "adc_LinkCoupling.h"
#include "dac_waves.h"
#include "rfdc_dsa_vop.h"
#include "rfdc_interpolation_decimation.h"
#include "rfdc_mts.h"
#include "rfdc_nyquistzone.h"
#include "uram_play_cap.h"

/**************************** Type Definitions *******************************/

// VT100 esc sequences
char CHAR_ATTRIB_OFF[5] = "\x1B[0m";
char BOLD_ON[5] = "\x1B[1m";
char UNDERLINE_ON[5] = "\x1B[4m";
char BLINK_ON[5] = "\x1B[5m";
char REVERSE_ON[5] = "\x1B[5m";

char CLR_SCREEN[5] = "\x1B[2J";

// data buffer used for reading PLL registers
static u32 data[256];

const char clkoutBrdNames[][18] = {
    "RFIN_RF1",   "RF1_ADC_SYNC", "NC",         "AMS_SYSREF",
    "RFIN_RF2",   "RF2_DAC_SYNC", "DAC_REFCLK", "DDR_PL_CAP_SYNC",
    "PL_CLK",     "PL_SYSREF",    "NC",         "J10 SINGLE END",
    "ADC_REFCLK", "NC",
};

#define DAC_Current 30000
#define ADC_ATT 0

/*****************************************************************************/
/**
 *
 * My libmetal logger
 * Intercepts log prints and adjusts \r\n prints to display the some on a uart
 * or through a jtagUart.
 *
 ******************************************************************************/

void my_metal_default_log_handler(enum metal_log_level level,
                                  const char *format, ...) {
  char msg[1024];
  char msgOut[1048];
  char *outPtr;
  int i;

  va_list args;
  static const char *level_strs[] = {
      "metal: emergency: ", "metal: alert:     ", "metal: critical:  ",
      "metal: error:     ", "metal: warning:   ", "metal: notice:    ",
      "metal: info:      ", "metal: debug:     ",
  };

  va_start(args, format);
  vsnprintf(msg, sizeof(msg), format, args);
  va_end(args);

  // replace single \n with \n\r
  outPtr = msgOut;
  for (i = 0; i < 1024; i++) {
    // if /n/r or /r/n combo
    if ((msg[i] == '\r' && msg[i + 1] == '\n') ||
        (msg[i] == '\n' && msg[i + 1] == '\r')) {
      *outPtr++ = msg[i++];
    } else if (msg[i] == '\n') {
      // if first char in string is \n, then remove
      if (i == 0) {
        continue;
      } else {
        *outPtr++ = '\r';
      }
    }
    *outPtr++ = msg[i];
    if (msg[i] == 0) {
      break;
    }
  }
  // if line doesn't end with \n\r, then add it
  if ((msg[i - 1] != '\n') && (msg[i - 1] != '\r')) {
    *(outPtr - 1) = '\r';
    *outPtr++ = '\n';
    *outPtr++ = 0;
  }

  if (level <= METAL_LOG_EMERGENCY || level > METAL_LOG_DEBUG)
    level = METAL_LOG_EMERGENCY;

  xil_printf("%s%s", level_strs[level], msgOut);
}

/****************************************************************************/
/**
 *
 * This function resets all CLK_104 PLL I2C devices.
 *
 * @param	None
 *
 * @return
 *	- XST_SUCCESS if successful.
 *	- XST_FAILURE if failed.
 *
 * @note		None
 *
 ****************************************************************************/
// static
int resetAllClk104(void) {
  int ret = EXIT_FAILURE;
  //	printf("Reset LMK\n\r");
  if (XST_FAILURE == XRFClk_ResetChip(RFCLK_LMK)) {
    printf("Failure in XRFClk_ResetChip(RFCLK_LMK)\n\r");
    return ret;
  }

  //	printf("Reset LMX2594_1\n\r");
  if (XST_FAILURE == XRFClk_ResetChip(RFCLK_LMX2594_1)) {
    printf("Failure in XRFClk_ResetChip(RFCLK_LMX2594_1)\n\r");
    return ret;
  }

  //	printf("Reset LMX2594_2\n\r");
  if (XST_FAILURE == XRFClk_ResetChip(RFCLK_LMX2594_2)) {
    printf("Failure in XRFClk_ResetChip(RFCLK_LMX2594_2)\n\r");
    return ret;
  }

#ifdef XPS_BOARD_ZCU111
  //	printf("Reset LMX2594_3\n\r");
  if (XST_FAILURE == XRFClk_ResetChip(RFCLK_LMX2594_3)) {
    printf("Failure in XRFClk_ResetChip(RFCLK_LMX2594_3)\n\r");
    return ret;
  }
#endif

  return EXIT_SUCCESS;
}

/****************************************************************************/
/**
 *
 * Print LMK PLL device settings such as input and output clk frequencies.
 * The instance structure is initialized by calling LMK_init()
 *
 * @param
 *	- lmkInstPtr a pointer to the LMK instance structure
 *
 * @return
 *	- void
 *
 * @note		None
 *
 ****************************************************************************/
void printLMKsettings(lmk_config_t *lmkInstPtr) {

#ifdef LMK_DEBUG
  LMK_intermediateDump(lmkInstPtr);
#endif

  // Print LMK CLKin frequencies
  if (lmkInstPtr->clkin_sel_mode == LMK_CLKin_SEL_MODE_AUTO_MODE) {
    xil_printf("CLKin Auto Mode Enabled\n\r");
  }
  for (int i = 0; i < 3; i++) {
    if (lmkInstPtr->clkin[i].freq != -1) {
      xil_printf("CLKin%d_freq: %12ldKHz\n\r", i,
                 lmkInstPtr->clkin[i].freq / 1000);
    }
  }

  // Print LMK CLKout frequencies
  for (int i = 0; i < 7; i++) {
    xil_printf("DCLKout%02d(%-10s):", i * 2, clkoutBrdNames[i * 2]);
    if (lmkInstPtr->clkout[i].dclk_freq == -1) {
      xil_printf("%12s", "-----");
    } else {
      xil_printf("%9ldKHz", lmkInstPtr->clkout[i].dclk_freq / 1000);
    }

    xil_printf(" SDCLKout%02d(%-15s):", i * 2 + 1, clkoutBrdNames[i * 2 + 1]);
    if (lmkInstPtr->clkout[i].sclk_freq == -1) {
      xil_printf("%12s\n\r", "-----");
    } else {
      xil_printf("%9ldKHz\n\r", lmkInstPtr->clkout[i].sclk_freq / 1000);
    }
  }
}

/****************************************************************************/
/**
 *
 * Print LMX PLL device output clk frequencies.
 * The instance structure is initialized by calling LMX_SettingsInit()
 *
 * @param
 * 	- clkin is the clk freq fed into the LMX PLL. This value is used to
 * 	  calculate and display the output frequencies
 *	- lmxInstPtr a pointer to the LMX instance structure
 *
 * @return
 *	- void
 *
 * @note		None
 *
 ****************************************************************************/
void printLMXsettings(long int clkin, lmx_config_t *lmxInstPtr) {

#ifdef LMX_DEBUG
  LMX_intermediateDump(lmxInstPtr);
#endif

  // Print LMX CLKin freq
  xil_printf("CLKin_freq: %10ldKHz\n\r", clkin / 1000);

  // Print LMX CLKout frequencies
  xil_printf("RFoutA Freq:");
  if (lmxInstPtr->RFoutA_freq == -1) {
    xil_printf("%13s\n\r", "-----");
  } else {
    xil_printf("%10ldKHz\n\r", lmxInstPtr->RFoutA_freq / 1000);
  }

  xil_printf("RFoutB Freq:");
  if (lmxInstPtr->RFoutB_freq == -1) {
    xil_printf("%13s\n\r", "-----");
  } else {
    xil_printf("%10ldKHz\n\r", lmxInstPtr->RFoutB_freq / 1000);
  }
}

/****************************************************************************/
/**
 *
 * Reads the configuration of LMK and LMX PLL then calculates and displays
 * the PLL frequencies and settings.
 * The instance structures ar initialized by calling LMK_init() or
 * LMX_SettingsInit()
 *
 * @param
 * 	- nil
 *
 * @return
 *	- void
 *
 * @note		None
 *
 ****************************************************************************/
void printCLK104_settings(void) {
  char pllNames[3][9] = {"LMK ----", "LMX_RF1", "LMX_RF2"};
  u32 chipIds[3] = {RFCLK_LMK, RFCLK_LMX2594_1, RFCLK_LMX2594_2};

  for (int i = 0; i < 3; i++) {
    if (XST_FAILURE == XRFClk_GetConfigFromOneChip(chipIds[i], data)) {
      printf("Failure in XRFClk_GetConfigFromOneChip()\n\r");
      return;
    }

    // For LMX, reverse readback data to match exported register sets and
    // order of LMX2594[][]
    if (chipIds[i] != RFCLK_LMK) {
      reverse32bArray(data, LMX2594_COUNT - 3);
    }

#if 0
		// Dump raw data read from device
		printf("Config data is:\n\r");
		for (int j = 0; j < ((chipIds[i]==RFCLK_LMK) ? LMK_COUNT : LMX2594_COUNT-3); j++) {
			printf("%08X, ", data[j]);
			if( !(j % 6) ) printf("\n\r");
		}
		printf("\n\r");
#endif

    // Display clock values of device
    printf("Clk settings read from %s ---------------------\n\r", pllNames[i]);
    if (chipIds[i] == RFCLK_LMK) {
      LMK_init(data, &lmkConfig);
      printLMKsettings(&lmkConfig);
    } else {
      // clkout index is i=1 idx = 0, i=2 idx=2. i&2 meets this alg
      LMX_SettingsInit(lmkConfig.clkout[(i & 2)].dclk_freq, data, &lmxConfig);
      printLMXsettings(lmkConfig.clkout[(i & 2)].dclk_freq, &lmxConfig);
    }
    xil_printf("\n\r");
  }
}

void reverse32bArray(u32 *src, int size) {
  u32 tmp[200];
  int i, j;

  // copy src into temp
  for (i = 0, j = size - 1; i < size; i++, j--) {
    tmp[i] = src[j];
  }

  // copy swapped array to original
  for (i = 0; i < size; i++) {
    src[i] = tmp[i];
  }
  return;
}

/**
 * @brief Đọc giá trị của một thanh ghi 8-bit từ LMK04832 qua SPI.
 *
 * @param SpiInstance Con trỏ tới instance SPI của bạn (chứa file descriptor).
 * @param address Địa chỉ 16-bit của thanh ghi cần đọc.
 * @return Giá trị 8-bit đọc được, hoặc -1 nếu có lỗi.
 */
int LMK04832_ReadRegister(LinuxSpiDevice *SpiInstance, uint16_t address) {
  uint8_t tx_buffer[3];
  uint8_t rx_buffer[3];
  int status;

  // =======================================================================
  // BƯỚC QUAN TRỌNG NHẤT: Chuẩn bị gói tin YÊU CẦU ĐỌC
  // Để đọc, bit R/W (bit 15) của địa chỉ 16-bit phải được set lên 1.
  // Chúng ta làm điều này bằng phép OR với 0x8000.
  uint16_t read_command_address = address | 0x8000;

  // Gói tin gửi đi sẽ là: [Addr High + R/W bit], [Addr Low], [Dummy Byte]
  tx_buffer[0] = (uint8_t)(read_command_address >> 8);
  tx_buffer[1] = (uint8_t)(read_command_address & 0xFF);
  tx_buffer[2] =
      0x00; // Gửi một byte rỗng để tạo xung clock cho chip gửi dữ liệu về

  // Gọi hàm truyền/nhận SPI.
  // Lưu ý: SPI là full-duplex, khi ta gửi 3 byte đi, ta cũng nhận về 3 byte.
  status = SPI_Transfer(SpiInstance->fd, tx_buffer, rx_buffer, 3);
  if (status != 0) {
    fprintf(stderr, "[SPI_READ] Loi khi doc dia chi 0x%04X\n", address);
    return -1; // Trả về lỗi
  }

  // Dữ liệu chip gửi về nằm ở byte thứ 3 nhận được.
  return rx_buffer[2];
}

/**
 * @brief Đọc giá trị của một thanh ghi 16-bit từ LMX2820 qua SPI.
 *
 * @param SpiInstance Con trỏ tới instance SPI của bạn.
 * @param address Địa chỉ 7-bit của thanh ghi cần đọc (0x00 - 0x7F).
 * @return Giá trị 16-bit đọc được, hoặc -1 nếu có lỗi.
 */
int32_t LMX2820_ReadRegister(LinuxSpiDevice *SpiInstance, uint8_t address) {
  uint8_t tx_buffer[3];
  uint8_t rx_buffer[3];
  int status;

  // --- Chuẩn bị gói tin YÊU CẦU ĐỌC cho LMX2820 ---
  // Byte đầu tiên: [R/W bit = 1] [A6] [A5] [A4] [A3] [A2] [A1] [A0]
  // Chúng ta OR địa chỉ 7-bit với 0x80 để set bit cao nhất (R/W) lên 1.
  tx_buffer[0] = (address & 0x7F) | 0x80;

  // Hai byte tiếp theo gửi đi là dummy để tạo clock
  tx_buffer[1] = 0x00;
  tx_buffer[2] = 0x00;

  // Thực hiện giao dịch SPI 3 byte
  status = SPI_Transfer(SpiInstance->fd, tx_buffer, rx_buffer, 3);
  if (status != 0) {
    fprintf(stderr, "[SPI_READ] Loi khi doc LMX2820 tai dia chi 0x%02X\n",
            address);
    return -1; // Trả về lỗi
  }

  // Dữ liệu 16-bit chip gửi về nằm ở byte thứ 2 và 3 nhận được.
  // Ghép 2 byte này lại thành một số 16-bit.
  uint16_t read_value = ((uint16_t)rx_buffer[1] << 8) | rx_buffer[2];

  return read_value;
}

// sysref 	= 960 khz
// clk 		= 200 MHz
// refclk 	= 300 MHz

void Start_Test_System(void) {
  u32 Val;
  u32 Minor;
  u32 Major;
  int Status;
  int Status1 = 0;

  u8 sendbuffer[3];

  // uint8_t sendbuffer[3];
  uint8_t receivebuffer[3];
  // int Status;
  // u8 recvbuffer[256];

  XRFdc_Config *ConfigPtr;
  cmdExitVal = 0;

  // int lmkConfigIndex;

  // Initialize CLI commands structure
  cli_init();
  cli_cmd_func_mem_init();
  cli_rfdc_cmd_init();
  cli_rfdc_poweron_status_init();
  cli_DacAdcIntDec_init();
  cli_rfdcNyquistzone_init();
  cli_adcGetLinkCoupling_init();
  cli_rfdcDSAVOP_init();
  cli_cmd_mts_init();
  cli_adcFreezeCalStatus_init();
  cli_adcGetCalCoefficients_init();
  cli_adcSaveCalCoeff_init();
  cli_adcLoadCalCoeff_init();
  cli_adcDisableCoeffOvrd_init();
  // cli_dac_waves_init();
  // cli_uram_play_cap_init();

  xil_printf("\n\r###############################################\n\r");
  xil_printf("Hello RFSoC World!\n\r\n");

  // Display IP version
  Val = Xil_In32(RFDC_BASE + 0x00000);
  Major = (Val >> 24) & 0xFF;
  Minor = (Val >> 16) & 0xFF;

  xil_printf("RFDC IP Version: %d.%d\r\n\n", Major, Minor);

  ////////////////////////////////////////////////////////
  // Configure zcu111 clks
  //	xil_printf("\nConfiguring the data converter clocks...\r\n");

  // initialize and reset CLK104 devices on i2c and i2c muxes
  // XRFClk_Init();
  /*
          if (resetAllClk104() == EXIT_FAILURE) {
                  xil_printf("resetAllClk104() failed\n\r");
                  return XST_FAILURE;
          }
  */
  //	xil_printf("Configuring CLK104 LMK and LMX devices\r\n");
  xil_printf("Configuring LMK and LMX devices for RFSOC-SOM\r\n");

  XGpioPs_WritePin(&psgpio, LMK_RESET_IO, 0x0);
  XGpioPs_WritePin(&psgpio, LMK_CLKIN_SEL0_IO, 0x1);
  XGpioPs_WritePin(&psgpio, LMK_CLKIN_SEL1_IO, 0x1);

  XGpioPs_WritePin(&psgpio, LMX_MUTE1_IO, 0x1);
  XGpioPs_WritePin(&psgpio, LMX_CE1_IO, 0x1);
  XGpioPs_WritePin(&psgpio, LMX_MUTE2_IO, 0x1);
  XGpioPs_WritePin(&psgpio, LMX_CE2_IO, 0x1);

#ifndef LMK / X_Use_Default

  int i;
  uint16_t current_address = 0; // LMK04832_registers[i].address;
  uint8_t value_to_write = 0;   //(u8)(LMK04832_registers[i].value);
  int read_back_value = 0;

  uint8_t current_address2 = 0; // LMK04832_registers[i].address;
  int32_t read_back_value2 = 0;
  uint32_t value_to_write2 = 0; //(u8)(LMK04832_registers[i].value);

  for (i = 0; i < 125; i++) {

    // LMK04832: 24-bit (8-bit addr High, 8-bit addr Low, 8-bit Data)
    sendbuffer[0] = (u8)(LMK04832_registers[i].address >> 8);
    sendbuffer[1] = (u8)(LMK04832_registers[i].address & 0xFF);
    sendbuffer[2] = (u8)(LMK04832_registers[i].value);

    // Gọi hàm Linux (Truyền trực tiếp fd)
    Status1 = SPI_Transfer(Pl_Spi0.fd, sendbuffer, NULL, 3);
    if (Status1 != 0) {
      xil_printf("ERROR [0x%04X]\n\r", current_address);
      continue;
    }

    usleep(40000); // usleep(200000);
  }

  xil_printf("Done with settings of LMK04832\n\r");
  usleep(500000);
  usleep(500000);

  for (i = 0; i < 123; i++) {
    current_address2 = LMX2820_registers[i].address;
    value_to_write2 = (LMX2820_registers[i].value);

    // LMX2820: 24-bit (8-bit addr, 16-bit data MSB first)
    sendbuffer[0] = (u8)(LMX2820_registers[i].address & 0xFF);
    sendbuffer[1] = (u8)(LMX2820_registers[i].value >> 8);
    sendbuffer[2] = (u8)(LMX2820_registers[i].value & 0xFF);

    Status1 = SPI_Transfer(Pl_Spi1.fd, sendbuffer, NULL, 3);
    if (Status1 != 0) {
      xil_printf("ERROR [0x%04X]\n\r", current_address2);
      continue;
    }
    // --- BƯỚC 2: ĐỌC LẠI ĐỂ KIỂM TRA ---
    usleep(20000);
    usleep(20000); // usleep(200000);
  }
  usleep(500000);
  usleep(500000);
  XGpioPs_WritePin(&psgpio, LMX_MUTE1_IO, 0x0);
  xil_printf("Done with settings of LMX2820 PLL1\n\r");

  for (i = 0; i < 123; i++) {
    current_address2 = LMX2820_registers[i].address;
    value_to_write2 = (LMX2820_registers[i].value);

    sendbuffer[0] = (u8)(LMX2820_registers[i].address & 0xFF);
    sendbuffer[1] = (u8)(LMX2820_registers[i].value >> 8);
    sendbuffer[2] = (u8)(LMX2820_registers[i].value & 0xFF);

    Status1 = SPI_Transfer(Pl_Spi2.fd, sendbuffer, NULL, 3);
    if (Status1 != 0) {
      xil_printf("ERROR [0x%04X]\n\r", current_address2);
      continue;
    }
    // --- BƯỚC 2: ĐỌC LẠI ĐỂ KIỂM TRA ---
    usleep(20000);
    usleep(20000); // usleep(200000);
  }
  usleep(500000);
  usleep(500000);
  XGpioPs_WritePin(&psgpio, LMX_MUTE2_IO, 0x0);
  xil_printf("Done with settings of LMX2820 PLL2\n\r");

#endif

  xil_printf("Done with settings of LMK/X clocks\n\r");

  /* Set config on all chips */
  // using below LMK config index
  //	lmkConfigIndex = 3;
  /*
          if (XST_FAILURE ==
     XRFClk_SetConfigOnAllChipsFromConfigId(lmkConfigIndex,
     LMX2594_FREQ_300M00_PD, LMX2594_FREQ_300M00_PD)) { printf("Failure in
     XRFClk_SetConfigOnAllChipsFromConfigId()\n\r"); return XST_FAILURE;
          }
  */
  ////////////////////////////////////////////////////
  /* Get config from PLLs and display               */

  //	printCLK104_settings();

  ///////////////////////////////////////////////////
  /* Close spi connections to clk104 */
  //	XRFClk_Close();

  //    sleep(2);

  xil_printf("\nThe design is ready to use.\r\n");
  xil_printf("Open XSCT or the terminal to run commands.\r\n");
  xil_printf("###############################################\n\r");

  xil_printf("\r\n--------------- Entering main() ---------------\n\r");

  /////////////////////////////////////////////////////////////////////////////////
  // Initialize RFdc
  //	.log_level		= METAL_LOG_DEBUG,
  //  .log_level		= METAL_LOG_INFO,

#ifdef ENABLE_METAL_PRINTS
  xil_printf("=== Metal log enabled ===\n\r");

  struct metal_init_params init_param = {
      .log_handler = my_metal_default_log_handler,
      .log_level = METAL_LOG_DEBUG,

  };
#else
  struct metal_init_params init_param = METAL_INIT_DEFAULTS;
#endif

  if (metal_init(&init_param)) {
    xil_printf("ERROR: Failed to run metal initialization\n");
    return XRFDC_FAILURE;
  }

  /* Initialize the RFdc driver. */
  ConfigPtr = XRFdc_LookupConfig(RFDC_DEVICE_ID);
  if (ConfigPtr == NULL) {
    xil_printf("Failed to init RFdc driver\r\n");
    return XST_FAILURE;
  } else {
    xil_printf("\n\rDeviceID: %d \r\nSilicon Revision: %d\r\n",
               ConfigPtr->DeviceId, ConfigPtr->SiRevision);
  }

  // ===== PHẦN CẦN THÊM VÀO =====
  // 1. Mở thiết bị RFDC bằng libmetal
  Status =
      metal_device_open("platform", "a0200000.usp_rf_data_converter", &device);
  if (Status != 0) {
    xil_printf("ERROR: Failed to open RFDC device.\n");
    return XST_FAILURE;
  }

  // 2. Lấy vùng nhớ I/O từ thiết bị
  // Con số '0' ở đây là chỉ số của vùng nhớ, thường là 0
  io = metal_device_io_region(device, 0);
  if (io == NULL) {
    xil_printf("ERROR: Failed to get I/O region from device.\n");
    metal_device_close(device);
    return XST_FAILURE;
  }

  // 3. Gán vùng I/O này vào instance của driver RFDC
  //    Đây là bước quan trọng nhất bị thiếu
  RFdcInst.io = io;

  /* Initializes the controller */
  Status = XRFdc_CfgInitialize(&RFdcInst, ConfigPtr);
  if (Status != XST_SUCCESS) {
    xil_printf("Failed to init RFdc controller\r\n");
    return XST_FAILURE;
  }

  /////////////////////////////////////////////////////////////////////////////////
  // Display various configurations/status of RFDC

  // Restart the data converters - related to clock configuration
  //	rfdcShutdown(NULL);
  rfdcStartup(NULL);

  // display ready status for ADCs and DACs
  rfdcReady(NULL);

  // display the Power On Status ADCs and DACs
  //	rfdcPoweronStatus(NULL);
  int pi;
  u32 my_cmd_values[3];

  for (pi = 0; pi < 16; pi++) {
    my_cmd_values[0] = pi / 4;      // Tile_Id = 0
    my_cmd_values[1] = pi % 4;      // Block_Id = 0 (kenh/khoi DAC dau tien)
    my_cmd_values[2] = DAC_Current; // uACurrent = 1000 uA
    dacSetVOP(my_cmd_values);       // cai dat kenh DAC1
    usleep(1000);
  }

  for (pi = 0; pi < 16; pi++) {
    adcSetDSA2(pi / 4, pi % 4, ADC_ATT);
    usleep(1000);
  }

  u32 my_cmd_values2[3];
  my_cmd_values2[0] = 0;
  my_cmd_values2[1] = 0;
  my_cmd_values2[2] = 0;
  adcGetDSA(my_cmd_values2);
  // Display current setting for DAC 0 in tile 0
  dacCurrent(NULL);

  /////////////////////////////////////////////////////////////////////////////////
  // Misc initialization specific to this design

  // Initialize buffer address and size for DAC Player
  //		{
  //			u32 vals[] = { (u64)URAM_PLAY_BASE >> 32,
  //(URAM_PLAY_BASE & 0xFFFFFFFF), 128 }; 			waveSetBuf(vals);
  //		}
  //
  //		{
  //			int clr_val = 0;
  //			   xil_printf("=== Initialize ADC coefficients array
  //===\n\r");
  //
  //				for (clr_val=0; clr_val < NUM_SETS; clr_val++)
  //				{
  //					u32 tmp[] = { (u32)clr_val };
  //					adcCoeffClr(tmp);
  //				}
  //			   xil_printf("\n\r");
  //
  //		}

  // enable MTS function - Manhtx
  xil_printf("Manhtx implement new automation process\r\n");
  u32 MTS_mask = 0xffffffff;
  dacMTS(&MTS_mask);
  adcMTS(&MTS_mask);

  /////////////////////////////////////////////////////////////////////////////////
  // Main Loop

  //	    xil_printf("--------------------------------\r\n");
  //	    xil_printf("Available commands:\r\n");
  //	    xil_printf("--------------------------------\r\n");
  //		doHelp();

  //		// Run command line interpreter
  //		xil_printf("Enter ? for help on commands\n\r");
  //		xil_printf("Enter 'exit' to end the application and shutdown the
  //data converters.\n\r");
  //
  //
  //		while(!cmdExitVal)
  //		{
  //			xil_printf( "\n\r> ");
  //
  //			Status = cmdParseAndRun();
  //
  //			if (Status == XST_NO_DATA)
  //			{
  //				continue;
  //			}
  //			if (Status != XST_SUCCESS)
  //			{
  //				xil_printf("Invalid command or option. Enter ?
  //for help. \n\r");
  //			}
  //		}

  //		rfdcShutdown(NULL);
  //		xil_printf("--- Application closed, have a good day! --- \r\n");
}
