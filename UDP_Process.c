#include "UDP_Process.h"
#include "User_RAM.h"

/* defined by each RAW mode application */
void print_app_header();
int start_application();
int transfer_data();
void tcp_fasttmr(void);
void tcp_slowtmr(void);

/* missing declaration in lwIP */
void lwip_init();

#if LWIP_IPV6==0
#if LWIP_DHCP==1
extern volatile int dhcp_timoutcntr;
err_t dhcp_start(struct netif *netif);
#endif
#endif

// Bạn cần một phiên bản blocking của xemacif_input
void xemacif_input_blocking(UdpConnection *conn) {
    unsigned char rx_buffer[1400];
    ssize_t n = recvfrom(conn->sockfd, rx_buffer, sizeof(rx_buffer), 0,
                         (struct sockaddr *)&conn->client_addr, &conn->client_len);
    if (n > 0) 
	{
		// ---- Logic xử lý gói tin của bạn (giữ nguyên) ----
		if (n == 320) {
			memcpy(&Radar.PC8_Config, rx_buffer, 320);
			if ((Radar.PC8_Config[0] == 0xab) && (Radar.PC8_Config[1] == 0x56)) {
				Radar.Receiver_Status4 = 1;
			}
			conn->is_client_addr_valid = 1;
		} else if (n == 828) {
			memcpy(&Radar.PC8_Excel, rx_buffer, 828);
			Radar.Receiver_Excel = 1;
			conn->is_client_addr_valid = 1;
		}
		// Thêm các else if khác nếu cần
	}
}

void* udp_control_thread(void *arg) {
	int cmd_update = 0;
    printf("[UDP Thread] Ready to receive commands.\n");
    while (1) {
        // xemacif_input bây giờ nên có timeout dài hơn hoặc block
        // để không tốn CPU.
        xemacif_input_blocking(&udp_conn); // Một phiên bản blocking của xemacif_input
        
        if(Radar.Receiver_Status4==1) // goi tin tu PC8
		{
			Radar.Receiver_Status4=0;
			Radar_Read_PC8_CMD();
			cmd_update++;
			if(cmd_update >= 5)
			{
				cmd_update = 0;
			printf("u\r\n");
			}
		}
		else if(Radar.Receiver_Excel==1) // goi tin tu PC8
		{
			Radar.Receiver_Excel=0;
			Radar_Read_PC8_Excel();
		}
    }
    return NULL;
}

void _UDP_Innit_Buffer(void)
{
	Radar.UDP_PC8_Start 	= 0;
	Radar.UDP_Read_counter	= 0;
	Radar.UDP_Read_Start	= 0;

	PC8.Frame_counters 		= 0;
	PC8.Read_ENC_start 		= 0;
	PC8.Read_ENC_Time 		= 0;
	PC8.Read_Bamsat_start 	= 0;
	PC8.Read_Bamsat_time 	= 0;
	PC8.Phase_Step 			= 0;
	PC8.Phase_Step0 		= 0;

}

void _UDP_Read_Position(void)
{
	PC8.Frame_counters++;
	if(PC8.Frame_counters >= 2)
	{
		Erob_Read_Posision();
		PC8.Frame_counters = 0;
		PC8.Read_ENC_start = 1;
	}
}

void _UDP_Process_Bam_Sat_PC1(void)
{

	PC8.Read_Bamsat_time++;	// giam toc do xu ly

	if((PC8.Excel.Loading_done == 15)&&
	   (PC8.Read_Bamsat_time >= PC8.BamSat.Giam_mau_xuly))
	{
		PC8.Read_Bamsat_time = 0;

		PC8_Get_BeamMax_data();
		//PC8_Get_BeamMax_channel();
		//_Tichluy_Epxilon_Phi();

//		if(PC8._KS_Pvi.Loading == 1)
//		{
//			_Khao_Sat_Phuong_Vi();
//		}

		//_BS_Check_Target_in_Window();

	}
}

void _UDP_Process_Bam_Sat_PC8(void)
{
	PC8_Get_BeamMax_data();
	//PC8_Get_BeamMax_channel();
	//_Tichluy_Epxilon_Phi();
	//_Get_SS1_SS2_SS3_SS4_Max_Val();

	//_BS_Check_Target_in_Window();
}

void _UDP_Process_Phase(void)
{
	if(PC8.Phase_Step0 == 0)
	{
		System_Set_Phase_Start_Read(1);
		usleep(1);

		PC8_Read_Phase_Data();
		PC8_Get_ADC_Val();

		PC8_Get_Phase1(0);
		UDP_Phase1.AXI_GPIO_IN = 0x11111100 + PC8.Phase_Step;
		//udpsenderr = udp_sendto(&send_pcb2, psnd5, &RemoteAddr1, RemotePort5);

		pthread_mutex_lock(&radar_data_mutex); // --- KHÓA ---
		bytes_sent = sendto(udp_conn.sockfd,          // Socket để gửi đi
                    &UDP_Phase1,              // Con trỏ trực tiếp đến dữ liệu (thay cho pbuf)
                    sizeof(UDP_Phase1),       // Kích thước của dữ liệu
                    0,                        // Cờ (flags), thường là 0
                    (const struct sockaddr *)&udp_conn.remote_pc2_addr, // Địa chỉ đích
                    sizeof(udp_conn.remote_pc2_addr));                  // Kích thước địa chỉ đích

		PC8.Phase_Step0 = 1;
		pthread_mutex_unlock(&radar_data_mutex); // --- MỞ KHÓA ---
	}
	else if(PC8.Phase_Step0 == 1)
	{
		PC8_Get_Phase1(1);
		UDP_Phase1.AXI_GPIO_IN = 0x22222200 + PC8.Phase_Step;
		//udpsenderr = udp_sendto(&send_pcb2, psnd5, &RemoteAddr1, RemotePort5);

		pthread_mutex_lock(&radar_data_mutex); // --- KHÓA ---
		bytes_sent = sendto(udp_conn.sockfd,          // Socket để gửi đi
                    &UDP_Phase1,              // Con trỏ trực tiếp đến dữ liệu (thay cho pbuf)
                    sizeof(UDP_Phase1),       // Kích thước của dữ liệu
                    0,                        // Cờ (flags), thường là 0
                    (const struct sockaddr *)&udp_conn.remote_pc2_addr, // Địa chỉ đích
                    sizeof(udp_conn.remote_pc2_addr));                  // Kích thước địa chỉ đích

		PC8.Phase_Step0 = 2;
		pthread_mutex_unlock(&radar_data_mutex); // --- MỞ KHÓA ---
	}
	else if(PC8.Phase_Step0 == 2)
	{

		PC8_Get_Phase1(2);
		UDP_Phase1.AXI_GPIO_IN = 0x33333300 + PC8.Phase_Step;
		//udpsenderr = udp_sendto(&send_pcb2, psnd5, &RemoteAddr1, RemotePort5);

		pthread_mutex_lock(&radar_data_mutex); // --- KHÓA ---

		bytes_sent = sendto(udp_conn.sockfd,          // Socket để gửi đi
                    &UDP_Phase1,              // Con trỏ trực tiếp đến dữ liệu (thay cho pbuf)
                    sizeof(UDP_Phase1),       // Kích thước của dữ liệu
                    0,                        // Cờ (flags), thường là 0
                    (const struct sockaddr *)&udp_conn.remote_pc2_addr, // Địa chỉ đích
                    sizeof(udp_conn.remote_pc2_addr));                  // Kích thước địa chỉ đích

		PC8.Phase_Step0 = 3;
		pthread_mutex_unlock(&radar_data_mutex); // --- MỞ KHÓA ---
	}
	else if(PC8.Phase_Step0 == 3)
	{
		PC8_Get_Phase1(3);
		UDP_Phase1.AXI_GPIO_IN = 0x44444400 + PC8.Phase_Step;
		//udpsenderr = udp_sendto(&send_pcb2, psnd5, &RemoteAddr1, RemotePort5);

		pthread_mutex_lock(&radar_data_mutex); // --- KHÓA ---

		bytes_sent = sendto(udp_conn.sockfd,          // Socket để gửi đi
                    &UDP_Phase1,              // Con trỏ trực tiếp đến dữ liệu (thay cho pbuf)
                    sizeof(UDP_Phase1),       // Kích thước của dữ liệu
                    0,                        // Cờ (flags), thường là 0
                    (const struct sockaddr *)&udp_conn.remote_pc2_addr, // Địa chỉ đích
                    sizeof(udp_conn.remote_pc2_addr));                  // Kích thước địa chỉ đích
					
		PC8.Phase_Step++;
		if(PC8.Phase_Step > 250) PC8.Phase_Step = 0;


		//xil_printf("%d\r\n",PC8.Phase_Step);

		PC8.Phase_Step0 = 0;
		pthread_mutex_unlock(&radar_data_mutex); // --- MỞ KHÓA ---

		System_Set_Phase_Start_Read(0);
	}
}

void _UDP_Update_Click_Point(void)
{
	if(PC8.BS_PC1_Click == 0)
	{
		if((PC8.BamSat.TL_Phi_final <= 8.0)&&(PC8.BamSat.TL_Phi_final >= -8.0))
		{
			PC8.BS_Center_anglef = PC8.BS_Center_anglef0 + PC8.BamSat.TL_Phi_final;

			if(PC8.BS_Center_anglef >= 360.0) PC8.BS_Center_anglef = PC8.BS_Center_anglef - 360.0;
			if(PC8.BS_Center_anglef < 0.0) PC8.BS_Center_anglef = PC8.BS_Center_anglef + 360.0;
		}

		if((PC8.BamSat.TL_Epxilon_final <= 8.0)&&(PC8.BamSat.TL_Epxilon_final >= -8.0))
		{
			PC8.BS_Center_alphaf = PC8.BS_Center_alphaf0 + PC8.BamSat.TL_Epxilon_final;

			if(PC8.BS_Center_alphaf >= 360.0) PC8.BS_Center_alphaf = PC8.BS_Center_alphaf - 360.0;
			if(PC8.BS_Center_alphaf < 0.0) PC8.BS_Center_alphaf = PC8.BS_Center_alphaf + 360.0;
		}
	}
}

void _UDP_PC1_Send(void)
{
	if (Radar.Read_ok == 1)
	{
		Radar.Read_ok = 0;

		Radar.UDP_Step2++; // CHIA TAN PC8
		Radar.UDP_Step3++; // CHIA TAN PC1

		//PC8.code_data = Radar_Hyper_Input(_Input_trigger_sel);

		if(PC8.Phase_Mode == 0)
		{
			Radar.Beta_Re_cnt++;
			if(Radar.Beta_Re_cnt >= 2)
			{
				Radar.Beta_Re_en = 1;
				Radar.Beta_Re_cnt = 0;
			}

			_UDP_Read_Position();

			Radar_Read_Frame();

			if(Radar.Screen_Mode == 1)
			{
				
				if(Radar.Beta_HWi != Radar.Beta_HW)
				{
					pthread_mutex_lock(&radar_data_mutex); // --- KHÓA ---
					Radar.Beta_HWi 		= Radar.Beta_HW;
					Radar.IP_Data_ready = 0;
					//psnd->payload 		= &Radar_Frame1;
					//udpsenderr 			= udp_sendto(&send_pcb, psnd, &RemoteAddr1, RemotePort1); // 192.168.1.10:1024

					// 2. Gọi hàm sendto()
					bytes_sent = sendto(udp_conn.sockfd,           // Socket để gửi đi
										&Video_Frame,              // Con trỏ trực tiếp đến dữ liệu (thay cho pbuf)
										sizeof(Video_Frame),       // Kích thước của dữ liệu
										0,                         // Cờ (flags), thường là 0
										(const struct sockaddr *)&udp_conn.remote_pc1_addr, // Địa chỉ đích
										sizeof(udp_conn.remote_pc1_addr));                  // Kích thước địa chỉ đích

					pthread_mutex_unlock(&radar_data_mutex); // --- MỞ KHÓA ---				
				}
				
					Radar_taogia_Encoder();
			}

			if(PC8.BS_Start == 1)
			{
				if(Radar.Beta_Re_en == 1) _UDP_Process_Bam_Sat_PC1(); // xu ly du lieu theo video PC1
			}

			if(Radar.Beta_Re_en == 1)
			{
				//Erob_Process(); // xu ly bam sat, quay dong co
				Radar.Beta_Re_en = 0;
			}
//			}


		}
	}
}

void _UDP_PC8_Send(void)
{
	if(Radar.UDP_Step2 >= Radar.UDP_Delay_T5) // 30
	{
			Radar.UDP_PC8_Start = 1;
			Radar.UDP_Step2 = 0;
	}

	if((Radar.UDP_PC8_Start == 1)|| // cho pehp gui du lieu
	   (Radar.UDP_PC8_Start2 == 1)) // cap nhat file excel
	{
		Radar.UDP_PC8_Start = 0;
		Radar.UDP_PC8_Start2 = 0;

		PC8.code_data2 = Radar_Hyper_Input(_Input_trigger_sel);

		if(PC8.Video_Val <= 1)
		{
			if(Radar.UDP_PC8_Read_video == 0) // bat dau qua trinh doc
			{
				Radar.UDP_PC8_Read_video = 1;
				System_Set_BS_Sel(0);
				Radar.UDP_PC8_Read_Point  = 0;
			}
		}

		Radar.UDP_counter3++;
		PC8.Speed_Val[7] = PC8.Speed_Val[6];
		PC8.Speed_Val[6] = PC8.Speed_Val[5];
		PC8.Speed_Val[5] = PC8.Speed_Val[4];
		PC8.Speed_Val[4] = PC8.Speed_Val[3];
		PC8.Speed_Val[3] = PC8.Speed_Val[2];
		PC8.Speed_Val[2] = PC8.Speed_Val[1];
		PC8.Speed_Val[1] = PC8.Speed_Val[0];
		PC8.Speed_Val[0] = ((PC8.Alpha_Delta_ENC * 600 * 1000) / 524288); // Beta_Delta_ENC

		PC8.Beta_Speeds = (	PC8.Speed_Val[0] +
							PC8.Speed_Val[1] +
							PC8.Speed_Val[2] +
							PC8.Speed_Val[3] +
							PC8.Speed_Val[4] +
							PC8.Speed_Val[5] +
							PC8.Speed_Val[6] +
							PC8.Speed_Val[7]) / 8;

		Frame.Radar_Speed = PC8.Beta_Speeds;

		if(PC8.Phase_Mode == 0)
		{
			PC8_Get_ADC_Val();
			PC8_Get_Data(); // lay du lieu gui PC8

			//tinh toan goc
			if(PC8.BS_Start == 0) _UDP_Process_Bam_Sat_PC8();

			//udpsenderr = udp_sendto(&send_pcb2, psnd3, &RemoteAddr1, RemotePort5);
			// 2. Gọi hàm sendto()
			bytes_sent = sendto(udp_conn.sockfd,          // Socket để gửi đi
                    &UDP_Frame,              // Con trỏ trực tiếp đến dữ liệu (thay cho pbuf)
                    sizeof(UDP_Frame),       // Kích thước của dữ liệu
                    0,                        // Cờ (flags), thường là 0
                    (const struct sockaddr *)&udp_conn.remote_pc2_addr, // Địa chỉ đích
                    sizeof(udp_conn.remote_pc2_addr));                  // Kích thước địa chỉ đích

			PC8.Phase_Step0 = 0;

		}
		else if(PC8.Phase_Mode == 1) // gui goi tin chinh Phase, dang bi treo
		{
			//if(((Radar.IP_Code == 0)&&(PC8.code_data2 == 0))||
			//   ((Radar.IP_Code >= 1)&&(PC8.code_data2 == 1)))

			//if(PC8.code_data2 == 1)
			//{
				_UDP_Process_Phase(); // xu ly gui du lieu phase
			//}
		}
	}

}
