#ifndef HAL_CAHCE_H
#define HAL_CAHCE_H

#include "linux_hal.h"

// --- Định nghĩa IOCTL Command và Struct (PHẢI GIỐNG HỆT KERNEL MODULE) ---
#define CACHE_IOCTL_MAGIC 'k'
#define IOCTL_INVALIDATE_RANGE _IOWR(CACHE_IOCTL_MAGIC, 1, struct cache_range)

struct cache_range {
    void *start; // Trong userspace, ta dùng void* bình thường
    size_t size;
};

void cache_invalidate_range(void* virt_addr, size_t size);

#endif