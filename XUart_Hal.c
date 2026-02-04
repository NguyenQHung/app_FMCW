#include "XUart_Hal.h"

// Hàm helper để cấu hình các thông số Serial (Baudrate 115200, 8N1)
int uart_configure(int fd) {
    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) return -1;

    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars
    tty.c_iflag &= ~IGNBRK;                     // disable break processing
    tty.c_lflag = 0;                            // no echoing, no canonical processing
    tty.c_oflag = 0;                            // no remapping, no delays
    tty.c_cc[VMIN]  = 0;                        // read doesn't block
    tty.c_cc[VTIME] = 5;                        // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);     // shut off xon/xoff ctrl
    tty.c_cflag |= (CLOCAL | CREAD);            // ignore modem controls, enable reading
    tty.c_cflag &= ~(PARENB | PARODD);          // shut off parity
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) return -1;
    return 0;
}

// Hàm khởi tạo UART chung cho Linux
int XUartLite_Initialize_Linux(XUartLite *Instance, const char* path) {
    Instance->device_path = path;
    Instance->fd = open(path, O_RDWR | O_NOCTTY | O_SYNC);
    
    if (Instance->fd < 0) {
        fprintf(stderr, "Error opening %s: %s\n", path, strerror(errno));
        return XST_FAILURE;
    }

    if (uart_configure(Instance->fd) < 0) {
        close(Instance->fd);
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

// Các hàm Init cụ thể (Refactored)
int UART_Generic_Init(XUartLite *Instance, const char* path, const char* name) {
    if (XUartLite_Initialize_Linux(Instance, path) == XST_SUCCESS) {
        printf("%s ok (%s)\n", name, path);
        return XST_SUCCESS;
    } else {
        printf("%s error\n", name);
        return XST_FAILURE;
    }
}