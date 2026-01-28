//#include "xparameters.h"


/************************** Variable Definitions *****************************/

extern volatile int TcpFastTmrFlag;
extern volatile int TcpSlowTmrFlag;
extern struct netif *echo_netif;

// Global Variables to store results and handle data flow
// Global variables for data flow
extern volatile u8			IndArrDone;
extern volatile u32			EthBytesReceived;
extern volatile u8			SendResults;
extern volatile u8   		DMA_TX_Busy;
extern volatile u8			Error;
extern unsigned long int 	Check_IO1;
// Global Variables for Ethernet handling

extern struct udp_pcb 		send_pcb;
extern struct udp_pcb 		send_pcb2;
extern struct udp_pcb 		send_pcb3;
extern struct pbuf * 		psnd;
extern struct pbuf * 		psnd2;
extern struct pbuf * 		psnd3;
extern struct pbuf * 		psnd4;
extern struct pbuf * 		psnd5;
extern struct pbuf * 		psnd6;
extern int 					status;
extern int 					Udp_counters;
