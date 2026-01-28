#include "xgpio_hal.h" // Include file header của chính nó

// Tên định danh cho ứng dụng của bạn
#define GPIO_CONSUMER "fmcw_app"

// =============================================================================
// PHẦN QUAN TRỌNG NHẤT: Bảng Ánh xạ Nội bộ
// =============================================================================

// 1. Cấu trúc nội bộ để quản lý trạng thái bằng libgpiod.
//    Nó KHÔNG PHẢI là XGpio.
typedef struct {
    XGpio *instance_key;            // "Chìa khóa": Con trỏ tới biến XGpio gốc
    const char* chip_name;          // Tên của gpiochip
    struct gpiod_chip *chip;        // Handle của chip
    struct gpiod_line_bulk lines_ch1; // Handle các line của kênh 1
    struct gpiod_line_bulk lines_ch2; // Handle các line của kênh 2
    int width_ch1;                  // Độ rộng kênh 1
    int width_ch2;                  // Độ rộng kênh 2
    bool is_initialized;            // Cờ trạng thái
} GpiodMap;

/************************************************** *
root@xilinx-zcu216-2021_2:~# gpiodetect
gpiochip0 [firmware:zynqmp-firmware:gpio] (4 lines)
gpiochip1 [a0250000.gpio] (6 lines)
gpiochip10 [a02a0000.gpio] (56 lines)
gpiochip11 [zynqmp_gpio] (174 lines)
gpiochip2 [a00d0000.gpio] (2 lines)
gpiochip3 [a00e0000.gpio] (1 lines)
gpiochip4 [a00f0000.gpio] (42 lines)
gpiochip5 [a0100000.gpio] (41 lines)
gpiochip6 [a0110000.gpio] (15 lines)
gpiochip7 [a0120000.gpio] (2 lines)
gpiochip8 [a00c0000.gpio] (1 lines)
gpiochip9 [a0290000.gpio] (56 lines)

**************************************************/

// 2. Bảng ánh xạ: Liên kết giữa con trỏ XGpio gốc và dữ liệu libgpiod
//    Bảng này là "trái tim" của lớp trừu tượng HAL.
static GpiodMap gpiod_hal_table[] = {
    // {&InstanceGoc,      "Tên chip", chip, {0}, {0}, Rộng CH1, Rộng CH2, false},
    {&GPIO_HS1,           "gpiochip4", NULL, {0}, {0}, 10,       32,       false},
    {&GPIO_HS2,           "gpiochip5", NULL, {0}, {0}, 9,        32,       false},
    {&GPIO_HS3,           "gpiochip6", NULL, {0}, {0}, 3,        12,       false},
    {&GPIO_ENDFRAME_IRQ,  "gpiochip2", NULL, {0}, {0}, 2,        0,        false},
    {&GPIO_ENDFRAME_IRQ1, "gpiochip3", NULL, {0}, {0}, 1,        0,        false},
    {&GPIO_SYNC_CMAC,     "gpiochip1", NULL, {0}, {0}, 6,        0,        false},
    {&GPIO_TX_PREAMBLEIN, "gpiochip9", NULL, {0}, {0}, 32,       24,       false},
    {&GPIO_RX_PREAMBLEOUT,"gpiochip10",NULL, {0}, {0}, 32,       24,       false}
};
#define GPIOD_HAL_TABLE_SIZE (sizeof(gpiod_hal_table)/sizeof(gpiod_hal_table[0]))

// khai bao ngat tại: GPIO_ENDFRAME_IRQ1, chân 1
const char *IRQ_chipname = "gpiochip3";
unsigned int IRQ_offset = 0; // Tương ứng với GIC 124 của bạn
struct gpiod_chip *IRQ_chip;
struct gpiod_line *IRQ_line;
struct gpiod_line_event IRQ_event;
int IRQ_ret;

// 3. Hàm helper để tìm kiếm trong bảng ánh xạ
static GpiodMap* find_gpiod_map(XGpio *InstancePtr) 
{
    return &gpiod_hal_table[InstancePtr->index];
}

// =============================================================================
// TRIỂN KHAI LẠI CÁC HÀM API (KHÔNG THAY ĐỔI CHỮ KÝ HÀM)
// =============================================================================

int XGpio_Interrupt_Innit() 
{
    // 1. Mở GPIO chip
    IRQ_chip = gpiod_chip_open_by_name(IRQ_chipname);
    if (!IRQ_chip) {
        perror("Mở chip thất bại");
        return 1;
    }

    // 2. Lấy line (chân GPIO)
    IRQ_line = gpiod_chip_get_line(IRQ_chip, IRQ_offset);
    if (!IRQ_line) {
        perror("Lấy line thất bại");
        gpiod_chip_close(IRQ_chip);
        return 1;
    }

    // 3. Đăng ký bắt sự kiện cả 2 cạnh (Rising và Falling)
    // Nếu chỉ muốn 1 cạnh, dùng: gpiod_line_request_rising_edge_events
    IRQ_ret = gpiod_line_request_rising_edge_events(IRQ_line, "FFT_handler");
    if (IRQ_ret < 0) {
        perror("Yeu cau su kien that bai");
        gpiod_chip_close(IRQ_chip);
        return 1;
    }

    printf("dang cho su kien tren %s line %u (GIC 124)...\n", IRQ_chipname, IRQ_offset);
}

int XGpio_Interrupt_IRQ() 
{
    // 4. Vòng lặp chờ Interrupt
    while (1) 
    {
        // Chờ sự kiện trong tối đa 1 giây (timeout)
        struct timespec timeout = { 1, 0 };
        IRQ_ret = gpiod_line_event_wait(IRQ_line, &timeout);

        if (IRQ_ret < 0) 
        {
            perror("Loi khi đoi su kien");
            break;
        } else if (IRQ_ret == 0) {
            // Timeout - không có ngắt trong 1 giây qua
            continue;
        }

        // 5. Đọc sự kiện
        if (gpiod_line_event_read(IRQ_line, &IRQ_event) < 0) 
        {
            perror("doc su kien that bai");
            break;
        }

        // 6. Xử lý logic của bạn tại đây
        if (IRQ_event.event_type == GPIOD_LINE_EVENT_RISING_EDGE) 
        {
            hal_FFT_IRQ();
            //printf("Su kiun: CANH LEN (Rising) tai %ld.%ld\n", IRQ_event.ts.tv_sec, IRQ_event.ts.tv_nsec);
        } 
        // else 
        // {
        //     printf("Sự kiện: CẠNH XUỐNG (Falling) tại %ld.%ld\n", event.ts.tv_sec, event.ts.tv_nsec);
        // }
    }
}

// =============================================================================
// TRIỂN KHAI LẠI CÁC HÀM API (KHÔNG THAY ĐỔI CHỮ KÝ HÀM)
// =============================================================================

int XGpio_Initialize(XGpio *InstancePtr, uint32_t Chip_index) {
    //(void)EffectiveAddr; // Không dùng đến

    InstancePtr->index = Chip_index;

    GpiodMap* map = find_gpiod_map(InstancePtr);
    if (!map) {
        fprintf(stderr, "Loi: Khong tim thay GPIO instance trong bang HAL.\n");
        return XST_FAILURE;
    }

    if (map->is_initialized) return XST_SUCCESS; // Đã khởi tạo rồi

    

    map->chip = gpiod_chip_open_by_name(map->chip_name);
    if (!map->chip) { /* ... xử lý lỗi ... */ return XST_FAILURE; }

    if (map->width_ch1 > 0) {
        int *offsets = malloc(sizeof(int) * map->width_ch1);
        for (int i = 0; i < map->width_ch1; i++) offsets[i] = i;
        if (gpiod_chip_get_lines(map->chip, offsets, map->width_ch1, &map->lines_ch1) < 0) { /* ... xử lý lỗi ... */ free(offsets); return XST_FAILURE; }
        free(offsets);
    }

    if (map->width_ch2 > 0) {
        int *offsets = malloc(sizeof(int) * map->width_ch2);
        for (int i = 0; i < map->width_ch2; i++) offsets[i] = map->width_ch1 + i;
        if (gpiod_chip_get_lines(map->chip, offsets, map->width_ch2, &map->lines_ch2) < 0) { /* ... xử lý lỗi ... */ free(offsets); return XST_FAILURE; }
        free(offsets);
    }
    
    map->is_initialized = true;
    return XST_SUCCESS;
}

/**
 * @brief  Cấu hình hướng cho một kênh GPIO (Input hoặc Output) và kiểm tra lỗi.
 *
 * @param  InstancePtr   Con trỏ tới instance XGpio.
 * @param  Channel       Kênh để cấu hình (1 hoặc 2).
 * @param  DirectionMask Quy ước của Xilinx: 
 *                       - 0xFFFFFFFF (tất cả bit 1) -> Tất cả là Input.
 *                       - 0x00000000 (tất cả bit 0) -> Tất cả là Output.
 */
void XGpio_SetDataDirection(XGpio *InstancePtr, int Channel, uint32_t DirectionMask) {
    // Tìm cấu trúc map tương ứng
    GpiodMap* map = find_gpiod_map(InstancePtr);
    if (!map || !map->is_initialized) {
        fprintf(stderr, "[HAL ERROR] SetDataDirection: Instance chưa được khởi tạo.\n");
        return;
    }

    // Xác định đúng kênh
    struct gpiod_line_bulk *lines = (Channel == 1) ? &map->lines_ch1 : &map->lines_ch2;
    int width = (Channel == 1) ? map->width_ch1 : map->width_ch2;
    if (width == 0) return; // Bỏ qua nếu kênh không có chân nào

    // Luôn giải phóng các line trước khi request lại với hướng mới.
    // Điều này đảm bảo trạng thái sạch sẽ.
    gpiod_line_release_bulk(lines);

    // Quy ước của Xilinx:
    // - Mask có bất kỳ bit 1 nào được hiểu là Input.
    // - Chỉ khi mask bằng 0 mới được hiểu là Output.
    if (DirectionMask != 0) {
        // --- CẤU HÌNH LÀM INPUT ---
        if (gpiod_line_request_bulk_input(lines, GPIO_CONSUMER) < 0) {
            // Nếu request thất bại, báo lỗi chi tiết
            fprintf(stderr, "[HAL ERROR] gpiod_line_request_bulk_input thất bại trên chip '%s' (Kênh %d): ",
                    map->chip_name, Channel);
            perror(NULL);
        } else {
            // Thông báo thành công (tùy chọn, có thể xóa đi nếu không muốn quá nhiều log)
            printf("[HAL INFO] Chip '%s' (Kênh %d) INPUT.\n",
                   map->chip_name, Channel);
        }
    } else {
        // --- CẤU HÌNH LÀM OUTPUT ---
        // Tham số cuối là giá trị mặc định, NULL có nghĩa là không thay đổi giá trị hiện tại.
        if (gpiod_line_request_bulk_output(lines, GPIO_CONSUMER, NULL) < 0) {
            // Nếu request thất bại, báo lỗi chi tiết
            fprintf(stderr, "[HAL ERROR] gpiod_line_request_bulk_output thất bại trên chip '%s' (Kênh %d): ",
                    map->chip_name, Channel);
            perror(NULL); // In ra lỗi hệ thống, ví dụ "Operation not permitted"
        } else {
            // Thông báo thành công (tùy chọn)
            printf("[HAL INFO] Chip '%s' (Kênh %d) OUTPUT.\n",
                   map->chip_name, Channel);
        }
    }
}

void XGpio_DiscreteWrite(XGpio* InstancePtr, int Channel, uint32_t Data) {
    GpiodMap* map = find_gpiod_map(InstancePtr);
    if (!map || !map->is_initialized) return;

    struct gpiod_line_bulk *lines = (Channel == 1) ? &map->lines_ch1 : &map->lines_ch2;
    int width = (Channel == 1) ? map->width_ch1 : map->width_ch2;
    if (width == 0) return;

    int *values = malloc(sizeof(int) * width);
    for (int i = 0; i < width; i++) values[i] = (Data >> i) & 1;
    
    if (gpiod_line_set_value_bulk(lines, values) < 0) 
    { 
        //perror("gpiod_line_set_value_bulk"); 
        // --- ĐÂY LÀ PHẦN THAY ĐỔI ---
        // Nếu có lỗi, in ra thông báo chi tiết, bao gồm cả tên chip và lỗi hệ thống
        fprintf(stderr, "[HAL ERROR] gpiod_line_set_value_bulk thất bại trên chip '%s' (Kênh %d): ", 
                map->chip_name, Channel);
        perror(NULL); // perror(NULL) sẽ chỉ in ra thông báo lỗi hệ thống (ví dụ: "Operation not permitted")
    }
    free(values);
}


uint32_t XGpio_DiscreteRead(XGpio *InstancePtr, int Channel) {
    GpiodMap* map = find_gpiod_map(InstancePtr);
    if (!map || !map->is_initialized) return 0;
    
    struct gpiod_line_bulk *lines = (Channel == 1) ? &map->lines_ch1 : &map->lines_ch2;
    int width = (Channel == 1) ? map->width_ch1 : map->width_ch2;
    if (width == 0) return 0;

    int *values = malloc(sizeof(int) * width);
    if (gpiod_line_get_value_bulk(lines, values) < 0) { /* ... xử lý lỗi ... */ free(values); return 0; }
    
    uint32_t result = 0;
    for (int i = 0; i < width; i++) {
        if (values[i] == 1) result |= (1 << i);
    }
    free(values);
    return result;
}


void XGpio_CleanupAll(void) {
    for (int i = 0; i < GPIOD_HAL_TABLE_SIZE; i++) {
        if (gpiod_hal_table[i].is_initialized) {
            if (gpiod_hal_table[i].width_ch1 > 0) gpiod_line_release_bulk(&gpiod_hal_table[i].lines_ch1);
            if (gpiod_hal_table[i].width_ch2 > 0) gpiod_line_release_bulk(&gpiod_hal_table[i].lines_ch2);
            gpiod_chip_close(gpiod_hal_table[i].chip);
            gpiod_hal_table[i].is_initialized = false;
        }
    }
}

