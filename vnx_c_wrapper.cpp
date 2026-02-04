// vnx_c_wrapper.cpp
#include "vnx_c_wrapper.h"
#include "vnx/cmac.hpp"
#include "vnx/networklayer.hpp"
#include <xrt/xrt_device.h>
#include <xrt/xrt_kernel.h>

extern "C" {

CMAC_Handle vnx_cmac_create(int device_index, const char* kernel_name) {
    auto device = xrt::device(device_index);
    auto ip = xrt::ip(device, kernel_name);
    return static_cast<CMAC_Handle>(new vnx::CMAC(std::move(ip)));
}

void vnx_cmac_destroy(CMAC_Handle handle) {
    delete static_cast<vnx::CMAC*>(handle);
}

bool vnx_cmac_get_link_status(CMAC_Handle handle) {
    auto* cmac = static_cast<vnx::CMAC*>(handle);
    auto status = cmac->link_status();
    return status["rx_status"]; // Trả về link UP/DOWN
}

void vnx_cmac_set_loopback(CMAC_Handle handle, bool enable) {
    static_cast<vnx::CMAC*>(handle)->set_loopback(enable);
}

NetworkLayer_Handle vnx_nl_create(int device_index, const char* kernel_name) {
    auto device = xrt::device(device_index);
    auto ip = xrt::ip(device, kernel_name);
    return static_cast<NetworkLayer_Handle>(new vnx::Networklayer(std::move(ip)));
}

void vnx_nl_destroy(NetworkLayer_Handle handle) {
    delete static_cast<vnx::Networklayer*>(handle);
}

uint32_t vnx_nl_encode_ip(const char* ip_str) {
    return vnx::encode_ip_address(std::string(ip_str));
}

void vnx_nl_update_ip(NetworkLayer_Handle handle, const char* ip_str) {
    static_cast<vnx::Networklayer*>(handle)->update_ip_address(std::string(ip_str));
}

void vnx_nl_arp_discovery(NetworkLayer_Handle handle) {
    static_cast<vnx::Networklayer*>(handle)->arp_discovery();
}

void vnx_nl_configure_socket(NetworkLayer_Handle handle, int index, 
                             const char* theirIP, uint16_t theirPort, 
                             uint16_t myPort, bool valid) {
    static_cast<vnx::Networklayer*>(handle)->configure_socket(index, std::string(theirIP), theirPort, myPort, valid);
}

void vnx_nl_populate_socket_table(NetworkLayer_Handle handle) {
    static_cast<vnx::Networklayer*>(handle)->populate_socket_table();
}

uint32_t vnx_nl_get_udp_in_pkts(NetworkLayer_Handle handle) {
    return static_cast<vnx::Networklayer*>(handle)->get_udp_in_pkts();
}

uint32_t vnx_nl_get_udp_out_pkts(NetworkLayer_Handle handle) {
    return static_cast<vnx::Networklayer*>(handle)->get_udp_out_pkts();
}

} // extern "C"