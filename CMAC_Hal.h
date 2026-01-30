#ifndef CMAC_HAL_H
#define CMAC_HAL_H

#include "linux_hal.h"   // Lớp trừu tượng phần cứng (sysfs GPIO)

// --- OFFSETS THANH GHI CẤU HÌNH ---
#define CORE_VERSION_REG    0x0024
#define CORE_MODE_REG       0x0020
#define GT_LOOPBACK_REG     0x0090
#define CONFIG_TX_REG1      0x000C
#define CONFIG_RX_REG1      0x0014

// --- OFFSETS THANH GHI TRẠNG THÁI ---
#define STAT_TX_STATUS_REG      0x0200   // TX Local Fault
#define STAT_RX_STATUS_REG      0x0204   // RX Status, Aligned, Hi_BER, Remote_Fault
#define STAT_STATUS_REG1        0x0208   // PTP/FIFO Errors
#define STAT_RX_BLOCK_LOCK      0x020C   // 20-bit Block Lock
#define STAT_RX_LANE_SYNC       0x0210   // 20-bit Lane Sync
#define STAT_RX_LANE_ERR        0x0214   // 20-bit Sync Errors
#define STAT_RX_PCSL_DEMUXED    0x0224   // 20-bit PCS Lane Demuxed
#define STAT_RSFEC_STATUS_REG   0x1004   // RS-FEC Status (Nếu dùng CAUI-4/RS-FEC)
#define STAT_TICK_REG           0x02B0   // Snapshot Bo dem thong ke

// --- OFFSETS RS-FEC (Mới thêm) ---
#define RSFEC_CONFIG_IND_CORR  0x1000
#define RSFEC_STATUS_REG       0x1004
#define RSFEC_CONFIG_ENABLE    0x107C

// ==========================================================
// CẤU HÌNH ĐỊA CHỈ VÀ OFFSET (Dựa trên PG203)
// ==========================================================

// Thanh ghi cấu hình (Table 5-5)
#define REG_TX_CONFIG           0x000C   // ctl_tx_enable, ctl_tx_send_rfi
#define REG_RX_CONFIG           0x0014   // ctl_rx_enable
#define REG_TX_CUSTOM_PREAMBLE  0x0159
#define REG_CORE_MODE           0x0020   // Chế độ hoạt động
#define REG_CORE_VERSION        0x0024   // Phiên bản IP
#define REG_GT_LOOPBACK         0x0090   // Cấu hình Loopback
#define REG_USER_REG0           0x00CC   // Thường dùng điều khiển Polarity trong Example Design
#define REG_SOFT_RESET          0x0004   // Reset TX/RX core (usr_tx_reset, usr_rx_reset)
#define REG_GT_RESET            0x0000   // gt_reset_all (Table 5-7)

// Thanh ghi trạng thái (Table 5-6)
#define STAT_TX_STATUS          0x0200   // Trạng thái TX (Fault)
#define STAT_RX_STATUS          0x0204   // Trạng thái RX chính (0x3 là mục tiêu)
#define STAT_STATUS_REG1        0x0208   // Lỗi PTP/FIFO
#define STAT_RX_BLOCK_LOCK      0x020C   // Khóa khối 20 lane (20 bit)
#define STAT_RX_LANE_SYNC       0x0210   // Đồng bộ lane PCS (20 bit)
#define STAT_RX_LANE_ERR        0x0214   // Lỗi đồng bộ lane PCS (20 bit)
#define STAT_RX_PCSL_DEMUXED    0x0224   // Trạng thái Demux 20 lane

// Thanh ghi RS-FEC (Table 5-43, 5-44)
#define REG_RSFEC_CONFIG_ENABLE 0x107C   // Bật/Tắt FEC
#define REG_RSFEC_IND_CORR      0x1000   // Cấu hình sửa lỗi FEC
#define STAT_RSFEC_STATUS       0x1004   // Trạng thái FEC (Alignment Lock)
#define STAT_FEC_CORRECTED      0x1008   // Số block FEC đã sửa lỗi
#define STAT_FEC_UNCORRECTED    0x1010   // Số block FEC không thể sửa

// Các Offset bổ sung cho chẩn đoán sâu
#define STAT_FEC_CORRECTED_LSB      0x1008
#define STAT_FEC_UNCORRECTED_LSB    0x1010
// API

void CMAC_Innit();
void CMAC_Innit_by_HW();
void smart_init_cmac();
void* CMAC_RXSYNC_Check_Thread(void* arg);
void process_cmac_100g_sr4_full();
void final_check();
void check_signal_stability();
void final_link_up_force();
void test_internal_loopback_with_fec();
void force_tx_and_check_pc();

void run_cmac_test_pattern(int enable);
void check_pattern_errors();

void setup_advanced_cmac_features();
void test_custom_preamble();
void check_rx_protocol_errors();
void Check_CMAC_Pin();
void CMAC_Reset_rx_datapath();
void Set_CMAC_Hardware_Loopback(uint32_t mode_val, const char* mode_name);
void Test_All_Loopback_Modes();
#endif