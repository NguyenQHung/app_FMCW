# Tên của file thực thi cuối cùng
TARGET = fmcw_app

# --- Phần cấu hình Toolchain và Sysroot ---
# Sử dụng trực tiếp các biến môi trường được cung cấp bởi script environment-setup-* của SDK.
# Điều này đảm bảo chúng ta dùng đúng trình biên dịch, cờ tối ưu hóa và sysroot.
CC := $(CC)
CXX := $(CXX)
LD := $(LD)

# --- Phần cấu hình cờ biên dịch và liên kết ---
# Lấy các cờ mặc định từ môi trường SDK và thêm các cờ của riêng chúng ta vào.
# CFLAGS dùng cho file C. CXXFLAGS dùng cho file C++. CPPFLAGS dùng cho cả hai.

# Cờ cho Preprocessor (dùng cho cả C và C++).
# Thêm tất cả các thư mục con chứa file .h vào đây.
# -I. : Thư mục gốc
# -I./include, -I./rfdc, -I./metal : Các thư mục con cụ thể
# -I$(SDKTARGETSYSROOT)/usr/include/libmetal : Thêm thủ công đường dẫn header của libmetal
CPPFLAGS := $(CPPFLAGS) -I. -I./rfdc -I$(SDKTARGETSYSROOT)/usr/include

# Cờ riêng cho trình biên dịch C
CFLAGS := $(CFLAGS) -Wall -O2 -g

# Cờ riêng cho trình biên dịch C++
CXXFLAGS := $(CXXFLAGS) -Wall -O2 -g

# Cờ cho Linker.
# -lm : Liên kết với thư viện toán học
# -lmetal : Liên kết với thư viện libmetal
LDFLAGS := $(LDFLAGS) -lm -lmetal -lgpiod -pthread

# --- Phần tự động tìm kiếm file mã nguồn ---
# Tìm tất cả các file .c trong thư mục hiện tại và tất cả các thư mục con
SRCS := $(shell find . -name '*.c')
# Nếu có file C++, hãy thêm dòng này:
# SRCS += $(shell find . -name '*.cpp')

# Tạo danh sách các file object (.o) và file phụ thuộc (.d) tương ứng
OBJS := $(SRCS:.c=.o)
DEPS := $(OBJS:.o=.d)


# --- Phần định nghĩa các quy tắc (Rules) ---

# Quy tắc mặc định: build tất cả
all: $(TARGET)

# Quy tắc liên kết (linking) để tạo ra file thực thi cuối cùng
$(TARGET): $(OBJS)
	@echo "Linking target: $@"
	$(CC) -o $@ $^ $(LDFLAGS)

# Quy tắc biên dịch một file .c thành một file .o
# Nó cũng tự động tạo ra một file phụ thuộc .d (.d file)
%.o: %.c
	@echo "Compiling: $<"
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@ -MMD -MP -MF"$(@:%.o=%.d)"

# Quy tắc dọn dẹp
clean:
	@echo "Cleaning project..."
	rm -f $(TARGET) $(OBJS) $(DEPS)

# Bao gồm tất cả các file phụ thuộc đã được tạo ra
-include $(DEPS)

# Đánh dấu các quy tắc không phải là file
.PHONY: all clean