#include "Hal_cache.h"

// #include <sys/ioctl.h>
// #include <unistd.h>
// // #include "my_cache_driver.h" // Chứa các hằng số ioctl

// // Giả định các hằng số ioctl (chỉ là ví dụ)
// #define CACHE_CLEAN      _IOWR('k', 1, struct cache_info)
// #define CACHE_INVALIDATE _IOWR('k', 2, struct cache_info)

// // Cấu trúc dữ liệu để truyền qua ioctl
// struct cache_info {
//     uint64_t phys_addr; // Địa chỉ vật lý
//     size_t length;      // Độ dài vùng nhớ
// };

// // Hàm giả định: Bạn phải biết file descriptor nào tương ứng với vùng nhớ.
// // Ở đây, tôi giả sử bạn sẽ dùng fd_cache đã mở /dev/mem
// extern int fd_cache; // Giả sử fd_cache được làm biến toàn cục

// int cache_clean_range(void* virt_addr, size_t size) {
//     uint64_t phys_addr = 0; // Cần hàm để chuyển đổi Virt -> Phys

//     // === THÁCH THỨC LỚN: Chuyển đổi Virt -> Phys trong Userspace ===
//     // Để DMA hoạt động chính xác, bạn cần địa chỉ VẬT LÝ để truyền cho ioctl.
//     // Việc chuyển đổi địa chỉ ảo mmaped sang vật lý là phức tạp (thường dùng /proc/self/pagemap).
    
//     // Nếu bạn biết địa chỉ vật lý, bạn sẽ làm thế này:
//     if (virt_addr == (void*)BRAM_Get_Virt_Addr(IDX_DMA_FFT_DESC)) {
//         phys_addr = BRAM_Get_Phys_Addr(IDX_DMA_FFT_DESC); // Sử dụng hàm có sẵn của bạn
//     }
//     // ... logic cho các vùng khác ...

//     struct cache_info info = {
//         .phys_addr = phys_addr,
//         .length = size
//     };

//     // Gọi ioctl trên file descriptor đã mở
//     // Rất khó để dùng fd_cache (mở /dev/mem) để gọi ioctl cache
//     // Cần phải gọi ioctl trên một file descriptor của DRIVER quản lý cache
//     // if (ioctl(dma_fd_driver, CACHE_CLEAN, &info) < 0) {
//     //     perror("ioctl CACHE_CLEAN failed");
//     //     return -1;
//     // }
    
//     // --> Kết luận: Không thể triển khai hàm này mà không có Kernel Driver.
    
//     // Nếu bạn muốn tiếp tục mà KHÔNG cần driver, bạn phải trở lại phương pháp:
//     // 1. Xóa `dma-coherent;` khỏi Device Tree.
//     // 2. Map Descriptors với O_SYNC (Non-Cached) như code ban đầu.

//     printf("[CACHE] WARNING: Cache operation skipped. Check your cache management implementation.\n");
//     return 0;
// }

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
