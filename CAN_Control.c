/*
 * CAN_Control.c - Triển khai driver CAN sử dụng SocketCAN trên Linux
 */

#include "CAN_Control.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

// =============================================================================
// ĐỊNH NGHĨA CÁC BIẾN TOÀN CỤC
// =============================================================================
struct Erob_Struct  Erob_Motor;
// Giữ lại struct này nếu logic cấp cao của bạn cần nó để làm buffer
struct CAN_Control_def CAN_Controls;

void* can_receiver_thread(void *arg) 
{
    struct can_frame frame;
    struct pollfd fds[2];

    // Cấu hình poll để theo dõi cả can0 và can1
    fds[0].fd = Erob_Can0.socket_fd;
    fds[0].events = POLLIN | POLLERR; // Theo dõi cả dữ liệu và lỗi

    fds[1].fd = Erob_Can1.socket_fd;
    fds[1].events = POLLIN | POLLERR;

    printf("[CAN Thread] Bat đau lang nghe...\n");

    while (1) {
        // Lệnh poll() này sẽ BLOCK VÔ THỜI HẠN (-1) cho đến khi có sự kiện
        // Kernel sẽ đánh thức luồng này dậy ngay khi có frame CAN
        int ret = poll(fds, 2, -1); 

        if (ret < 0) {
            perror("[CAN Thread] Loi poll(), luong ket thuc");
            break;
        }

        // Kiểm tra socket can0
        if (fds[0].revents & POLLIN) {
            if (read(Erob_Can0.socket_fd, &frame, sizeof(frame)) > 0) {
                // XỬ LÝ FRAME TỪ CAN0 NGAY LẬP TỨC
                // (Nội dung của hàm RecvHandler cũ)
                memcpy(CAN_Controls.RxFrame, &frame, sizeof(frame));
                Erob_Get_ENC1_Data();
            }
        }
        
        // Kiểm tra socket can1
        if (fds[1].revents & POLLIN) {
            if (read(Erob_Can1.socket_fd, &frame, sizeof(frame)) > 0) {
                // XỬ LÝ FRAME TỪ CAN1 NGAY LẬP TỨC
                // (Nội dung của hàm RecvHandler2 cũ)
                memcpy(CAN_Controls.RxFrame2, &frame, sizeof(frame));
                Erob_Get_ENC2_Data();
            }
        }
        
        // (Tùy chọn) Xử lý các sự kiện lỗi...
    }

    return NULL;
}

// =============================================================================
// TRIỂN KHAI CÁC HÀM API
// =============================================================================

int CAN_Init(LinuxCanBus *bus, const char* interface_name) {
    struct sockaddr_can addr;
    struct ifreq ifr;

    // Lưu lại tên interface
    strncpy(bus->interface_name, interface_name, sizeof(bus->interface_name) - 1);
    bus->is_connected = 0;

    // 1. Tạo socket
    bus->socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (bus->socket_fd < 0) {
        perror("[CAN] Error creating socket");
        return -1;
    }

    // 2. Lấy chỉ số của interface (ví dụ: "can0")
    strcpy(ifr.ifr_name, bus->interface_name);
    if (ioctl(bus->socket_fd, SIOCGIFINDEX, &ifr) < 0) {
        perror("[CAN] Error getting interface index");
        close(bus->socket_fd);
        bus->socket_fd = -1;
        return -1;
    }

    // 3. Bind socket vào interface
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(bus->socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("[CAN] Error binding socket");
        close(bus->socket_fd);
        bus->socket_fd = -1;
        return -1;
    }

    bus->is_connected = 1;
    xil_printf("[CAN] Interface %s initialized successfully.\n", bus->interface_name);
    return 0;
}

int CAN_SendFrame(LinuxCanBus *bus, u32 id, u8 dlc, const u8 *data) {
    if (!bus->is_connected || bus->socket_fd < 0) {
        return -1;
    }
    if (dlc > 8) {
        dlc = 8;
    }

    struct can_frame frame;
    frame.can_id = id;
    frame.can_dlc = dlc;
    memcpy(frame.data, data, dlc);

    if (write(bus->socket_fd, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
        perror("[CAN] Error writing to CAN socket");
        return -1;
    }

    return 0;
}

int CAN_RecvFrame(LinuxCanBus *bus, u32 *id, u8 *dlc, u8 *data) {
    if (!bus->is_connected || bus->socket_fd < 0) {
        return -1;
    }

    struct can_frame frame;
    int nbytes = read(bus->socket_fd, &frame, sizeof(struct can_frame));

    if (nbytes < 0) {
        perror("[CAN] Error reading from CAN socket");
        return -1;
    }

    if (nbytes < sizeof(struct can_frame)) {
        fprintf(stderr, "[CAN] Incomplete CAN frame received\n");
        return -1;
    }

    *id = frame.can_id;
    *dlc = frame.can_dlc;
    memcpy(data, frame.data, frame.can_dlc);

    return 0;
}

void CAN_Close(LinuxCanBus *bus) {
    if (bus->socket_fd >= 0) {
        close(bus->socket_fd);
        bus->socket_fd = -1;
        bus->is_connected = 0;
        xil_printf("[CAN] Interface %s closed.\n", bus->interface_name);
    }
}

int CAN_Init_All(void) {
    // Trước khi chạy code, bạn cần cấu hình interface từ terminal Linux:
    // > ip link set can0 up type can bitrate 1000000
    // > ip link set can1 up type can bitrate 1000000
    
    xil_printf("[CAN] Initializing all CAN buses...\n");
    if (CAN_Init(&Erob_Can0, "can0") != 0) {
        fprintf(stderr, "[CAN] Failed to initialize can0.\n");
        return -1;
    }
    
    if (CAN_Init(&Erob_Can1, "can1") != 0) {
        fprintf(stderr, "[CAN] Failed to initialize can1.\n");
        CAN_Close(&Erob_Can1); // Dọn dẹp cái đã mở trước đó
        return -1;
    }

    return 0;
}