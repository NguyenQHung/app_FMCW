
// --- CÁC THƯ VIỆN HỆ THỐNG CỦA LINUX ---
#include "linux_hal.h"
#include <arpa/inet.h>
#include <fcntl.h> // For open()
#include <netinet/in.h>
#include <poll.h> // Cần thiết cho việc nhận dữ liệu không blocking
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// --- CÁC FILE HEADER CỦA DỰ ÁN ---
#include "main.h" // Bao gồm linux_hal.h bên trong

#include "ADF4159.h"
#include "ADF4360.h"
#include "DK_Maythu.h"
#include "MI_505.h"
#include "Noise.h"
#include "Radar_HW.h"
// #include "RF_SOC.h"  // Comment: RF init cần port
#include "CAN_Control.h"
#include "DK_Maythu.h"
#include "Erob.h"
#include "LMK_display.h"
#include "LMX_display.h"
#include "UDP_Process.h"

#include "metal/device.h"
#include "metal/io.h"
#include "xrfdc.h"

// UdpSocket my_udp_socket;
UdpConnection udp_conn; // Biến tĩnh để quản lý kết nối
ssize_t bytes_sent;
// // Khai báo socket
pthread_mutex_t radar_data_mutex; // Khai báo mutex toàn cục

// Debug defines
#define READ_BACK_INDEX_ARRAY

// Define port to listen on
#define FF_UDP_PORT 105

// TIMEOUT FOR DMA AND GMM WAIT
#define RESET_TIMEOUT_COUNTER 10000

// DEFINES
#define WAVE_SIZE_BYTES 512 // Number of samples in waveform
#define INDARR_SIZE_BYTES                                                      \
  1024 // Number of bytes required to hold 512 fixed point floats

// HARDWARE DEFINES
#define NUMCHANNELS                                                            \
  2           // Number of parallel operations done on input stream (1 OR 2)
#define BW 32 // Total number of bits in fixed point data type
#define IW 24 // Number of bits left of decimal point in fixed point data type
#define BITDIV 256.0 // Divisor to shift fixed point to int and back to float

// Các port đích
#define REMOTE_PORT_1 1024
#define REMOTE_PORT_2 106
#define REMOTE_PORT_3 1706
#define REMOTE_PORT_4 1707
#define REMOTE_PORT_5 1708

// Địa chỉ IP của máy PC đích (ví dụ: 192.168.1.10)
#define REMOTE_IP_ADDR "192.168.1.19"

LinuxSpiDevice Pl_Spi0;
LinuxSpiDevice Pl_Spi1;
LinuxSpiDevice Pl_Spi2;
LinuxSpiDevice Pl_Spi3;
LinuxSpiDevice Pl_Spi4;

// XBram 			Bram;					/* The
// Instance of the BRAM Driver */ XScuGic 		ZYNQ_INTR;
// // ZYNQ interrupt device

XGpioPs psgpio;
XGpio GPIO_ENDFRAME_IRQ;  //	XPAR_ENDFRAME_IRQ_DEVICE_ID
XGpio GPIO_ENDFRAME_IRQ1; //	XPAR_ENDFRAME_IRQ_DEVICE_ID
XGpio GPIO_HS1;           //	GPIO_HS1
XGpio GPIO_HS2;           //	GPIO_HS2
XGpio GPIO_HS3;           //	GPIO_HS3
XGpio GPIO_SYNC_CMAC;

XUartLite Uart0; /* Instance of the UartLite Device */
XUartLite Uart1; /* Instance of the UartLite Device */
XUartLite Uart2; /* Instance of the UartLite Device */
XUartLite Uart3; /* Instance of the UartLite Device */
XUartLite Uart4; /* Instance of the UartLite Device */

LinuxCanBus Erob_Can0;
LinuxCanBus Erob_Can1;
// struct can_frame frame; // Buffer để nhận một CAN frame
// struct pollfd fds[2];   // Mảng để theo dõi 2 socket: can0 và can1

// --- TẠO VÀ CHẠY LUỒNG NHẬN CAN ---
pthread_t can_thread_id;
pthread_t udp_tid;
pthread_t FFT_tid;
pthread_t thread_adc;
pthread_t thread_fft;
pthread_t thread_CMAC;
pthread_t thread_CMAC_TX;

int trigger_adc = 0;  // Cờ báo hiệu cần đọc ADC
int trigger_FFT = 0;  // Cờ báo hiệu cần đọc ADC
int trigger_idle = 0; // Cờ báo hiệu cần đọc ADC
int DMA_cmd = 0;
int DMA_cnt = 0;
// pthread_mutex_t adc_mutex = PTHREAD_MUTEX_INITIALIZER;

XRFdc RFdcInst; /* RFdc driver instance */
struct metal_device *device = NULL;
struct metal_io_region *io = NULL;

lmk_config_t lmkConfig;
lmx_config_t lmxConfig;

int UDP_Cnt = 0;
int UDP_Sel = 0;
int Check_CMAC_Bringup = 0;
int Check_ADC_Raw = 0;

// --- KHAI BÁO CÁC HÀM TỪ CÁC FILE KHÁC (Forward Declarations) ---
// Giúp trình biên dịch biết các hàm này tồn tại trước khi nó thấy định nghĩa
// của chúng.
void Radar_Innit(void);
void Signal_UDP_Update(void);
void Signal_IRQ_Update(void);
// void VCO_Set_Freq(unsigned int freq);
// void Setup_Led_Channel(int val);
void delay_1s(void); // Giả sử bạn có hàm này

void hal_FFT_IRQ() {
  Radar.Read_ok = 1;

  DMA_cnt++;
  if (DMA_cnt >= 200) {
    DMA_cnt = 0;
    DMA_cmd = 1;
  }
}

#define ADC_RAM_ADDR 0x14000000 // Thử dùng vùng CMA an toàn hơn
#define ADC_SIZE (32 * 1024 * 1024)

/*
 * @brief Hàm giả lập start_application của LwIP.
 *        Trên Linux, hàm này sẽ tạo và bind socket UDP.
 * @return 0 nếu thành công, < 0 nếu thất bại.
 */
// --- Hàm khởi tạo TOÀN DIỆN ---
int start_application(uint16_t local_port) {
  // 1. Tạo socket
  if ((udp_conn.sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Loi: khong the tao socket");
    return -1;
  }

  // 2. Bind socket vào port của bo mạch để nhận dữ liệu
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(local_port);

  if (bind(udp_conn.sockfd, (const struct sockaddr *)&server_addr,
           sizeof(server_addr)) < 0) {
    perror("Loi: khong the bind socket");
    close(udp_conn.sockfd);
    return -2;
  }

  // 3. Chuẩn bị các địa chỉ đích để gửi chủ động
  memset(&udp_conn.remote_pc1_addr, 0, sizeof(udp_conn.remote_pc1_addr));
  udp_conn.remote_pc1_addr.sin_family = AF_INET;
  udp_conn.remote_pc1_addr.sin_port = htons(REMOTE_PORT_1); // = 1024
  inet_pton(AF_INET, "192.168.1.10", &udp_conn.remote_pc1_addr.sin_addr);

  memset(&udp_conn.remote_pc2_addr, 0, sizeof(udp_conn.remote_pc2_addr));
  udp_conn.remote_pc2_addr.sin_family = AF_INET;
  udp_conn.remote_pc2_addr.sin_port = htons(REMOTE_PORT_5); // = 1708
  inet_pton(AF_INET, "192.168.1.10", &udp_conn.remote_pc2_addr.sin_addr);

  // 4. Khởi tạo các biến trạng thái
  udp_conn.client_len = sizeof(udp_conn.client_addr);
  udp_conn.is_client_addr_valid = 0;

  printf("May chu UDP da khoi dong @ port %d\n", local_port);
  return 0; // XST_SUCCESS
}

void xemacif_input(UdpConnection *conn) {
  unsigned char rx_buffer[1400];

  // Vòng lặp để xử lý tất cả các gói tin đang chờ trong buffer
  while (1) {
    struct pollfd fds;
    fds.fd = conn->sockfd;
    fds.events = POLLIN;

    // Kiểm tra xem có dữ liệu không, không chờ đợi
    int ret = poll(&fds, 1, 0);

    // 1. Xử lý lỗi từ poll()
    if (ret < 0) {
      if (errno == EINTR) { // Bị ngắt bởi một signal, thử lại
        continue;
      }
      perror("Loi poll() trong xemacif_input");
      break; // Thoát vòng lặp nếu có lỗi nghiêm trọng
    }

    // 2. Không có dữ liệu để đọc, thoát ngay lập tức
    if (ret == 0) {
      break;
    }

    // 3. Có dữ liệu, tiến hành đọc
    if (fds.revents & POLLIN) {
      ssize_t n =
          recvfrom(conn->sockfd, rx_buffer, sizeof(rx_buffer), 0,
                   (struct sockaddr *)&conn->client_addr, &conn->client_len);

      if (n < 0) {
        // Lỗi khi đọc, ví dụ "Connection refused"
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
          perror("Loi recvfrom() trong xemacif_input");
        }
        break; // Thoát vòng lặp
      }

      if (n > 0) {
        // ---- Logic xử lý gói tin của bạn (giữ nguyên) ----
        if (n == 320) {
          memcpy(&Radar.PC8_Config, rx_buffer, 320);
          if ((Radar.PC8_Config[0] == 0xab) && (Radar.PC8_Config[1] == 0x56)) {
            Radar.Receiver_Status4 = 1;
          }
          conn->is_client_addr_valid = 1;
        } else if (n == 828) {
          memcpy(&Radar.PC8_Excel, rx_buffer, 828);
          Radar.Receiver_Excel = 1;
          conn->is_client_addr_valid = 1;
        }
        // Thêm các else if khác nếu cần
      }
    }
  } // Kết thúc vòng lặp while(1)
}

/*****************************************************************************/
/**
 *
 * Main function
 *
 * TBD
 *
 * @param	None
 *
 * @return
 *		- XST_SUCCESS if tests pass
 *		- XST_FAILURE if fails.
 *
 * @note		None.
 *
 ******************************************************************************/
int main(void) {

  if (hal_init() != 0) {
    xil_printf("HAL initialization failed. Exiting.\n\r");
    return -1;
  }
  xil_printf(" hal_init ok\n");

  // In thông tin cấu hình mạng (trên Linux, IP được cấu hình tĩnh bởi hệ điều
  // hành)
  // xil_printf("Cau hinh mang can duoc thiet lap tinh trong
  // /etc/network/interfaces\n");
  xil_printf("IP Bo mach: 192.168.1.5\n");
  xil_printf("IP PC: 192.168.1.10\n");

  Radar_Innit();
  usleep(100000); // cho 1 s cho cmac on dinh

  // Trong hàm main() trước khi tạo thread
  pthread_mutex_init(&radar_data_mutex, NULL);

  // khởi t
  Start_Test_System(); // kiem tra ok

  ADF4159_VCO_ENable(1);
  usleep(1000);
  usleep(1000);
  ADF4360_Set_Freq(1400);
  usleep(1000);
  ADF4360_Set_Freq(1400);
  usleep(1000);
  ADF4159_Set_Freq(7700);
  usleep(1000);
  ADF4159_Set_Freq(7700);
  usleep(10000);

  // Bắt đầu ứng dụng mạng (tạo và bind socket)
  int status = start_application(FF_UDP_PORT);
  if (status != XST_SUCCESS) {
    xil_printf("Loi start_application() với ma: %d\n", status);
    goto ErrorOrDone;
  }

  Radar.Config_Done = 1;
  _UDP_Innit_Buffer();

  XGpio_Interrupt_Innit();

  // if (pthread_create(&udp_tid, NULL, udp_control_thread, NULL) != 0) {
  //     perror("Failed to create UDP control thread");
  //     return -1;
  // }
  // else
  // {
  // 	xil_printf("pthread_create(&udp_tid, NULL, udp_control_thread, NULL) =>
  // done \r\n");
  // }

  if (pthread_create(&can_thread_id, NULL, can_receiver_thread, NULL) != 0) {
    perror("Không thể tạo luồng nhận CAN");
    return -1;
  } else {
    xil_printf("pthread_create(&can_thread_id, NULL, can_receiver_thread, "
               "NULL) => done \r\n");
  }

  if (pthread_create(&FFT_tid, NULL, XGpio_Interrupt_IRQ, NULL) != 0) {
    perror("Không thể tạo luồng nhận XGpio_Interrupt_IRQ");
    return -1;
  } else {
    xil_printf("pthread_create(&FFT_tid, NULL, XGpio_Interrupt_IRQ, NULL) => "
               "done \r\n");
  }

  pthread_create(&thread_adc, NULL, ADC_Worker_Thread, NULL);
  xil_printf("pthread_create(&thread_adc, NULL, ADC_Worker_Thread, NULL) => "
             "done \r\n");

  pthread_create(&thread_fft, NULL, FFT_Monitor_SG_S2MM_Thread, NULL);
  xil_printf("pthread_create(&thread_fft, NULL, FFT_Monitor_SG_S2MM_Thread, "
             "NULL) => done \r\n");

  /* receive and process packets */
  while (1) {
    // 2.1. Kiểm tra và nhận các gói tin UDP (không blocking)
    xemacif_input(&udp_conn);

    if (Radar.Receiver_Status4 == 1) // goi tin tu PC8
    {
      usleep(20);
      Radar.Receiver_Status4 = 0;
      // xil_printf("PC8\r\n");
      if (Radar.Screen_Modei != Radar.Screen_Mode) {
        xil_printf("Screen_Mode = %d\r\n", Radar.Screen_Mode);
        Radar.Screen_Modei = Radar.Screen_Mode;
      }

      if (PC8.Gia_Quay_ENi != PC8.Gia_Quay_EN) {
        xil_printf("Gia_Quay = %d\r\n", PC8.Gia_Quay_EN);
        PC8.Gia_Quay_ENi = PC8.Gia_Quay_EN;
      }
      Radar_Read_PC8_CMD();
    }

    if (Radar.Receiver_Excel == 1) // goi tin tu PC8
    {
      usleep(20);
      Radar.Receiver_Excel = 0;
      Radar_Read_PC8_Excel();
      xil_printf("Radar_Read_PC8_Excel Table done\r\n");
    }

    if (DMA_cmd == 1) {
      DMA_cmd = 0;
      pthread_mutex_lock(&radar_data_mutex);
      Check_ADC_Raw++;

      if ((Check_ADC_Raw % 10) == 0)
        trigger_FFT = 1;

      if (Check_ADC_Raw > 100) {
        Check_ADC_Raw = 0;
        trigger_adc = 1; // Kích hoạt đọc ADC trong thread kia, 10s 1 lan
      }

      ADF4360_Set_Freq(1400);
      usleep(20);
      ADF4159_Set_Freq(7700);
      pthread_mutex_unlock(&radar_data_mutex);

      Check_CMAC_Bringup++;
      if (Check_CMAC_Bringup >= 50) {
        Check_CMAC_Bringup = 0;
        trigger_idle = 1;
      }
    }

    _UDP_PC1_Send();
    _UDP_PC8_Send();
  }

ErrorOrDone:
  xil_printf("Loi nghiem trong! Dang dong chuong trinh...\n");

  // (Tùy chọn) Chờ luồng CAN kết thúc khi thoát chương trình
  pthread_join(thread_CMAC, NULL);
  pthread_join(thread_adc, NULL);
  pthread_join(thread_fft, NULL);
  pthread_join(can_thread_id, NULL);
  pthread_join(FFT_tid, NULL);

  xil_printf("Application exiting, closing RFDC device...\n");
  metal_device_close(device);

  hal_cleanup();

  CAN_Close(&Erob_Can0);
  CAN_Close(&Erob_Can1);

  if (udp_conn.sockfd > 0) {
    close(udp_conn.sockfd);
  }
  return 0;
}
