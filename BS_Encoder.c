#include "main.h"
// #include "MI_505.h"
// #include "DK_Maythu.h"
// #include "CAN_Control.h"
// #include <math.h>

void Erob_Get_ENC1_Data(void)
{
	CAN_Controls.FifoFull1_Timeout = 0;
	CAN_Controls.Motor1_OK		   = 1;

	Erob_Motor.Motor_PhuongVi.UDP_Datas1 = (CAN_Controls.RxFrame[2] & 0xff000000) >> 24;
	Erob_Motor.Motor_PhuongVi.UDP_Datas2 = (CAN_Controls.RxFrame[2] & 0x00ff0000) >> 16;
	Erob_Motor.Motor_PhuongVi.UDP_Datas3 = (CAN_Controls.RxFrame[2] & 0x0000ff00) >> 8;
	Erob_Motor.Motor_PhuongVi.UDP_Datas4 = (CAN_Controls.RxFrame[2] & 0x000000ff);

	Erob_Motor.Motor_PhuongVi.Zynq_Encoder = 	Erob_Motor.Motor_PhuongVi.UDP_Datas1 +
												Erob_Motor.Motor_PhuongVi.UDP_Datas2 * 0x00000100 +
												(Erob_Motor.Motor_PhuongVi.UDP_Datas3 & 0x07) * 0x00010000;
												//Erob_Motor.Motor_PhuongVi.UDP_Datas4 * 0x01000000;

	if(Erob_Motor.Motor_PhuongVi.Zynq_Encoder >= Erob_Motor.Motor_PhuongVi.Absolute_ENC)
	{
		PC8.Beta_Delta_ENC = Erob_Motor.Motor_PhuongVi.Zynq_Encoder - Erob_Motor.Motor_PhuongVi.Absolute_ENC;
	}
	else if(Erob_Motor.Motor_PhuongVi.Zynq_Encoder < Erob_Motor.Motor_PhuongVi.Absolute_ENC)
	{
		PC8.Beta_Delta_ENC = Erob_Motor.Motor_PhuongVi.Absolute_ENC - Erob_Motor.Motor_PhuongVi.Zynq_Encoder;
	}

	Erob_Motor.Motor_PhuongVi.Absolute_ENC	= Erob_Motor.Motor_PhuongVi.Zynq_Encoder;//0 % 524288;


	Erob_Motor.Motor_PhuongVi.UDP_Angle0 = Erob_Motor.Motor_PhuongVi.Erob_Calib - Erob_Motor.Motor_PhuongVi.Absolute_ENC;

	if(Erob_Motor.Motor_PhuongVi.UDP_Angle0 < 0)
		Erob_Motor.Motor_PhuongVi.UDP_Angle0 = Erob_Motor.Motor_PhuongVi.UDP_Angle0 + 524288;

	Erob_Motor.Motor_PhuongVi.UDP_Angle  	= Erob_Motor.Motor_PhuongVi.UDP_Angle0 * 0.0006866455078125;
	Erob_Motor.Motor_PhuongVi.UDP_Lygiac 	= Erob_Motor.Motor_PhuongVi.UDP_Angle * 16.66666666666667;

	Erob_Motor.Motor_PhuongVi.BS_Angle 		= Erob_Motor.Motor_PhuongVi.UDP_Lygiac;//
	Erob_Motor.Motor_PhuongVi.BS_Anglef 	= Erob_Motor.Motor_PhuongVi.UDP_Angle;//

}

void Erob_Get_ENC2_Data(void)
{
	CAN_Controls.FifoFull2_Timeout = 0;
	CAN_Controls.Motor2_OK		   = 1;

	Erob_Motor.Motor_GocTa.UDP_Datas1 = (CAN_Controls.RxFrame2[2] & 0xff000000) >> 24;
	Erob_Motor.Motor_GocTa.UDP_Datas2 = (CAN_Controls.RxFrame2[2] & 0x00ff0000) >> 16;
	Erob_Motor.Motor_GocTa.UDP_Datas3 = (CAN_Controls.RxFrame2[2] & 0x0000ff00) >> 8;
	Erob_Motor.Motor_GocTa.UDP_Datas4 = (CAN_Controls.RxFrame2[2] & 0x000000ff);
	Erob_Motor.Motor_GocTa.Zynq_Encoder = 	Erob_Motor.Motor_GocTa.UDP_Datas1 +
											Erob_Motor.Motor_GocTa.UDP_Datas2 * 0x00000100 +
											(Erob_Motor.Motor_GocTa.UDP_Datas3 & 0x07) * 0x00010000;// +
											//Erob_Motor.Motor_GocTa.UDP_Datas4 * 0x01000000;

	if(Erob_Motor.Motor_GocTa.Zynq_Encoder >= Erob_Motor.Motor_GocTa.Absolute_ENC)
		{
			PC8.Alpha_Delta_ENC = Erob_Motor.Motor_GocTa.Zynq_Encoder - Erob_Motor.Motor_GocTa.Absolute_ENC;
		}
		else if(Erob_Motor.Motor_GocTa.Zynq_Encoder < Erob_Motor.Motor_GocTa.Absolute_ENC)
		{
			PC8.Alpha_Delta_ENC = Erob_Motor.Motor_GocTa.Absolute_ENC - Erob_Motor.Motor_GocTa.Zynq_Encoder;
		}

	Erob_Motor.Motor_GocTa.Absolute_ENC	= Erob_Motor.Motor_GocTa.Zynq_Encoder;

	Erob_Motor.Motor_GocTa.UDP_Angle0   = Erob_Motor.Motor_GocTa.Erob_Calib - Erob_Motor.Motor_GocTa.Absolute_ENC;

	if(Erob_Motor.Motor_GocTa.UDP_Angle0 < 0)
		Erob_Motor.Motor_GocTa.UDP_Angle0 = Erob_Motor.Motor_GocTa.UDP_Angle0 + 524288;

	Erob_Motor.Motor_GocTa.UDP_Angle    = Erob_Motor.Motor_GocTa.UDP_Angle0 * 0.0006866455078125;
	Erob_Motor.Motor_GocTa.UDP_Lygiac   = Erob_Motor.Motor_GocTa.UDP_Angle * 16.66666666666667;

	Erob_Motor.Motor_GocTa.BS_Angle = Erob_Motor.Motor_GocTa.UDP_Lygiac;//UDP_Angle * 10;	//
	Erob_Motor.Motor_GocTa.BS_Anglef = Erob_Motor.Motor_GocTa.UDP_Angle;		//
}

// void Erob_Read_ENC1(void)
// {
// 	CAN_Controls.TxFrame[2] = 0x00000200; // 00 02
// 	SendFrame2(&Erob_Can0, 2);
// 	RecvFrame2(&Erob_Can0);
// }

// void Erob_Read_ENC2(void)
// {
// 	CAN_Controls.TxFrame2[2] = 0x00000200; // 00 02
// 	SendFrame3(&Erob_Can1, 2);
// 	RecvFrame3(&Erob_Can1);
// }

// Hàm gửi chung, có xử lý timeout
int SendFrame_Linux(LinuxCanBus *bus, uint32_t can_id, uint8_t dlc, const uint8_t *data) {
    if (!bus->is_connected) return -1;

    struct can_frame frame;
    frame.can_id = can_id;
    frame.can_dlc = dlc;
    memcpy(frame.data, data, dlc);

    // Lệnh write sẽ tự block cho đến khi gửi được, hoặc timeout (50ms)
    ssize_t nbytes = write(bus->socket_fd, &frame, sizeof(struct can_frame));

    if (nbytes < 0) {
        // Lỗi EAGAIN/EWOULDBLOCK nghĩa là timeout
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("[CAN] Send timeout on %s. Marking as disconnected.\n", bus->interface_name);
        } else {
            perror("[CAN] Error writing to socket");
        }
        return -1; // Báo lỗi
    }

    return 0; // Gửi thành công
}

// Thay thế SendFrame2
void Erob_Read_ENC1_S2(void) {
    // Giả định CAN_Controls.TxFrame[2] chứa dữ liệu 8 byte
    uint8_t *payload = (uint8_t *)&CAN_Controls.TxFrame[2]; 
    if (SendFrame_Linux(&Erob_Can0, TEST_MESSAGE_ID, 8, payload) != 0) {
        CAN_Controls.Motor1_disconect = 1;
    } else {
        CAN_Controls.FifoFull1_Timeout = 0; // Reset bộ đếm timeout
    }
}

// Thay thế SendFrame3
void Erob_Read_ENC2_S2(void) {
    uint8_t *payload = (uint8_t *)&CAN_Controls.TxFrame2[2];
    if (SendFrame_Linux(&Erob_Can1, TEST_MESSAGE_ID, 8, payload) != 0) {
        CAN_Controls.Motor2_disconect = 1;
    } else {
        CAN_Controls.FifoFull2_Timeout = 0;
    }
}

void Erob_Read_Posision(void)
{

	if(CAN_Controls.Motor1_disconect == 0)
	{
		Erob_Read_ENC1_S2();
	}
	else if(CAN_Controls.Motor1_disconect == 1)
	{
		if(CAN_Controls.Motor1_Restart == 0)
		{
			CAN_Controls.Motor1_Timeout++;
			if(CAN_Controls.Motor1_Timeout > 1000)
			{
				CAN_Controls.Motor1_Restart	 	 = 1;
				CAN_Controls.Motor1_Timeout 	 = 0;
				CAN_Controls.Motor1_Restart_Time = 0;
				CAN_Controls.Motor1_OK			 = 0;
			}
		}
		if(CAN_Controls.Motor1_Restart == 1)
		{
			CAN_Controls.Motor1_Restart_Time++;
			if(CAN_Controls.Motor1_Restart_Time >= 10000)
			{
				CAN_Controls.Motor1_Restart 	 = 0;
				CAN_Controls.Motor1_disconect 	 = 0;
				CAN_Controls.Motor1_Timeout 	 = 0;
				CAN_Controls.Motor1_Restart_Time = 0;
			}
		}
	}

	if(CAN_Controls.Motor2_disconect == 0)
	{
		Erob_Read_ENC2_S2();
	}
	else if(CAN_Controls.Motor2_disconect == 1)
	{
		if(CAN_Controls.Motor2_Restart == 0)
		{
			CAN_Controls.Motor2_Timeout++;
			if(CAN_Controls.Motor2_Timeout > 1000)
			{
				CAN_Controls.Motor2_Restart 	 = 1;
				CAN_Controls.Motor2_Timeout 	 = 0;
				CAN_Controls.Motor2_Restart_Time = 0;
				CAN_Controls.Motor2_OK			 = 0;
			}
		}
		if(CAN_Controls.Motor2_Restart == 1)
		{
			CAN_Controls.Motor2_Restart_Time++;
			if(CAN_Controls.Motor2_Restart_Time >= 10000)
			{
				CAN_Controls.Motor2_Restart 		= 0;
				CAN_Controls.Motor2_disconect 		= 0;
				CAN_Controls.Motor2_Timeout 		= 0;
				CAN_Controls.Motor2_Restart_Time 	= 0;
			}
		}
	}

}
