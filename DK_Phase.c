#include "DK_Maythu.h"
#include "ADF4159.h"
#include "ADF4360.h"
#include "Radar_HW.h"
#include "MI_505.h"
#include "Erob.h"
//#include "xparameters.h"
#include "CAN_Control.h"

/****************************************************************************/

void Radar_Update_Phase1(uint16_t Sine, uint16_t cos)
{
	Radar_Hyper_Output(Sinphase1, Sine);
	Radar_Hyper_Output(Cosphase1, cos);
}

void Radar_Update_Phase2(uint16_t Sine, uint16_t cos)
{
	Radar_Hyper_Output(Sinphase2, Sine);
	Radar_Hyper_Output(Cosphase2, cos);
}

void Radar_Update_Phase3(uint16_t Sine, uint16_t cos)
{
	Radar_Hyper_Output(Sinphase3, Sine);
	Radar_Hyper_Output(Cosphase3, cos);
}

void Radar_Update_Phase4(uint16_t Sine, uint16_t cos)
{
	Radar_Hyper_Output(Sinphase4, Sine);
	Radar_Hyper_Output(Cosphase4, cos);
}

void Radar_Update_Phase5(uint16_t Sine, uint16_t cos)
{
	Radar_Hyper_Output(Sinphase5, Sine);
	Radar_Hyper_Output(Cosphase5, cos);
}

void Radar_Update_Phase6(uint16_t Sine, uint16_t cos)
{
	Radar_Hyper_Output(Sinphase6, Sine);
	Radar_Hyper_Output(Cosphase6, cos);
}

void Radar_Update_Phase7(uint16_t Sine, uint16_t cos)
{
	Radar_Hyper_Output(Sinphase7, Sine);
	Radar_Hyper_Output(Cosphase7, cos);
}

void Radar_Update_Phase8(uint16_t Sine, uint16_t cos)
{
	Radar_Hyper_Output(Sinphase8, Sine);
	Radar_Hyper_Output(Cosphase8, cos);
}

void Radar_Update_Phase9(uint16_t Sine, uint16_t cos)
{
	Radar_Hyper_Output(Sinphase9, Sine);
	Radar_Hyper_Output(Cosphase9, cos);
}

void Radar_Update_Phase10(uint16_t Sine, uint16_t cos)
{
	Radar_Hyper_Output(Sinphase10, Sine);
	Radar_Hyper_Output(Cosphase10, cos);
}

void Radar_Update_Phase11(uint16_t Sine, uint16_t cos)
{
	Radar_Hyper_Output(Sinphase11, Sine);
	Radar_Hyper_Output(Cosphase11, cos);
}

void Radar_Update_Phase12(uint16_t Sine, uint16_t cos)
{
	Radar_Hyper_Output(Sinphase12, Sine);
	Radar_Hyper_Output(Cosphase12, cos);
}

void Radar_Update_Phase13(uint16_t Sine, uint16_t cos)
{
	Radar_Hyper_Output(Sinphase13, Sine);
	Radar_Hyper_Output(Cosphase13, cos);
}

void Radar_Update_Phase14(uint16_t Sine, uint16_t cos)
{
	Radar_Hyper_Output(Sinphase14, Sine);
	Radar_Hyper_Output(Cosphase14, cos);
}

void Radar_Update_Phase15(uint16_t Sine, uint16_t cos)
{
	Radar_Hyper_Output(Sinphase15, Sine);
	Radar_Hyper_Output(Cosphase15, cos);
}

void Radar_Update_Phase16(uint16_t Sine, uint16_t cos)
{
	Radar_Hyper_Output(Sinphase16, Sine);
	Radar_Hyper_Output(Cosphase16, cos);
}

void Radar_Innit_Phase(void)
{
	Radar_Update_Phase1(0, 32767);
	Radar_Update_Phase2(0, 32767);
	Radar_Update_Phase3(0, 32767);
	Radar_Update_Phase4(0, 32767);
	Radar_Update_Phase5(0, 32767);
	Radar_Update_Phase6(0, 32767);
	Radar_Update_Phase7(0, 32767);
	Radar_Update_Phase8(0, 32767);
	Radar_Update_Phase9(0, 32767);
	Radar_Update_Phase10(0, 32767);
	Radar_Update_Phase11(0, 32767);
	Radar_Update_Phase12(0, 32767);
	Radar_Update_Phase13(0, 32767);
	Radar_Update_Phase14(0, 32767);
	Radar_Update_Phase15(0, 32767);
	Radar_Update_Phase16(0, 32767);
}
