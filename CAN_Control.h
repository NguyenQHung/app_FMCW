#ifndef LINUX_CAN_CONTROL_H_
#define LINUX_CAN_CONTROL_H_

#include "main.h"
#include "linux_hal.h" // Để có các kiểu dữ liệu u8, u32, ...

// =============================================================================
// ĐỊNH NGHĨA VÀ STRUCT
// =============================================================================

/*
 * Các hằng số cho CAN, có thể giữ lại nếu logic ứng dụng cần
 */
#define TEST_MESSAGE_ID		0x643
#define FRAME_DATA_LENGTH	8 // Có thể thay đổi

// =============================================================================
// KHAI BÁO CÁC BIẾN TOÀN CỤC
// =============================================================================

// Thay thế Erob_Can0 và Erob_Can1 bằng các struct quản lý của Linux
extern LinuxCanBus Erob_Can0;
extern LinuxCanBus Erob_Can1;

// =============================================================================
// KHAI BÁO CÁC HÀM API
// =============================================================================

/**
 * @brief Khởi tạo và mở một bus CAN.
 * @param bus - Con trỏ tới struct LinuxCanBus cần khởi tạo.
 * @param interface_name - Tên của giao diện CAN (ví dụ: "can0").
 * @return 0 nếu thành công, -1 nếu thất bại.
 */
int CAN_Init(LinuxCanBus *bus, const char* interface_name);

/**
 * @brief Gửi một frame CAN.
 * @param bus - Con trỏ tới bus CAN đã được khởi tạo.
 * @param id - CAN ID của frame.
 * @param dlc - Độ dài dữ liệu (0-8).
 * @param data - Con trỏ tới buffer chứa dữ liệu.
 * @return 0 nếu thành công, -1 nếu thất bại.
 */
int CAN_SendFrame(LinuxCanBus *bus, u32 id, u8 dlc, const u8 *data);

/**
 * @brief Đọc một frame CAN (hàm này sẽ block cho đến khi có dữ liệu).
 * @param bus - Con trỏ tới bus CAN đã được khởi tạo.
 * @param id - Con trỏ để lưu CAN ID của frame nhận được.
 * @param dlc - Con trỏ để lưu độ dài dữ liệu của frame nhận được.
 * @param data - Buffer để lưu dữ liệu nhận được (phải đủ lớn, ít nhất 8 bytes).
 * @return 0 nếu thành công, -1 nếu thất bại.
 */
int CAN_RecvFrame(LinuxCanBus *bus, u32 *id, u8 *dlc, u8 *data);

/**
 * @brief Đóng và giải phóng tài nguyên của một bus CAN.
 * @param bus - Con trỏ tới bus CAN cần đóng.
 */
void CAN_Close(LinuxCanBus *bus);

/**
 * @brief Hàm khởi tạo cấp cao, gọi các hàm init cho cả hai bus.
 *        Hàm này thay thế cho CAN_Innit_Data() cũ.
 * @return 0 nếu thành công, -1 nếu thất bại.
 */
int CAN_Init_All(void);

void* can_receiver_thread(void *arg) ;

#endif /* LINUX_CAN_CONTROL_H_ */