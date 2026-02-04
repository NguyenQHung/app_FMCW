#include "Erob.h"
#include "MI_505.h"
#include "DK_Maythu.h"
#include "CAN_Control.h"
#include <math.h>

/**
 * @brief Khoi tao cac tham so cho bo loc Kalman.
 *
 * @param bl Con tro toi cau truc BoLocKalman.
 * @param q_param Nhieu qua trinh. Quyet dinh muc do "muot" cua tin hieu loc.
 *                Gia tri q nho hon -> tin hieu loc muot hon nhung phan ung cham hon.
 * @param r_param Nhieu do luong. Quyet dinh muc do tin tuong vao gia tri do moi.
 *                Gia tri r nho hon -> bo loc bam theo gia tri do nhanh hon nhung de bi anh huong boi nhieu.
 * @param x_ban_dau Gia tri ban dau de bat dau uoc tinh. Thuong la gia tri do dau tien.
 * @param p_ban_dau Sai so uoc tinh ban dau. Mot gia tri duong lon (vi du: 1) la mot lua chon an toan.
 */
void kalman_E_khoi_tao(double q_param, double r_param, double x_ban_dau, double p_ban_dau)
{
	PC8._E_Kalman.q = q_param;
	PC8._E_Kalman.r = r_param;
	PC8._E_Kalman.p = p_ban_dau;
	PC8._E_Kalman.x = x_ban_dau;
	PC8._E_Kalman.k = 0; // Se duoc tinh trong lan cap nhat dau tien
}

void kalman_H_khoi_tao(double q_param, double r_param, double x_ban_dau, double p_ban_dau)
{
	PC8._H_Kalman.q = q_param;
	PC8._H_Kalman.r = r_param;
	PC8._H_Kalman.p = p_ban_dau;
	PC8._H_Kalman.x = x_ban_dau;
	PC8._H_Kalman.k = 0; // Se duoc tinh trong lan cap nhat dau tien
}

/**
 * @brief Cap nhat bo loc voi mot gia tri do moi.
 *
 * @param bl Con tro toi cau truc BoLocKalman.
 * @param gia_tri_do Gia tri do moi (tin hieu nhieu dau vao).
 * @return double Gia tri da duoc loc.
 */
double kalman_E_cap_nhat(double gia_tri_do)
{
    // 1. Buoc du doan (Prediction)
    // Trong truong hop 1D don gian nay, trang thai du doan giong trang thai truoc do.
    // x_du_doan = x_truoc_do

    // Cap nhat sai so hiep phuong sai du doan: p = p + q
	PC8._E_Kalman.p = PC8._E_Kalman.p + PC8._E_Kalman.q;

    // 2. Buoc cap nhat (Update)
    // Tinh Kalman Gain: k = p / (p + r)
	PC8._E_Kalman.k = PC8._E_Kalman.p / (PC8._E_Kalman.p + PC8._E_Kalman.r);

    // Cap nhat gia tri uoc tinh voi gia tri do moi (gia_tri_do): x = x + k * (gia_tri_do - x)
	PC8._E_Kalman.x = PC8._E_Kalman.x + PC8._E_Kalman.k * (gia_tri_do - PC8._E_Kalman.x);

    // Cap nhat sai so hiep phuong sai cua uoc tinh: p = (1 - k) * p
	PC8._E_Kalman.p = (1 - PC8._E_Kalman.k) * PC8._E_Kalman.p;

    return PC8._E_Kalman.x;
}

double kalman_H_cap_nhat(double gia_tri_do)
{
    // 1. Buoc du doan (Prediction)
    // Trong truong hop 1D don gian nay, trang thai du doan giong trang thai truoc do.
    // x_du_doan = x_truoc_do

    // Cap nhat sai so hiep phuong sai du doan: p = p + q
	PC8._H_Kalman.p = PC8._H_Kalman.p + PC8._H_Kalman.q;

    // 2. Buoc cap nhat (Update)
    // Tinh Kalman Gain: k = p / (p + r)
	PC8._H_Kalman.k = PC8._H_Kalman.p / (PC8._H_Kalman.p + PC8._H_Kalman.r);

    // Cap nhat gia tri uoc tinh voi gia tri do moi (gia_tri_do): x = x + k * (gia_tri_do - x)
	PC8._H_Kalman.x = PC8._H_Kalman.x + PC8._H_Kalman.k * (gia_tri_do - PC8._H_Kalman.x);

    // Cap nhat sai so hiep phuong sai cua uoc tinh: p = (1 - k) * p
	PC8._H_Kalman.p = (1 - PC8._H_Kalman.k) * PC8._H_Kalman.p;

    return PC8._H_Kalman.x;
}

void kalman_Innit(void)
{
	kalman_E_khoi_tao(0.05, 0.40, 0.0, 1.0);
	kalman_H_khoi_tao(0.05, 0.40, 0.0, 1.0);
}
