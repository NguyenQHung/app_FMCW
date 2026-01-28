Hướng Dẫn Cấu Hình và Biên Dịch Dự Án C Cross-Compile cho ARM trên WSL & VS Code
lênh build: 
make
make clean

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
Cau hinh mang can duoc thiet lap tinh trong /etc/network/interfaces
  IP Bo mach: 192.168.1.5
  IP PC: 192.168.1.10
Radar Data innit ok
Innit_Radar_Debug ok
Error opening /dev/ttyUL1: No such file or directory
Uart0 error
Error opening /dev/ttyUL2: No such file or directory
Uart1 error
Error opening /dev/ttyUL3: No such file or directory
Uart2 error
Error opening /dev/ttyUL4: No such file or directory
Uart3 error
Error opening /dev/ttyUL5: No such file or directory
Uart4 error
Pl_Spi0 (a004) ok
Pl_Spi1 (a005) ok
Pl_Spi2 (a006) ok
Pl_Spi3 (a015) ok
Pl_Spi4 (a014) ok
SPI Configured: /dev/spidev4.0, Mode: 0, Speed: 1000000 Hz
SPI Configured: /dev/spidev5.0, Mode: 0, Speed: 1000000 Hz
[HAL] XGpioPs_CfgInitialize (sysfs) - Nothing to do here.
XGpioPs_Setup devices
psgpio ok
GPIO_ENDFRAME_IRQ ok
GPIO_ENDFRAME_IRQ1 ok
GPIO_HS1 ok
GPIO_HS2 ok
GPIO_HS3 ok
[CAN] Initializing all CAN buses...
[CAN] Interface can0 initialized successfully.
[CAN] Interface can1 initialized successfully.
ADF4159_Innit done
ADF4360_Innit done

###############################################
Hello RFSoC World!

Segmentation fault