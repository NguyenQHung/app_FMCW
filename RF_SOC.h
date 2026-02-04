
#include "main.h"
#include "linux_hal.h"      // Lớp trừu tượng phần cứng (sysfs GPIO)
/************************** Function Prototypes ******************************/

void my_metal_default_log_handler(enum metal_log_level level, const char *format, ...);

//static
int resetAllClk104(void);
void reverse32bArray(u32 *src, int size);
void printCLK104_settings(void);

//int SPI_StandMode_Config_Linux(const char* device, uint32_t speed, uint8_t mode);
//int Setup_SPI_Processing_Linux(int fd, uint8_t *TxBuffer, uint8_t Bytecount);
void Start_Test_System(void);
