#include "XGpioPS_Hal.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define ZYNQMP_GPIO_CHIP_NAME "gpiochip9"
#define ZYNQMP_GPIO_NUM_PINS 174
#define GPIO_CONSUMER "fmcw_app"

// Cấu trúc quản lý trạng thái, giữ nguyên
static struct {
  struct gpiod_chip *chip;
  struct gpiod_line *lines[ZYNQMP_GPIO_NUM_PINS];
  bool is_initialized;
} ps_gpio_state = {.chip = NULL, .lines = {NULL}, .is_initialized = false};

/**
 * @brief Khởi tạo kết nối tới chip PS GPIO.
 */
int XGpioPs_CfgInitialize(XGpioPs *InstancePtr, XGpioPs_Config *ConfigPtr,
                          unsigned long EffectiveAddr) {
  if (ps_gpio_state.is_initialized)
    return XST_SUCCESS;

  ps_gpio_state.chip = gpiod_chip_open_by_name(ZYNQMP_GPIO_CHIP_NAME);
  if (!ps_gpio_state.chip) {
    perror("[HAL] Lỗi: Không thể mở gpiochip cho PS");
    return XST_FAILURE;
  }

  // Khởi tạo mảng con trỏ line
  memset(ps_gpio_state.lines, 0, sizeof(ps_gpio_state.lines));

  ps_gpio_state.is_initialized = true;
  printf("[HAL] XGpioPs_CfgInitialize (gpiod) - OK.\n");
  return XST_SUCCESS;
}

/**
 * @brief Cấu hình hướng cho một chân GPIO (Input hoặc Output).
 */
void XGpioPs_SetDirectionPin(XGpioPs *InstancePtr, int Pin, int Direction) {
  if (!ps_gpio_state.is_initialized || Pin < 0 || Pin >= ZYNQMP_GPIO_NUM_PINS)
    return;

  // Lấy handle của chân (line) nếu đây là lần đầu tiên
  if (ps_gpio_state.lines[Pin] == NULL) {
    ps_gpio_state.lines[Pin] = gpiod_chip_get_line(ps_gpio_state.chip, Pin);
    if (!ps_gpio_state.lines[Pin]) {
      fprintf(stderr, "[HAL] Lỗi: Không thể get line %d\n", Pin);
      return;
    }
  }

  // Chỉ thực hiện thay đổi nếu chân chưa được request, hoặc request sai hướng
  if (!gpiod_line_is_requested(ps_gpio_state.lines[Pin]) ||
      gpiod_line_direction(ps_gpio_state.lines[Pin]) !=
          (Direction ? GPIOD_LINE_DIRECTION_OUTPUT
                     : GPIOD_LINE_DIRECTION_INPUT)) {
    // Giải phóng trước khi request lại với hướng mới
    if (gpiod_line_is_requested(ps_gpio_state.lines[Pin])) {
      gpiod_line_release(ps_gpio_state.lines[Pin]);
    }

    int ret;
    if (Direction == 1) { // Output
      // Không set giá trị mặc định, để hàm WritePin quyết định
      ret =
          gpiod_line_request_output(ps_gpio_state.lines[Pin], GPIO_CONSUMER, 0);
    } else { // Input
      ret = gpiod_line_request_input(ps_gpio_state.lines[Pin], GPIO_CONSUMER);
    }

    if (ret < 0) {
      fprintf(stderr, "[HAL] Lỗi request line %d: %s\n", Pin, strerror(errno));
    }
  }
}

/**
 * @brief Bật/tắt buffer đầu ra. Trong libgpiod, nó tương đương với
 * SetDirection.
 */
void XGpioPs_SetOutputEnablePin(XGpioPs *InstancePtr, int Pin, int IsOutput) {
  XGpioPs_SetDirectionPin(InstancePtr, Pin, IsOutput);
}

/**
 * @brief Ghi giá trị (0 hoặc 1) vào một chân output, sau đó đọc lại để xác
 * nhận.
 */
void XGpioPs_WritePin(XGpioPs *InstancePtr, int Pin, int State) {
  if (ps_gpio_state.lines[Pin] != NULL &&
      gpiod_line_is_requested(ps_gpio_state.lines[Pin])) {

    // 1. Thực hiện ghi giá trị
    if (gpiod_line_set_value(ps_gpio_state.lines[Pin], State) < 0) {
      fprintf(stderr, "[HAL] Lỗi khi ghi gia tri vao Pin %d: %s\n", Pin,
              strerror(errno));
      return; // Nếu ghi lỗi thì không cần đọc lại
    }
  }
}

/**
 * @brief Đọc giá trị từ một chân GPIO.
 */
int XGpioPs_ReadPin(XGpioPs *InstancePtr, int Pin) {
  if (ps_gpio_state.lines[Pin] != NULL &&
      gpiod_line_is_requested(ps_gpio_state.lines[Pin])) {
    int val = gpiod_line_get_value(ps_gpio_state.lines[Pin]);
    if (val < 0) {
      fprintf(stderr, "[HAL] Lỗi đọc giá trị từ Pin %d: %s\n", Pin,
              strerror(errno));
      return -1;
    }
    return val;
  }
  return -1;
}

/**
 * @brief Dọn dẹp, giải phóng tất cả các chân GPIO đã sử dụng.
 */
void XGpioPs_Cleanup(void) {
  if (ps_gpio_state.is_initialized) {
    for (int i = 0; i < ZYNQMP_GPIO_NUM_PINS; ++i) {
      if (ps_gpio_state.lines[i] != NULL) {
        gpiod_line_release(ps_gpio_state.lines[i]);
      }
    }
    gpiod_chip_close(ps_gpio_state.chip);
    ps_gpio_state.is_initialized = false;
    printf("[HAL] Đã giải phóng tất cả tài nguyên PS GPIO.\n");
  }
}
