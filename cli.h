#ifndef SRC_CLI_H_
#define SRC_CLI_H_

/***************************** Include Files *********************************/
#include <stdint.h>
#include <string.h>
#include "linux_hal.h"  // Chứa các định nghĩa u32, u8, XST_SUCCESS...

/******************** Constant Definitions **********************************/
#define MAX_CMD_VALS    3
#define CMD_STRING_LEN  56
#define CMD_HELP_LEN    56
#define LINEMAX         81
#define TBL_PTR_MAX     100

/**************************** Type Definitions *******************************/

// Cấu trúc lệnh CLI
typedef struct cmdStruct {
    char cmdString[CMD_STRING_LEN];
    char cmdHelp[CMD_HELP_LEN];
    unsigned char valCnt;
    void (*cmdFunc)(u32 *); // Thay đổi để nhận mảng giá trị tham số
} CMDSTRUCT;

// Cấu trúc bảng lệnh
typedef struct tblStruct {
    CMDSTRUCT *cmdPointer;
    int cmdCount;
} TBLSTRUCT;

/************************** Variable Definitions *****************************/

extern u32 cmdVals[MAX_CMD_VALS];
extern unsigned char numCmdVals;
extern int cmdExitVal;
extern char line[LINEMAX];
extern char lineOld[LINEMAX];
extern char *cmdPtr;
extern char seps[];

/************************** Function Prototypes ******************************/

// --- Các hàm điều khiển CLI core ---
void cli_init(void);
void cli_addCmds(CMDSTRUCT *cmds2add, int numCmds);
char* getAndParseInput(void);
int cmdParseAndRun(void);
void doHelp(void);
void doExit(void);
void cmdComment(void);

// --- Các hàm hỗ trợ nhập liệu chuẩn Linux ---
void enableRawMode(void);
void disableRawMode(void);
char inbyte(void);

// --- Các hàm kiểm tra UART (Đã loại bỏ BaseAddress) ---
u8  myChar_getRx(void);
int myChar_checkRx(void);

// --- Các hàm tiện ích (Sử dụng thư viện chuẩn Linux sẽ tốt hơn) ---
int my_sscanf(char *tokenPtr, u32 *value);

// Lưu ý: Các hàm my_strcpy, my_strcat nên thay thế trực tiếp bằng 
// strcpy, strcat của thư viện <string.h> để đạt hiệu năng cao nhất.

#endif /* SRC_CLI_H_ */

// /*
//  * cli.h
//  *
//  */

// #ifndef SRC_CLI_H_   /* prevent circular inclusions */
// #define SRC_CLI_H_   /* by using protection macros */

// /***************************** Include Files *********************************/
// //#include "xil_types.h"
// #include "linux_hal.h"      // Lớp trừu tượng phần cứng (sysfs GPIO)

// /******************** Constant Definitions **********************************/
// //maximum number of values for a command
// #define MAX_CMD_VALS    3
// #define CMD_STRING_LEN 56
// #define CMD_HELP_LEN   56

// // Max cli command line length
// #define LINEMAX 81

// // Max number of cli command table indexes
// #define TBL_PTR_MAX 100



// /**************************** Type Definitions *******************************/

// //cmd interface
// typedef struct cmdStruct {
// 	char cmdString[CMD_STRING_LEN];
// 	char cmdHelp[CMD_HELP_LEN];
// 	unsigned char valCnt;
// 	void (*cmdFunc)();
// } CMDSTRUCT;

// //cli cmd table structure
// typedef struct tblStruct {
// 	CMDSTRUCT *cmdPointer;
// 	int cmdCount;
// } TBLSTRUCT;

// /***************** Macros (Inline Functions) Definitions *********************/

// /************************** Variable Definitions *****************************/

// extern u32 cmdVals[MAX_CMD_VALS];
// extern CMDSTRUCT cmdtab[];
// extern unsigned char numCmdVals;
// extern int cmdExitVal;

// /************************** Function Prototypes ******************************/

// void cmdComment(void);		//null command

// void cli_init(void);
// void cli_addCmds(CMDSTRUCT *cmds2add, int numCmds);
// void cli_cpyCmds(CMDSTRUCT *cmds2add, CMDSTRUCT *cmdsTable, int curCliCmds, int numCmds);

// int my_sscanf(char *tokenPtr, u32 *value);
// char* getAndParseInput( void );
// void doHelp(void);
// int cmdParseAndRun( void );
// void doExit(void);

// u8 myChar_getRx(u32 BaseAddress);
// int myChar_checkRx(u32 BaseAddress);

// char *my_strcpy(char *DestPtr, const char *SrcPtr);
// char * my_strcat(char* Str1Ptr, const char* Str2Ptr);

// #endif /* SRC_CLI_H_ */

