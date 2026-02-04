#include "main.h"

void xemacif_input_blocking(UdpConnection *conn);
void* udp_control_thread(void *arg);
void _UDP_Innit_Buffer(void);
void _UDP_Process_Bam_Sat_PC1(void);
void _UDP_Process_Bam_Sat_PC8(void);
void _UDP_Update_Click_Point(void);
void _UDP_Process_Phase(void);
void _UDP_PC8_Send(void);
void _UDP_PC1_Send(void);
void _UDP_PC2_Send(void);
