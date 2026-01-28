// #include "vnx_c_wrapper.h"
// #include <stdio.h>

// int Vnx_Innit() 
// {
//     // 1. Khởi tạo
//     CMAC_Handle my_cmac = vnx_cmac_create(0, "cmac_0");
//     NetworkLayer_Handle my_nl = vnx_nl_create(0, "networklayer_0");

//     // 2. Cấu hình IP và Socket
//     vnx_nl_update_ip(my_nl, "192.168.1.100");
//     vnx_nl_configure_socket(my_nl, 0, "192.168.1.101", 62781, 62781, true);
//     vnx_nl_populate_socket_table(my_nl);

//     // 3. Kiểm tra Link
//     if (vnx_cmac_get_link_status(my_cmac)) {
//         printf("Ethernet Link is UP!\n");
//     } else {
//         printf("Ethernet Link is DOWN!\n");
//     }

//     // 4. Đọc stats
//     uint32_t pkts = vnx_nl_get_udp_in_pkts(my_nl);
//     printf("Received UDP packets: %u\n", pkts);

//     // 5. Giải phóng memory
//     vnx_cmac_destroy(my_cmac);
//     vnx_nl_destroy(my_nl);

//     return 0;
// }