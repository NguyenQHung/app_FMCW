Hướng Dẫn Cấu Hình và Biên Dịch Dự Án C Cross-Compile cho ARM trên WSL & VS Code
lênh build: 
make
make clean

trên terminal, đánh lệnh load trình biên dịch:
source /opt/pkg/petalinux/sdk/environment-setup-cortexa72-cortexa53-xilinx-linux

copy xuống kit:
root@xilinx-zcu216-2021_2:~# scp fmcw_app root@192.168.1.5:/home/root/                 
The authenticity of host '192.168.1.5 (192.168.1.5)' can't be established.
ECDSA key fingerprint is SHA256:5P/PK5RCDKw430K5TT3uaD6JmWuThdNZkx66Ns6/Re4.
Are you sure you want to continue connecting (yes/no/[fingerprint])? yes
Warning: Permanently added '192.168.1.5' (ECDSA) to the list of known hosts.
root@192.168.1.5's password: 
Permission denied, please try again.
root@192.168.1.5's password: 
fmcw_app                                                                                                                                                                  100% 2158KB  33.8MB/s   00:00    
root@xilinx-zcu216-2021_2:~# chmod +x /home/root/fmcw_app
root@xilinx-zcu216-2021_2:~# /home/root/fmcw_app
ADDR_CMAC_REG: 0xA0270000 
ADDR_DMA_CMAC: 0xA0280000 
ADDR_DMA_ADC: 0xA0240000 
ADDR_DMA_FFT: 0xA01C0000 
PHYS_ADC_BUF: 0x40000000 
PHYS_FFT_DESC: 0x50000000 
PHYS_FFT_BUF: 0x50001000
PHYS_CMAC_BUF: 0x60000000
[HAL] Successfully mapped physical address 0xA0000000 -> virtual memory.
 hal_init ok
IP Bo mach: 192.168.1.5
IP PC: 192.168.1.10
Radar Data innit ok
Innit_Radar_Debug ok
Uart0 ok (/dev/ttyUL1)
Uart1 ok (/dev/ttyUL2)
Uart2 ok (/dev/ttyUL3)
Uart3 ok (/dev/ttyUL4)
All SPI devices initialized successfully.
[HAL] XGpioPs_CfgInitialize (gpiod) - OK.
XGpioPs_Setup devices
psgpio ok
SFP_LPMODE = 0
SFP_MODE_SEL = 0
SFP_RESET = 1
[HAL INFO] Chip 'gpiochip2' (Kênh 1) INPUT  2 pin.
GPIO_ENDFRAME_IRQ ok
[HAL INFO] Chip 'gpiochip4' (Kênh 1) OUTPUT  10 pin.
[HAL INFO] Chip 'gpiochip4' (Kênh 2) OUTPUT  32 pin.
GPIO_HS1 ok
[HAL INFO] Chip 'gpiochip5' (Kênh 1) OUTPUT  9 pin.
[HAL INFO] Chip 'gpiochip5' (Kênh 2) INPUT  32 pin.
GPIO_HS2 ok
[HAL INFO] Chip 'gpiochip6' (Kênh 1) OUTPUT  3 pin.
[HAL INFO] Chip 'gpiochip6' (Kênh 2) INPUT  12 pin.
GPIO_HS3 ok
[HAL INFO] Chip 'gpiochip1' (Kênh 1) INPUT  8 pin.
[HAL INFO] Chip 'gpiochip1' (Kênh 2) OUTPUT  1 pin.
GPIO_SYNC_CMAC ok
[HAL INFO] Chip 'gpiochip9' (Kênh 1) OUTPUT  12 pin.
GPIO_TX_PREAMBLEIN ok
[HAL INFO] Chip 'gpiochip10' (Kênh 1) OUTPUT  3 pin.
GPIO_RX_PREAMBLEOUT ok
[CAN] Initializing all CAN buses...
[CAN] Interface can0 initialized successfully.
[CAN] Interface can1 initialized successfully.
ADF4159_Innit done
ADF4360_Innit done
pthread_create(&thread_CMAC, NULL, force_tx_and_check_pc, NULL) => done 
smart_init_cmac...

###############################################
Hello RFSoC World!

RFDC IP Version: 2.6

Configuring LMK and LMX devices for RFSOC-SOM

==================================================
 CMAC Innit - Start Bring-up Sequence
==================================================

[LOOPBACK CMD] Setting Hardware Loopback: Normal Mode (External) (Val: 0x000)
[LOOPBACK CMD] Toggling GT RX Reset to re-lock CDR...

======================================================
     QSFP28 MODULE FULL REGISTER DUMP & ANALYSIS
======================================================
--- Raw Register Dump (256 Bytes) ---
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f    0123456789abcdef
00: 11 08 00 00 00 00 01 00 00 00 00 00 00 00 00 00    ................
10: 00 00 00 00 00 00 2b 74 00 00 81 f2 00 00 00 00    ......+t........
20: 00 00 26 19 26 ff 25 3b 18 d2 0e c3 0e a4 0e a4    ..&.&.%;........
30: 0e a4 28 5b 29 e7 24 dc 26 61 00 00 00 00 00 00    ..([).$.&a......
40: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
60: 00 00 ff 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
70: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
80: 11 8c 0c 80 00 00 00 00 00 00 00 07 ff 00 00 23    ...............#
90: 00 00 32 00 44 45 4c 4c 20 45 4d 43 20 20 20 20    ..2.DELL EMC
a0: 20 20 20 20 00 00 90 65 46 54 4c 43 39 35 35 35        ...eFTLC9555
b0: 52 45 50 4d 33 2d 45 35 42 31 42 68 07 d0 55 e7    REPM3-E5B1Bh..U.
c0: 02 07 ff de 58 39 4d 41 59 51 37 20 20 20 20 20    ....X9MAYQ7
d0: 20 20 20 20 32 33 30 36 32 30 20 20 3c 10 67 26        230620  <.g&
e0: 20 30 31 39 2d 30 37 38 2d 30 35 38 20 41 30 31     019-078-058 A01
f0: df 10 01 11 11 00 10 00 00 00 00 00 00 00 00 00    ................

--- Key Register Analysis (Page 00h) ---
[Byte 02] Status Flags      : 0x00
  [FAIL] Data Not Ready (Bit 7): Module is busy or in low-power state.
[Byte 03] Channel Status    : 0x00
  [ OK ] RX LOS: All lanes have optical signal.
  [ OK ] TX Fault: No transmitter faults detected.

--- Static Information (Upper Page 00h) ---
[Byte 148-163] Vendor Name      : DELL EMC
[Byte 168-183] Vendor PN        : FTLC9555REPM3-E5
[Byte 196-211] Vendor SN        : X9MAYQ7

--- Overall Conclusion ---
=> LỖI NGHIÊM TRỌNG: Module quang báo 'Data Not Ready'.
   Nguyên nhân: Module đang bị khóa ở chế độ Low Power hoặc chưa khởi động xong.
   Hành động: 1. Thử Reset mềm module (ghi 0x80 vào 0x7B rồi ghi 0x00).
              2. Kiểm tra lại nguồn cấp cho module.
======================================================
[DEBUG] RX High Alarm Threshold: 3.40 dBm
[DEBUG] Current RX Power Lane 1: 8.08 dBm
=> CANH BAO: Module dang bi QUA TAI QUANG (Overload). Logic bi khoa la hien nhien.
[SFP RESET] Performing module soft reset...
[SFP RESET] Module reset complete.

--- SFP MODULE POWER-UP AND CMAC ALIGNMENT ---
[SFP INIT] Current Control Byte (0x7B): 0x00. New Value: 0x00 (Force HP Mode).
[CMAC ALIGN] Waiting for CMAC to Align...
[SUCCESS] Link UP after 0 attempts. (Status 0x03).

--- Reading QSFP28 Optical Power (DDM) ---
[INFO] Selecting Page 03h (writing 0x03 to 0x7F)...

--- Received Optical Power ---
Lane 1: 0xFB00 -> 6.4256 mW -> 8.08 dBm
Lane 2: 0x0000 -> 0.0000 mW -> -40.00 dBm
Lane 3: 0x0000 -> 0.0000 mW -> -40.00 dBm
Lane 4: 0x0000 -> 0.0000 mW -> -40.00 dBm

--- Transmitted Optical Power ---
Lane 1: 0x0000 -> 0.0000 mW -> -40.00 dBm
Lane 2: 0x5576 -> 2.1878 mW -> 3.40 dBm
Lane 3: 0x018E -> 0.0398 mW -> -14.00 dBm
Lane 4: 0x43E2 -> 1.7378 mW -> 2.40 dBm

[INFO] Returning to Page 00h (writing 0x00 to 0x7F)...
----------------------------------------
===========================================
 STARTING HARDWARE LOOPBACK TEST SEQUENCE
===========================================

[LOOPBACK CMD] Setting Hardware Loopback: Near-End PMA (Analog) (Val: 0x492)
[LOOPBACK CMD] Toggling GT RX Reset to re-lock CDR...
>>> RESULT: NE PMA Loopback FAILED. (Check GT RefClk or Equalizer)
Done with settings of LMK04832

[LOOPBACK CMD] Setting Hardware Loopback: Near-End PCS (Digital) (Val: 0x249)
[LOOPBACK CMD] Toggling GT RX Reset to re-lock CDR...
>>> RESULT: NE PCS Loopback FAILED. (Core Logic/Clock Issue)

[LOOPBACK CMD] Setting Hardware Loopback: Normal Mode (External) (Val: 0x000)
[LOOPBACK CMD] Toggling GT RX Reset to re-lock CDR...
>>> RESULT: External Link FAILED. (Check Cable/Module)
===========================================
[CMAC CONFIG] CORE_MODE_REG (0x0020) = 0x00000001
[CMAC CONFIG] GT_LOOPBACK_REG (0x0090) = 0x00000001
[CMAC CONFIG] CONFIG_TX_REG1 (0x000C) = 0x00000000
[CMAC] Waiting for RX Alignment (stat_rx_aligned = 1)...

--- QSFP28 MODULE DIAGNOSTICS (I2C-1 @ 0x50) ---
[STATUS] SFF-8636 Byte 2 (Interrupt Flags) = 0x02
 [FAIL] Module Not Ready (Bit 7 = 0). Cannot Trust Data.
[STATUS] SFF-8636 Byte 3 (Warning Flags)   = 0x00
[STATUS] SFF-8636 Byte 12 (LOS/TxFault) = 0x00

--- CMAC LINK STATUS DEDUCTION ---
 [ OK ] RX LOS (Lane 0-3): All LANS have signal.
 [ OK ] TX Fault (Lane 0-3): No TX Fault detected.

=> KET LUAN: MODULE BÁO LỖI TÍN HIỆU QUANG. LỖI NGOÀI.
--------------------------------------------------
[CMAC Status] : stat_rx_aligned=0, stat_tx_local_fault=0, interal_rx_local_fault=1, stat_rx_local_fault=1, stat_rx_miss_aligned=0, stat_tx_bad_fcs=0
[CMAC Status] : SFP_intl=1, SFP_MODPRSL=0
[CMAC Status Reg] : 0x0204 = 0x00000003 (LATCHED VAL)
[CMAC] Performing RX Datapath Toggle Reset...
[CMAC] Performing RX Datapath Toggle Reset...
Done with settings of LMX2820 PLL1
[CMAC] Performing RX Datapath Toggle Reset...
[CMAC] Performing RX Datapath Toggle Reset...
[CMAC] Performing RX Datapath Toggle Reset...

--- QSFP28 MODULE DIAGNOSTICS (I2C-1 @ 0x50) ---
[STATUS] SFF-8636 Byte 2 (Interrupt Flags) = 0x02
 [FAIL] Module Not Ready (Bit 7 = 0). Cannot Trust Data.
[STATUS] SFF-8636 Byte 3 (Warning Flags)   = 0x00
[STATUS] SFF-8636 Byte 12 (LOS/TxFault) = 0x00

--- CMAC LINK STATUS DEDUCTION ---
 [ OK ] RX LOS (Lane 0-3): All LANS have signal.
 [ OK ] TX Fault (Lane 0-3): No TX Fault detected.

=> KET LUAN: MODULE BÁO LỖI TÍN HIỆU QUANG. LỖI NGOÀI.
--------------------------------------------------
[CMAC Status] : stat_rx_aligned=0, stat_tx_local_fault=0, interal_rx_local_fault=1, stat_rx_local_fault=1, stat_rx_miss_aligned=0, stat_tx_bad_fcs=0
[CMAC Status] : SFP_intl=1, SFP_MODPRSL=0
[CMAC Status Reg] : 0x0204 = 0x00000003 (LATCHED VAL)
[CMAC] Performing RX Datapath Toggle Reset...
[CMAC] Performing RX Datapath Toggle Reset...
Done with settings of LMX2820 PLL2
Done with settings of LMK/X clocks

The design is ready to use.
Open XSCT or the terminal to run commands.
###############################################

--------------- Entering main() ---------------
=== Metal log enabled ===
metal: debug:     added page size 4096 @/tmp
metal: debug:     registered platform bus
metal: debug:     registered pci bus
metal: info:      metal_linux_dev_open: checking driver vfio-platform,a0000000.usp_rf_data_converter,(null)
metal: debug:     bound device a0000000.usp_rf_data_converter to driver uio_pdrv_genirq
metal: debug:     opened platform:a0000000.usp_rf_data_converter as /dev/uio4
metal: debug:     metal_uio_read_map_attr():79 offset = 0
metal: debug:     metal_uio_read_map_attr():79 addr = 0xa0000000
metal: debug:     metal_uio_read_map_attr():79 size = 0x40000
metal: info:      metal_linux_dev_open: checking driver vfio-platform,a0000000.usp_rf_data_converter,(null)
metal: debug:     bound device a0000000.usp_rf_data_converter to driver uio_pdrv_genirq
metal: debug:     opened platform:a0000000.usp_rf_data_converter as /dev/uio4
metal: debug:     metal_uio_read_map_attr():79 offset = 0
metal: debug:     metal_uio_read_map_attr():79 addr = 0xa0000000
metal: debug:     metal_uio_read_map_attr():79 size = 0x40000

DeviceID: 0
Silicon Revision: 1
metal: info:      metal_linux_dev_open: checking driver vfio-platform,a0000000.usp_rf_data_converter,(null)
metal: debug:     bound device a0000000.usp_rf_data_converter to driver uio_pdrv_genirq
metal: debug:     opened platform:a0000000.usp_rf_data_converter as /dev/uio4
metal: debug:     metal_uio_read_map_attr():79 offset = 0
metal: debug:     metal_uio_read_map_attr():79 addr = 0xa0000000
metal: debug:     metal_uio_read_map_attr():79 size = 0x40000

###############################################
Data Converter startup up in progress...
RF Data Converters Powered up.
[CMAC] Performing RX Datapath Toggle Reset...
[CMAC] Performing RX Datapath Toggle Reset...

The Power-on sequence step. 0xF is complete.
   DAC Tile0 Power-on Sequence Step: 0x0000000f
   DAC Tile1 Power-on Sequence Step: 0x0000000f
   DAC Tile2 Power-on Sequence Step: 0x0000000f
   DAC Tile3 Power-on Sequence Step: 0x0000000f
   ADC Tile0 Power-on Sequence Step: 0x0000000f
   ADC Tile1 Power-on Sequence Step: 0x0000000f
   ADC Tile2 Power-on Sequence Step: 0x0000000f
   ADC Tile3 Power-on Sequence Step: 0x0000000f

Data Converter power up is complete!
###############################################

###############################################
=== Data Converter Status Report ===
DAC Status
   Tile: 0 ready
   Tile: 1 ready
   Tile: 2 ready
   Tile: 3 ready
ADC Status
   Tile: 0 ready
   Tile: 1 ready
   Tile: 2 ready
   Tile: 3 ready
###############################################

DAC[0,0]= 29968 uA
DAC[0,1]= 29968 uA
DAC[0,2]= 29968 uA
DAC[0,3]= 29968 uA
DAC[1,0]= 29968 uA
DAC[1,1]= 29968 uA
DAC[1,2]= 29968 uA
DAC[1,3]= 29968 uA
DAC[2,0]= 29968 uA
DAC[2,1]= 29968 uA
DAC[2,2]= 29968 uA
DAC[2,3]= 29968 uA
DAC[3,0]= 29968 uA
DAC[3,1]= 29968 uA
DAC[3,2]= 29968 uA
DAC[3,3]= 29968 uA
Set Attenuation in dB to :  0.0
###############################################
   ADC Tile0 ch0 DSA set to 0.0 dB.
###############################################
Set Attenuation in dB to :  0.0
###############################################
   ADC Tile0 ch1 DSA set to 0.0 dB.
###############################################
Set Attenuation in dB to :  0.0
###############################################
ADC Tile0 ch2 is not available.
###############################################
Set Attenuation in dB to :  0.0
###############################################
ADC Tile0 ch3 is not available.
###############################################
Set Attenuation in dB to :  0.0
###############################################
   ADC Tile1 ch0 DSA set to 0.0 dB.
###############################################
Set Attenuation in dB to :  0.0
###############################################
   ADC Tile1 ch1 DSA set to 0.0 dB.
###############################################
Set Attenuation in dB to :  0.0
###############################################
   ADC Tile1 ch2 DSA set to 0.0 dB.
###############################################
Set Attenuation in dB to :  0.0
###############################################
   ADC Tile1 ch3 DSA set to 0.0 dB.
###############################################
Set Attenuation in dB to :  0.0
###############################################
   ADC Tile2 ch0 DSA set to 0.0 dB.
###############################################
Set Attenuation in dB to :  0.0
###############################################
   ADC Tile2 ch1 DSA set to 0.0 dB.
###############################################
Set Attenuation in dB to :  0.0
###############################################
ADC Tile2 ch2 is not available.
###############################################
Set Attenuation in dB to :  0.0
###############################################
ADC Tile2 ch3 is not available.
###############################################
Set Attenuation in dB to :  0.0
###############################################
   ADC Tile3 ch0 DSA set to 0.0 dB.
###############################################
Set Attenuation in dB to :  0.0
###############################################
   ADC Tile3 ch1 DSA set to 0.0 dB.
###############################################
Set Attenuation in dB to :  0.0
###############################################
   ADC Tile3 ch2 DSA set to 0.0 dB.
###############################################
Set Attenuation in dB to :  0.0
###############################################
   ADC Tile3 ch3 DSA set to 0.0 dB.
###############################################

###############################################
=== ADC DSA Report ===
   ADC Tile0 ch0 DSA set to 0.0 dB.
   ADC Tile0 ch1 DSA set to 0.0 dB.
   ADC Tile1 ch0 DSA set to 0.0 dB.
   ADC Tile1 ch1 DSA set to 0.0 dB.
   ADC Tile1 ch2 DSA set to 0.0 dB.
   ADC Tile1 ch3 DSA set to 0.0 dB.
   ADC Tile2 ch0 DSA set to 0.0 dB.
   ADC Tile2 ch1 DSA set to 0.0 dB.
   ADC Tile3 ch0 DSA set to 0.0 dB.
   ADC Tile3 ch1 DSA set to 0.0 dB.
   ADC Tile3 ch2 DSA set to 0.0 dB.
   ADC Tile3 ch3 DSA set to 0.0 dB.
###############################################

###############################################
=== Data Current Report ===
   DAC Tile0 Ch0 output current mode is set to RFSoC Gen3.
   DAC Tile0 Ch0 output current is set to 29968 uA.
   DAC Tile0 Ch1 output current mode is set to RFSoC Gen3.
   DAC Tile0 Ch1 output current is set to 29968 uA.
   DAC Tile0 Ch2 output current mode is set to RFSoC Gen3.
   DAC Tile0 Ch2 output current is set to 29968 uA.
   DAC Tile0 Ch3 output current mode is set to RFSoC Gen3.
   DAC Tile0 Ch3 output current is set to 29968 uA.
   DAC Tile1 Ch0 output current mode is set to RFSoC Gen3.
   DAC Tile1 Ch0 output current is set to 29968 uA.
   DAC Tile1 Ch1 output current mode is set to RFSoC Gen3.
   DAC Tile1 Ch1 output current is set to 29968 uA.
   DAC Tile1 Ch2 output current mode is set to RFSoC Gen3.
   DAC Tile1 Ch2 output current is set to 29968 uA.
   DAC Tile1 Ch3 output current mode is set to RFSoC Gen3.
   DAC Tile1 Ch3 output current is set to 29968 uA.
###############################################

Manhtx implement new automation process

###############################################
Enabling DAC Multi-Tile Synchronization...

DAC_Sync_Config2.Tiles: 0xffffffff

=== Multi-Tile Synchronization Metal Log Report ===
metal: info:      DTC Scan PLL
metal: debug:     Target 64, DTC Code 2, Diff 62, Min 62
metal: debug:     Target 64, DTC Code 18, Diff 46, Min 46
metal: debug:     Target 64, DTC Code 37, Diff 27, Min 27
metal: debug:     Target 64, DTC Code 57, Diff 7, Min 7
metal: debug:     Target 64, DTC Code 77, Diff 13, Min 7
metal: debug:     Target 64, DTC Code 97, Diff 33, Min 7
metal: debug:     Target 64, DTC Code 117, Diff 53, Min 7
metal: debug:     RefTile (0): DTC Code Target 64, Picked 57
metal: info:      DAC0: 000000111222000000000000011132200000000000000111322000000*000000#113222000000000000011132220000000000000011322200000000000001113
metal: debug:     Tile (1): Max/Min 57/57, Range 0
metal: debug:     Tile (1): Code 4, New-Range: 53, Min-Range: 53
metal: debug:     Tile (1): Code 21, New-Range: 36, Min-Range: 36
metal: debug:     Tile (1): Code 41, New-Range: 16, Min-Range: 16
metal: debug:     Tile (1): Code 61, New-Range: 4, Min-Range: 4
metal: debug:     Tile (1): Code 80, New-Range: 23, Min-Range: 4
metal: debug:     Tile (1): Code 99, New-Range: 42, Min-Range: 4
metal: debug:     Tile (1): Code 119, New-Range: 62, Min-Range: 4
metal: debug:     Tile (1): Code 61, Range Prev 0, New 4
metal: info:      DAC1: 000000000111222000000000000001122220000000000000011322200#000*000000111222000000000000011322200000000000000113222000000000000001
metal: debug:     Tile (2): Max/Min 61/57, Range 4
metal: debug:     Tile (2): Code 4, New-Range: 57, Min-Range: 57
metal: debug:     Tile (2): Code 21, New-Range: 40, Min-Range: 40
metal: debug:     Tile (2): Code 41, New-Range: 20, Min-Range: 20
metal: debug:     Tile (2): Code 60, New-Range: 4, Min-Range: 4
metal: debug:     Tile (2): Code 80, New-Range: 23, Min-Range: 4
metal: debug:     Tile (2): Code 99, New-Range: 42, Min-Range: 4
metal: debug:     Tile (2): Code 119, New-Range: 62, Min-Range: 4
metal: debug:     Tile (2): Code 60, Range Prev 4, New 4
metal: info:      DAC2: 000000000113222000000000000011122220000000000000111322200#00*0000001133222000000000000011222200000000000001112222000000000000011
metal: debug:     Tile (3): Max/Min 61/57, Range 4
metal: debug:     Tile (3): Code 3, New-Range: 58, Min-Range: 58
metal: debug:     Tile (3): Code 20, New-Range: 41, Min-Range: 41
metal: debug:     Tile (3): Code 40, New-Range: 21, Min-Range: 21
metal: debug:     Tile (3): Code 60, New-Range: 4, Min-Range: 4
metal: debug:     Tile (3): Code 79, New-Range: 22, Min-Range: 4
metal: debug:     Tile (3): Code 98, New-Range: 41, Min-Range: 4
metal: debug:     Tile (3): Code 118, New-Range: 61, Min-Range: 4
metal: debug:     Tile (3): Code 60, Range Prev 4, New 4
metal: info:      DAC3: 000000001112222000000000000111322200000000000001113222000#00*0000001132220000000000000113222000000000000011122220000000000000113
metal: info:      DTC Scan T1
metal: debug:     Target 64, DTC Code 32, Diff 32, Min 32
metal: debug:     Target 64, DTC Code 99, Diff 35, Min 32
metal: debug:     RefTile (0): DTC Code Target 64, Picked 32
metal: info:      DAC0: 00000000000000000000000000000000*0000000000000000000000000000000#113222000000000000000000000000000000000000000000000000000000000
metal: debug:     Tile (1): Max/Min 32/32, Range 0
metal: debug:     Tile (1): Code 33, New-Range: 1, Min-Range: 1
metal: debug:     Tile (1): Code 100, New-Range: 68, Min-Range: 1
metal: debug:     Tile (1): Code 33, Range Prev 0, New 1
metal: info:      DAC1: 00000000000000000000000000000000#*0000000000000000000000000000000000111222000000000000000000000000000000000000000000000000000000
metal: debug:     Tile (2): Max/Min 33/32, Range 1
metal: debug:     Tile (2): Code 33, New-Range: 1, Min-Range: 1
metal: debug:     Tile (2): Code 101, New-Range: 69, Min-Range: 1
metal: debug:     Tile (2): Code 33, Range Prev 1, New 1
metal: info:      DAC2: 00000000000000000000000000000000#*0000000000000000000000000000000000113222200000000000000000000000000000000000000000000000000000
metal: debug:     Tile (3): Max/Min 33/32, Range 1
metal: debug:     Tile (3): Code 34, New-Range: 2, Min-Range: 2
metal: debug:     Tile (3): Code 101, New-Range: 69, Min-Range: 2
metal: debug:     Tile (3): Code 34, Range Prev 1, New 2
metal: info:      DAC3: 00000000000000000000000000000000#0*000000000000000000000000000000000011322200000000000000000000000000000000000000000000000000000
metal: debug:     Marker Read Tile 0, FIFO 0 - 00006000 = 0000: count=18, loc=0, done=1
metal: info:      DAC0: Marker: - 18, 0
metal: debug:     Marker Read Tile 0, FIFO 1 - 00006400 = 0000: count=18, loc=0, done=1
metal: debug:     Marker Read Tile 0, FIFO 2 - 00006800 = 0000: count=18, loc=0, done=1
metal: debug:     Marker Read Tile 0, FIFO 3 - 00006C00 = 0000: count=18, loc=0, done=1
metal: debug:     Marker Read Tile 1, FIFO 0 - 0000A000 = 0000: count=18, loc=0, done=1
metal: info:      DAC1: Marker: - 18, 0
metal: debug:     Marker Read Tile 1, FIFO 0 - 0000A000 = 0000: count=18, loc=0, done=1
metal: debug:     Marker Read Tile 1, FIFO 1 - 0000A400 = 0000: count=18, loc=0, done=1
metal: debug:     Marker Read Tile 1, FIFO 2 - 0000A800 = 0000: count=18, loc=0, done=1
metal: debug:     Marker Read Tile 2, FIFO 0 - 0000E000 = 0000: count=18, loc=0, done=1
metal: info:      DAC2: Marker: - 18, 0
metal: debug:     Marker Read Tile 2, FIFO 0 - 0000E000 = 0000: count=18, loc=0, done=1
metal: debug:     Marker Read Tile 2, FIFO 1 - 0000E400 = 0000: count=18, loc=0, done=1
metal: debug:     Marker Read Tile 3, FIFO 0 - 00012000 = 0000: count=18, loc=0, done=1
metal: info:      DAC3: Marker: - 18, 0
metal: debug:     Marker Read Tile 3, FIFO 0 - 00012000 = 0000: count=18, loc=0, done=1
metal: debug:     Marker Read Tile 3, FIFO 2 - 00012800 = 0000: count=18, loc=0, done=1
metal: debug:     Marker Read Tile 3, FIFO 3 - 00012C00 = 0000: count=18, loc=0, done=1
metal: debug:     Count_W 24, loc_W 12
metal: info:      SysRef period in terms of DAC T1s = 480
metal: info:      DAC target latency = 432
metal: debug:     Tile 0, latency 432, max 432
metal: debug:     Tile 1, latency 432, max 432
metal: debug:     Tile 2, latency 432, max 432
metal: debug:     Tile 3, latency 432, max 432
metal: debug:     Target 432, Tile 0, delta 0, i/f_part 0/0, offset 0
metal: debug:     Target 432, Tile 1, delta 0, i/f_part 0/0, offset 0
metal: debug:     Target 432, Tile 2, delta 0, i/f_part 0/0, offset 0
metal: debug:     Target 432, Tile 3, delta 0, i/f_part 0/0, offset 0

========== DAC Multi-Tile Sync Report ==========
DAC0: Latency(T1) = 432, Adjusted Delay Offset(T12) =   0, Marker Delay = 15
=== MTS DAC Tile0 PLL Report ===
    DAC0: PLL DTC Code =57
    DAC0: PLL Num Windows =7
    DAC0: PLL Max Gap =14
    DAC0: PLL Min Gap =6
    DAC0: PLL Max Overlap =1
=== MTS DAC Tile0 T1 Report ===
    DAC0: T1 DTC Code =32
    DAC0: T1 Num Windows =2
    DAC0: T1 Max Gap =65
    DAC0: T1 Min Gap =57
    DAC0: T1 Max Overlap =1

DAC1: Latency(T1) = 432, Adjusted Delay Offset(T12) =   0, Marker Delay = 15
=== MTS DAC Tile1 PLL Report ===
    DAC1: PLL DTC Code =61
    DAC1: PLL Num Windows =7
    DAC1: PLL Max Gap =14
    DAC1: PLL Min Gap =9
    DAC1: PLL Max Overlap =1
=== MTS DAC Tile1 T1 Report ===
    DAC1: T1 DTC Code =33
    DAC1: T1 Num Windows =2
    DAC1: T1 Max Gap =68
    DAC1: T1 Min Gap =54
    DAC1: T1 Max Overlap =0

DAC2: Latency(T1) = 432, Adjusted Delay Offset(T12) =   0, Marker Delay = 15
=== MTS DAC Tile2 PLL Report ===
    DAC2: PLL DTC Code =60
    DAC2: PLL Num Windows =7
    DAC2: PLL Max Gap =13
    DAC2: PLL Min Gap =9
    DAC2: PLL Max Overlap =2
=== MTS DAC Tile2 T1 Report ===
    DAC2: T1 DTC Code =33
    DAC2: T1 Num Windows =2
    DAC2: T1 Max Gap =68
    DAC2: T1 Min Gap =53
    DAC2: T1 Max Overlap =1

DAC3: Latency(T1) = 432, Adjusted Delay Offset(T12) =   0, Marker Delay = 15
=== MTS DAC Tile3 PLL Report ===
    DAC3: PLL DTC Code =60
    DAC3: PLL Num Windows =7
    DAC3: PLL Max Gap =13
    DAC3: PLL Min Gap =8
    DAC3: PLL Max Overlap =1
=== MTS DAC Tile3 T1 Report ===
    DAC3: T1 DTC Code =34
    DAC3: T1 Num Windows =2
    DAC3: T1 Max Gap =69
    DAC3: T1 Min Gap =53
    DAC3: T1 Max Overlap =1

DAC Multi-Tile Synchronization is complete.
###############################################

###############################################
Enabling ADC Multi-Tile Synchronization...

ADC_Sync_Config2.Tiles: 0xffffffff

=== Multi-Tile Synchronization Metal Log Report ===
metal: info:      DTC Scan PLL
metal: debug:     Target 64, DTC Code 11, Diff 53, Min 53
metal: debug:     Target 64, DTC Code 30, Diff 34, Min 34
metal: debug:     Target 64, DTC Code 49, Diff 15, Min 15
metal: debug:     Target 64, DTC Code 68, Diff 4, Min 4
metal: debug:     Target 64, DTC Code 87, Diff 23, Min 4
metal: debug:     Target 64, DTC Code 106, Diff 42, Min 4
metal: debug:     Target 64, DTC Code 123, Diff 59, Min 4
metal: debug:     RefTile (0): DTC Code Target 64, Picked 68
metal: info:      ADC0: 1122200000000000001132220000000000000113222000000000000001132220#000*00000011122200000000000001132220000000000000111322000000000
metal: debug:     Tile (1): Max/Min 68/68, Range 0
metal: debug:     Tile (1): Code 13, New-Range: 55, Min-Range: 55
metal: debug:     Tile (1): Code 32, New-Range: 36, Min-Range: 36
metal: debug:     Tile (1): Code 52, New-Range: 16, Min-Range: 16
metal: debug:     Tile (1): Code 71, New-Range: 3, Min-Range: 3
metal: debug:     Tile (1): Code 90, New-Range: 22, Min-Range: 3
metal: debug:     Tile (1): Code 109, New-Range: 41, Min-Range: 3
metal: debug:     Tile (1): Code 125, New-Range: 57, Min-Range: 3
metal: debug:     Tile (1): Code 71, Range Prev 0, New 3
metal: info:      ADC1: 01113220000000000000111222000000000000001112222000000000000111322000#00*00000001122200000000000001112220000000000000111322200000
metal: debug:     Tile (2): Max/Min 71/68, Range 3
metal: debug:     Tile (2): Code 14, New-Range: 57, Min-Range: 57
metal: debug:     Tile (2): Code 33, New-Range: 38, Min-Range: 38
metal: debug:     Tile (2): Code 53, New-Range: 18, Min-Range: 18
metal: debug:     Tile (2): Code 72, New-Range: 4, Min-Range: 4
metal: debug:     Tile (2): Code 91, New-Range: 23, Min-Range: 4
metal: debug:     Tile (2): Code 110, New-Range: 42, Min-Range: 4
metal: debug:     Tile (2): Code 72, Range Prev 3, New 4
metal: info:      ADC2: 00011322000000000000011322200000000000000113222000000000000011322200#000*0000001132220000000000000011222000000000000001112220000
metal: debug:     Tile (3): Max/Min 72/68, Range 4
metal: debug:     Tile (3): Code 14, New-Range: 58, Min-Range: 58
metal: debug:     Tile (3): Code 33, New-Range: 39, Min-Range: 39
metal: debug:     Tile (3): Code 52, New-Range: 20, Min-Range: 20
metal: debug:     Tile (3): Code 71, New-Range: 4, Min-Range: 4
metal: debug:     Tile (3): Code 90, New-Range: 22, Min-Range: 4
metal: debug:     Tile (3): Code 110, New-Range: 42, Min-Range: 4
metal: debug:     Tile (3): Code 125, New-Range: 57, Min-Range: 4
metal: debug:     Tile (3): Code 71, Range Prev 4, New 4
metal: info:      ADC3: 00111222000000000000011322200000000000000112220000000000000011322000#00*00000001122200000000000001113222000000000000011132200000
metal: info:      DTC Scan T1
metal: debug:     Target 64, DTC Code 40, Diff 24, Min 24
metal: debug:     Target 64, DTC Code 104, Diff 40, Min 24
metal: debug:     RefTile (0): DTC Code Target 64, Picked 40
metal: info:      ADC0: 1132220000000000000000000000000000000000*00000000000000000000000#000000000011322200000000000000000000000000000000000000000000000
metal: debug:     Tile (1): Max/Min 40/40, Range 0
metal: debug:     Tile (1): Code 43, New-Range: 3, Min-Range: 3
metal: debug:     Tile (1): Code 105, New-Range: 65, Min-Range: 3
metal: debug:     Tile (1): Code 43, Range Prev 0, New 3
metal: info:      ADC1: 0001122220000000000000000000000000000000#00*000000000000000000000000000000000001122200000000000000000000000000000000000000000000
metal: debug:     Tile (2): Max/Min 43/40, Range 3
metal: debug:     Tile (2): Code 43, New-Range: 3, Min-Range: 3
metal: debug:     Tile (2): Code 105, New-Range: 65, Min-Range: 3
metal: debug:     Tile (2): Code 43, Range Prev 3, New 3
metal: info:      ADC2: 0001132220000000000000000000000000000000#00*000000000000000000000000000000000011122200000000000000000000000000000000000000000000
metal: debug:     Tile (3): Max/Min 43/40, Range 3
metal: debug:     Tile (3): Code 43, New-Range: 3, Min-Range: 3
metal: debug:     Tile (3): Code 106, New-Range: 66, Min-Range: 3
metal: debug:     Tile (3): Code 43, Range Prev 3, New 3
metal: info:      ADC3: 0001122220000000000000000000000000000000#00*000000000000000000000000000000000001122220000000000000000000000000000000000000000000
metal: debug:     Marker Read Tile 0, FIFO 0 - 00014000 = 100009: count=9, loc=0, done=1
metal: info:      ADC0: Marker: - 9, 0
metal: debug:     Marker Read Tile 0, FIFO 0 - 00014000 = 100009: count=9, loc=0, done=1
metal: debug:     Marker Read Tile 0, FIFO 2 - 00014000 = 100009: count=9, loc=0, done=1
metal: debug:     Marker Read Tile 1, FIFO 0 - 00018000 = 100009: count=9, loc=0, done=1
metal: info:      ADC1: Marker: - 9, 0
metal: debug:     Marker Read Tile 1, FIFO 0 - 00018000 = 100009: count=9, loc=0, done=1
metal: debug:     Marker Read Tile 1, FIFO 1 - 00018000 = 100009: count=9, loc=0, done=1
metal: debug:     Marker Read Tile 1, FIFO 2 - 00018000 = 100009: count=9, loc=0, done=1
metal: debug:     Marker Read Tile 1, FIFO 3 - 00018000 = 100009: count=9, loc=0, done=1
metal: debug:     Marker Read Tile 2, FIFO 0 - 0001C000 = 100009: count=9, loc=0, done=1
metal: info:      ADC2: Marker: - 9, 0
metal: debug:     Marker Read Tile 2, FIFO 1 - 0001C000 = 100009: count=9, loc=0, done=1
metal: debug:     Marker Read Tile 3, FIFO 0 - 00020000 = 100009: count=9, loc=0, done=1
metal: info:      ADC3: Marker: - 9, 0
metal: debug:     Count_W 24, loc_W 12
metal: info:      SysRef period in terms of ADC T1s = 960
metal: info:      ADC target latency = 216
metal: debug:     Tile 0, latency 216, max 216
metal: debug:     Tile 1, latency 216, max 216
metal: debug:     Tile 2, latency 216, max 216
metal: debug:     Tile 3, latency 216, max 216
metal: debug:     Target 216, Tile 0, delta 0, i/f_part 0/0, offset 0
metal: debug:     Target 216, Tile 1, delta 0, i/f_part 0/0, offset 0
metal: debug:     Target 216, Tile 2, delta 0, i/f_part 0/0, offset 0
metal: debug:     Target 216, Tile 3, delta 0, i/f_part 0/0, offset 0

========== ADC Multi-Tile Sync Report ==========
ADC0: Latency(T1) = 216, Adjusted Delay Offset(T12) =   0, Marker Delay = 15
=== MTS ADC Tile0 PLL Report ===
    ADC0: PLL DTC Code =68
    ADC0: PLL Num Windows =7
    ADC0: PLL Max Gap =14
    ADC0: PLL Min Gap =9
    ADC0: PLL Max Overlap =1
=== MTS ADC Tile0 T1 Report ===
    ADC0: T1 DTC Code =40
    ADC0: T1 Num Windows =2
    ADC0: T1 Max Gap =69
    ADC0: T1 Min Gap =47
    ADC0: T1 Max Overlap =1

ADC1: Latency(T1) = 216, Adjusted Delay Offset(T12) =   0, Marker Delay = 15
=== MTS ADC Tile1 PLL Report ===
    ADC1: PLL DTC Code =71
    ADC1: PLL Num Windows =7
    ADC1: PLL Max Gap =14
    ADC1: PLL Min Gap =5
    ADC1: PLL Max Overlap =1
=== MTS ADC Tile1 T1 Report ===
    ADC1: T1 DTC Code =43
    ADC1: T1 Num Windows =2
    ADC1: T1 Max Gap =70
    ADC1: T1 Min Gap =44
    ADC1: T1 Max Overlap =0

ADC2: Latency(T1) = 216, Adjusted Delay Offset(T12) =   0, Marker Delay = 15
=== MTS ADC Tile2 PLL Report ===
    ADC2: PLL DTC Code =72
    ADC2: PLL Num Windows =6
    ADC2: PLL Max Gap =14
    ADC2: PLL Min Gap =13
    ADC2: PLL Max Overlap =1
=== MTS ADC Tile2 T1 Report ===
    ADC2: T1 DTC Code =43
    ADC2: T1 Num Windows =2
    ADC2: T1 Max Gap =69
    ADC2: T1 Min Gap =44
    ADC2: T1 Max Overlap =1

ADC3: Latency(T1) = 216, Adjusted Delay Offset(T12) =   0, Marker Delay = 15
=== MTS ADC Tile3 PLL Report ===
    ADC3: PLL DTC Code =71
    ADC3: PLL Num Windows =7
    ADC3: PLL Max Gap =14
    ADC3: PLL Min Gap =5
    ADC3: PLL Max Overlap =1
=== MTS ADC Tile3 T1 Report ===
    ADC3: T1 DTC Code =43
    ADC3: T1 Num Windows =2
    ADC3: T1 Max Gap =70
    ADC3: T1 Min Gap =43
    ADC3: T1 Max Overlap =0

ADC Multi-Tile Synchronization is complete.
###############################################
May chu UDP da khoi dong @ port 105
dang cho su kien tren gpiochip3 line 0 (GIC 124)...
pthread_create(&can_thread_id, NULL, can_receiver_thread, NULL) => done
[CAN Thread] Bat đau lang nghe...
pthread_create(&FFT_tid, NULL, XGpio_Interrupt_IRQ, NULL) => done
pthread_create(&thread_adc, NULL, ADC_Worker_Thread, NULL) => done
[ADC] Thread started for 4MB packets.
pthread_create(&thread_fft, NULL, FFT_Monitor_SG_S2MM_Thread, NULL) => done
[FFT] Initializing virtual pointers...
------------------------------------------------------------
[FFT] Virtual Memory Map Diagnostics:
  - DMA Registers (IDX_DMA_FFT): Phys=0xA01C0000, Virt=0xffff93ce9000
  - Descriptors   (IDX_DMA_FFT_DESC): Phys=0x50000000, Virt=0xffff94250000
  - Data Buffer   (IDX_FFT_BUF): Phys=0x50001000, Virt=0xffff63b49000
------------------------------------------------------------
[FFT] Config: Packet Size=1048576, Buffer Size=1048832

[FFT DEBUG] Descriptor 0 (Virt: 0xffff94250000, Phys: 0x50000000) Cấu hình:
  - Buffer Phys: 0x50001000 (MSB: 0x0)
  - Next Desc Phys: 0x50000040 (MSB: 0x0) -> Expected 0x50000040
  - Control (Size): 1048832
  - Status: 0x0

[FFT DEBUG] Descriptor 1 (Virt: 0xffff94250040, Phys: 0x50000040) Cấu hình:
  - Buffer Phys: 0x50101100 (MSB: 0x0) -> Expected 0x50101100
  - Next Desc Phys: 0x50000000 (MSB: 0x0) -> Expected 0x50000000 (Desc 0)
  - Control (Size): 1048832
  - Status: 0x0
[FFT] Resetting DMA channel...
[FFT] DMA Reset complete.
[FFT] Setting initial descriptors...
[FFT] Starting DMA Engine...

[FFT DEBUG READBACK] Sau khi Ghi Lệnh Cấu hình:
  - DMACR: 0x13003 (Readback)
  - DMASR: 0x00010008
  - CURDESC: 0x50000000 (Exp: 0x50000000)
  - TAILDESC: 0x50000040 (Exp: 0x50000040)
  - Status Idle/Halted: Idle=0, Halted=0, ErrIrq=0
[FFT] Ping-Pong SG Mode Started. Polling loop active...
[FFT DIAG] Hardware Status Changed: 0x00010008
[FFT DIAG] Pointers -> hw_status: 0x00010008 CURDESC: 0x50000000 | TAILDESC: 0x50000040
   -> BYTES REMAINING (0x58): 0
[CAN] Error writing to socket: Network is down
[CAN] Error writing to socket: Network is down
[CMAC] Performing RX Datapath Toggle Reset...

--- QSFP28 MODULE DIAGNOSTICS (I2C-1 @ 0x50) ---
[STATUS] SFF-8636 Byte 2 (Interrupt Flags) = 0x02
 [FAIL] Module Not Ready (Bit 7 = 0). Cannot Trust Data.
[STATUS] SFF-8636 Byte 3 (Warning Flags)   = 0x00
[STATUS] SFF-8636 Byte 12 (LOS/TxFault) = 0x00

--- CMAC LINK STATUS DEDUCTION ---
 [ OK ] RX LOS (Lane 0-3): All LANS have signal.
 [ OK ] TX Fault (Lane 0-3): No TX Fault detected.

=> KET LUAN: MODULE BÁO LỖI TÍN HIỆU QUANG. LỖI NGOÀI.
--------------------------------------------------
[CMAC Status] : stat_rx_aligned=0, stat_tx_local_fault=0, interal_rx_local_fault=1, stat_rx_local_fault=1, stat_rx_miss_aligned=0, stat_tx_bad_fcs=0
[CMAC Status] : SFP_intl=1, SFP_MODPRSL=0
[CMAC Status Reg] : 0x0204 = 0x00000003 (LATCHED VAL)
[CMAC] Performing RX Datapath Toggle Reset...
[CMAC] Performing RX Datapath Toggle Reset...
[FFT DIAG] Hardware Status Changed: 0x00010008
[FFT DIAG] Pointers -> hw_status: 0x00010008 CURDESC: 0x50000000 | TAILDESC: 0x50000040
   -> BYTES REMAINING (0x58): 0
[CMAC] Performing RX Datapath Toggle Reset...
[CMAC] Performing RX Datapath Toggle Reset...
[FFT DIAG] Hardware Status Changed: 0x00010008
[FFT DIAG] Pointers -> hw_status: 0x00010008 CURDESC: 0x50000000 | TAILDESC: 0x50000040
   -> BYTES REMAINING (0x58): 0
[CMAC] Performing RX Datapath Toggle Reset...

--- QSFP28 MODULE DIAGNOSTICS (I2C-1 @ 0x50) ---
[STATUS] SFF-8636 Byte 2 (Interrupt Flags) = 0x02
 [FAIL] Module Not Ready (Bit 7 = 0). Cannot Trust Data.
[STATUS] SFF-8636 Byte 3 (Warning Flags)   = 0x00
[STATUS] SFF-8636 Byte 12 (LOS/TxFault) = 0x00

--- CMAC LINK STATUS DEDUCTION ---
 [ OK ] RX LOS (Lane 0-3): All LANS have signal.
 [ OK ] TX Fault (Lane 0-3): No TX Fault detected.

=> KET LUAN: MODULE BÁO LỖI TÍN HIỆU QUANG. LỖI NGOÀI.
--------------------------------------------------
[CMAC Status] : stat_rx_aligned=0, stat_tx_local_fault=0, interal_rx_local_fault=1, stat_rx_local_fault=1, stat_rx_miss_aligned=0, stat_tx_bad_fcs=0
[CMAC Status] : SFP_intl=1, SFP_MODPRSL=0
[CMAC Status Reg] : 0x0204 = 0x00000003 (LATCHED VAL)
[CMAC] Performing RX Datapath Toggle Reset...
[FFT DIAG] Hardware Status Changed: 0x00010008
[FFT DIAG] Pointers -> hw_status: 0x00010008 CURDESC: 0x50000000 | TAILDESC: 0x50000040
   -> BYTES REMAINING (0x58): 0
[CMAC] Performing RX Datapath Toggle Reset...
[CMAC] Performing RX Datapath Toggle Reset...
[ADC] Timeout! SR: 0x0, Recv: 4194304/4194304
[ADC] Error! Bytes received before TLAST: 4194304
[ADC] SUCCESS! Data[0..3]: 0x0444FEAD 0x00070009 0x0003FFFB 0xFFF80002
[CMAC] Performing RX Datapath Toggle Reset...
[FFT DIAG] Hardware Status Changed: 0x00010008
[FFT DIAG] Pointers -> hw_status: 0x00010008 CURDESC: 0x50000000 | TAILDESC: 0x50000040
   -> BYTES REMAINING (0x58): 0
[CMAC] Performing RX Datapath Toggle Reset...

--- QSFP28 MODULE DIAGNOSTICS (I2C-1 @ 0x50) ---
[STATUS] SFF-8636 Byte 2 (Interrupt Flags) = 0x02
 [FAIL] Module Not Ready (Bit 7 = 0). Cannot Trust Data.
[STATUS] SFF-8636 Byte 3 (Warning Flags)   = 0x00
[STATUS] SFF-8636 Byte 12 (LOS/TxFault) = 0x00

--- CMAC LINK STATUS DEDUCTION ---
 [ OK ] RX LOS (Lane 0-3): All LANS have signal.
 [ OK ] TX Fault (Lane 0-3): No TX Fault detected.

=> KET LUAN: MODULE BÁO LỖI TÍN HIỆU QUANG. LỖI NGOÀI.
--------------------------------------------------
[CMAC Status] : stat_rx_aligned=0, stat_tx_local_fault=0, interal_rx_local_fault=1, stat_rx_local_fault=1, stat_rx_miss_aligned=0, stat_tx_bad_fcs=0
[CMAC Status] : SFP_intl=1, SFP_MODPRSL=0
[CMAC Status Reg] : 0x0204 = 0x00000003 (LATCHED VAL)
[CMAC] Performing RX Datapath Toggle Reset...
[CMAC] Performing RX Datapath Toggle Reset...
[FFT DIAG] Hardware Status Changed: 0x00010008
[FFT DIAG] Pointers -> hw_status: 0x00010008 CURDESC: 0x50000000 | TAILDESC: 0x50000040
   -> BYTES REMAINING (0x58): 0
[CMAC] Performing RX Datapath Toggle Reset...
[CMAC] Performing RX Datapath Toggle Reset...
[FFT DIAG] Hardware Status Changed: 0x00010008
[FFT DIAG] Pointers -> hw_status: 0x00010008 CURDESC: 0x50000000 | TAILDESC: 0x50000040
   -> BYTES REMAINING (0x58): 0
[CMAC] Performing RX Datapath Toggle Reset...

--- QSFP28 MODULE DIAGNOSTICS (I2C-1 @ 0x50) ---
[STATUS] SFF-8636 Byte 2 (Interrupt Flags) = 0x02
 [FAIL] Module Not Ready (Bit 7 = 0). Cannot Trust Data.
[STATUS] SFF-8636 Byte 3 (Warning Flags)   = 0x00
[STATUS] SFF-8636 Byte 12 (LOS/TxFault) = 0x00

--- CMAC LINK STATUS DEDUCTION ---
 [ OK ] RX LOS (Lane 0-3): All LANS have signal.
 [ OK ] TX Fault (Lane 0-3): No TX Fault detected.

=> KET LUAN: MODULE BÁO LỖI TÍN HIỆU QUANG. LỖI NGOÀI.
--------------------------------------------------
[CMAC Status] : stat_rx_aligned=0, stat_tx_local_fault=0, interal_rx_local_fault=1, stat_rx_local_fault=1, stat_rx_miss_aligned=0, stat_tx_bad_fcs=0
[CMAC Status] : SFP_intl=1, SFP_MODPRSL=0
[CMAC Status Reg] : 0x0204 = 0x00000003 (LATCHED VAL)
[ADC] SUCCESS! Data[0..3]: 0x047DFFC4 0x00030001 0xFFF8FFF8 0x0001FFFC
[CMAC] Performing RX Datapath Toggle Reset...