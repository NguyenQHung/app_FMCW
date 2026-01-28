#include "HAL_cache.h"

/**
 * @brief Vô hiệu hóa D-Cache cho một vùng nhớ ảo cụ thể.
 * @details Hàm này giao tiếp với kernel module /dev/cache_control để yêu cầu
 *          kernel thực hiện thao tác cache invalidate. Nó mở file thiết bị
 *          một lần và giữ lại file descriptor để tăng hiệu năng.
 * @param virt_addr Địa chỉ ảo bắt đầu của vùng nhớ cần invalidate.
 * @param size Kích thước của vùng nhớ.
 */
void cache_invalidate_range(void* virt_addr, size_t size) {
    // Dùng biến static để chỉ mở file một lần duy nhất
    static int cache_fd = -1;
    
    // Nếu file chưa được mở, hãy mở nó
    if (cache_fd < 0) {
        cache_fd = open("/dev/cache_control", O_RDWR);
        if (cache_fd < 0) {
            perror("FATAL: Could not open /dev/cache_control. Is the driver loaded?");
            // Trong một hệ thống thực, bạn có thể muốn thoát hoặc xử lý lỗi này
            // một cách an toàn hơn.
            return; 
        }
    }
    
    // Chuẩn bị dữ liệu để gửi cho kernel
    struct cache_range range;
    range.start = virt_addr;
    range.size = size;

    // Gửi yêu cầu IOCTL
    if (ioctl(cache_fd, IOCTL_INVALIDATE_RANGE, &range) < 0) {
        perror("ioctl(IOCTL_INVALIDATE_RANGE) failed");
    }
}

// Bạn có thể thêm một hàm cleanup để đóng file descriptor khi chương trình kết thúc
// void cache_control_cleanup() {
//     if (cache_fd >= 0) {
//         close(cache_fd);
//         cache_fd = -1;
//     }
// }