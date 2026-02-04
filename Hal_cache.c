#include "HAL_cache.h"

/**
 * @brief Vô hiệu hóa D-Cache cho một vùng nhớ ảo cụ thể.
 * @details Hàm này giao tiếp với kernel module /dev/cache_control để yêu cầu
 *          kernel thực hiện thao tác cache invalidate. Nó mở file thiết bị
 *          một lần và giữ lại file descriptor để tăng hiệu năng.
 * @param virt_addr Địa chỉ ảo bắt đầu của vùng nhớ cần invalidate.
 * @param size Kích thước của vùng nhớ.
 */
void cache_invalidate_range(void *virt_addr, size_t size) {
  static int cache_fd = -2; // -2: Chưa khởi tạo, -1: Lỗi/Không có driver

  // 1. Memory Barrier: Đảm bảo CPU hoàn thành mọi lệnh đọc/ghi trước đó
  __asm__ volatile("dmb sy" ::: "memory");

  // 2. Thử dùng Driver nếu có
  if (cache_fd == -2) {
    cache_fd = open("/dev/cache_control", O_RDWR);
    if (cache_fd < 0) {
      printf("[CACHE] Info: /dev/cache_control not found. Assuming Coherent "
             "Hardware.\n");
      cache_fd = -1; // Đánh dấu là không có driver để không thử lại
    }
  }

  if (cache_fd >= 0) {
    struct cache_range range;
    range.start = virt_addr;
    range.size = size;
    ioctl(cache_fd, IOCTL_INVALIDATE_RANGE, &range);
  }
}

void cache_flush_range(void *virt_addr, size_t size) {
  static int cache_fd = -2;

  // 1. Memory Barrier: Đảm bảo dữ liệu đã được ghi xuống Buffer trước khi báo
  // DMA
  __asm__ volatile("dmb sy" ::: "memory");

  // 2. Thử dùng Driver nếu có (Flush thường dùng cùng IOCTL hoặc IOCTL khác, ở
  // đây ta dùng chung logic driver) Lưu ý: Nếu driver hỗ trợ flush riêng, cần
  // define IOCTL_FLUSH_RANGE. Tuy nhiên với Coherent, write buffer drain (dmb)
  // là quan trọng nhất.

  if (cache_fd == -2) {
    cache_fd = open("/dev/cache_control", O_RDWR);
    if (cache_fd < 0) {
      // printf("[CACHE] Flush: Driver not found. Relying on barriers.\n");
      cache_fd = -1;
    }
  }

  // Nếu có driver và cần Clean/Flush:
  // if (cache_fd >= 0) { ... }
}

// Bạn có thể thêm một hàm cleanup để đóng file descriptor khi chương trình kết
// thúc void cache_control_cleanup() {
//     if (cache_fd >= 0) {
//         close(cache_fd);
//         cache_fd = -1;
//     }
// }