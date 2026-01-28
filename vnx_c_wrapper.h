// vnx_c_wrapper.h
#ifndef VNX_C_WRAPPER_H
#define VNX_C_WRAPPER_H

#include <stdint.h>
#include <stdbool.h>
#include "linux_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

// Opaque handles (Con trỏ ẩn để quản lý đối tượng C++)
typedef void* CMAC_Handle;
typedef void* NetworkLayer_Handle;

// --- Khởi tạo và Giải phóng ---
// kernel_name thường là "cmac_0", "networklayer_0" tùy vào thiết kế Vivado
CMAC_Handle vnx_cmac_create(int device_index, const char* kernel_name);
void vnx_cmac_destroy(CMAC_Handle handle);

NetworkLayer_Handle vnx_nl_create(int device_index, const char* kernel_name);
void vnx_nl_destroy(NetworkLayer_Handle handle);

// --- Các hàm cho CMAC ---
bool vnx_cmac_get_link_status(CMAC_Handle handle);
void vnx_cmac_set_loopback(CMAC_Handle handle, bool enable);

// --- Các hàm cho Network Layer ---
uint32_t vnx_nl_encode_ip(const char* ip_str);
void vnx_nl_update_ip(NetworkLayer_Handle handle, const char* ip_str);
void vnx_nl_arp_discovery(NetworkLayer_Handle handle);
void vnx_nl_configure_socket(NetworkLayer_Handle handle, int index, 
                             const char* theirIP, uint16_t theirPort, 
                             uint16_t myPort, bool valid);
void vnx_nl_populate_socket_table(NetworkLayer_Handle handle);

// --- Các hàm lấy thống kê (Statistics) ---
uint32_t vnx_nl_get_udp_in_pkts(NetworkLayer_Handle handle);
uint32_t vnx_nl_get_udp_out_pkts(NetworkLayer_Handle handle);

#ifdef __cplusplus
}
#endif

#endif