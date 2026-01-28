#include "DK_Maythu.h"
#include "ADF4159.h"
#include "ADF4360.h"
#include "Radar_HW.h"
#include "MI_505.h"
#include "Erob.h"
//#include "xparameters.h"
#include "CAN_Control.h"
#define _GNU_SOURCE
#include <math.h>
#include <complex.h>


void Radar_Update_V1(uint16_t Beam, uint16_t Sine, uint16_t cos)
{

}

void Radar_Update_V2(uint16_t Beam, uint16_t Sine, uint16_t cos)
{

}

void Radar_Update_V3(uint16_t Beam, uint16_t Sine, uint16_t cos)
{

}

void Radar_Update_V4(uint16_t Beam, uint16_t Sine, uint16_t cos)
{

}

void Caculator_V1_Value(void)
{
	uint8_t k = 0;
	//PC8.V1.E_Angle = 0.0f;
	//PC8.V1.H_Angle = -2.0f;
	/*
	% Beam 1 SS
	phiB = 0;
	epsilonB = 6;
	for n = 1:nElements
	    tau_Beam1 = (cosd(epsilonB)*sind(phiB)*yPos(n)+...
	        sind(epsilonB)*zPos(n));
	    B1(n) = exp(-1j*2*pi*tau_Beam1/lambda);
	end

	so phuc: _Complex_I = 1j

	*/

	double epsilonB = PC8.V1.E_Angle * M_PI / 180.0; // Convert degrees to radians
	double phiB 	= PC8.V1.H_Angle * M_PI / 180.0; // Convert degrees to radians

	for(k = 0; k< 16; k++)
	{
		PC8.V1.tau[k] 	= (cos(epsilonB)*sin(phiB)*PC8.yPos[k]+sin(epsilonB)*PC8.zPos[k]);
		// #define _Complex_I 1.0fi
		PC8.V1.A[k]   	= cexp(-_Complex_I * 2 * M_PI * PC8.V1.tau[k] / PC8.Lamda);

		PC8.V1._Image 	= cimag(PC8.V1.A[k]) * 32767;
		PC8.V1._Real 	= creal(PC8.V1.A[k]) * 32767;

		if(PC8.V1._Image >= 0.0) PC8.V1.Image[k] = (uint16_t)(PC8.V1._Image);
		else  PC8.V1.Image[k] = (uint16_t)(PC8.V1._Image + 65534);

		if(PC8.V1._Real >= 0.0) PC8.V1.Real[k] = (uint16_t)(PC8.V1._Real);
		else  PC8.V1.Real[k] = (uint16_t)(PC8.V1._Real + 65536);

	}


	PC8.V1.Innit_done = 1;
	//float		H_Angle;
	//float		E_Angle;

}

void Caculator_V2_Value(void)
{
	uint8_t k = 0;
	/*
	% Beam 1 SS
	phiB = 0;
	epsilonB = 6;
	for n = 1:nElements
	    tau_Beam1 = (cosd(epsilonB)*sind(phiB)*yPos(n)+...
	        sind(epsilonB)*zPos(n));
	    B1(n) = exp(-1j*2*pi*tau_Beam1/lambda);
	end

	so phuc: _Complex_I = 1j

	*/

	double epsilonB = PC8.V2.E_Angle * M_PI / 180.0; // Convert degrees to radians
	double phiB 	= PC8.V2.H_Angle * M_PI / 180.0; // Convert degrees to radians

	for(k = 0; k< 16; k++)
	{
		PC8.V2.tau[k] = (cos(epsilonB)*sin(phiB)*PC8.yPos[k]+sin(epsilonB)*PC8.zPos[k]);
		// #define _Complex_I 1.0fi
		PC8.V2.A[k]   = cexp(-_Complex_I * 2 * M_PI * PC8.V2.tau[k] / PC8.Lamda);

		PC8.V2._Image 	= cimag(PC8.V2.A[k]) * 32767;
		PC8.V2._Real 	= creal(PC8.V2.A[k]) * 32767;

		if(PC8.V2._Image >= 0.0) PC8.V2.Image[k] = (uint16_t)(PC8.V2._Image);
		else  PC8.V2.Image[k] = (uint16_t)(PC8.V2._Image + 65536);

		if(PC8.V2._Real >= 0.0) PC8.V2.Real[k] = (uint16_t)(PC8.V2._Real);
		else  PC8.V2.Real[k] = (uint16_t)(PC8.V2._Real + 65536);

	}


	PC8.V3.Innit_done = 1;
	//float		H_Angle;
	//float		E_Angle;

}

void Caculator_V3_Value(void)
{
	uint8_t k = 0;
	/*
	% Beam 1 SS
	phiB = 0;
	epsilonB = 6;
	for n = 1:nElements
	    tau_Beam1 = (cosd(epsilonB)*sind(phiB)*yPos(n)+...
	        sind(epsilonB)*zPos(n));
	    B1(n) = exp(-1j*2*pi*tau_Beam1/lambda);
	end

	so phuc: _Complex_I = 1j

	*/

	double epsilonB = PC8.V3.E_Angle * M_PI / 180.0; // Convert degrees to radians
	double phiB 	= PC8.V3.H_Angle * M_PI / 180.0; // Convert degrees to radians

	for(k = 0; k< 16; k++)
	{
		PC8.V3.tau[k] = (cos(epsilonB)*sin(phiB)*PC8.yPos[k]+sin(epsilonB)*PC8.zPos[k]);
		// #define _Complex_I 1.0fi
		PC8.V3.A[k]   = cexp(-_Complex_I * 2 * M_PI * PC8.V3.tau[k] / PC8.Lamda);

		PC8.V3._Image 	= cimag(PC8.V3.A[k]) * 32767;
		PC8.V3._Real 	= creal(PC8.V3.A[k]) * 32767;

		if(PC8.V3._Image >= 0.0) PC8.V3.Image[k] = (uint16_t)(PC8.V3._Image);
		else  PC8.V3.Image[k] = (uint16_t)(PC8.V3._Image + 65536);

		if(PC8.V3._Real >= 0.0) PC8.V3.Real[k] = (uint16_t)(PC8.V3._Real);
		else  PC8.V3.Real[k] = (uint16_t)(PC8.V3._Real + 65536);

	}


	PC8.V3.Innit_done = 1;
	//float		H_Angle;
	//float		E_Angle;

}

void Caculator_V4_Value(void)
{
	uint8_t k = 0;
	//PC8.V1.E_Angle = 0.0f;
	//PC8.V1.H_Angle = -2.0f;
	/*
	% Beam 1 SS
	phiB = 0;
	epsilonB = 6;
	for n = 1:nElements
	    tau_Beam1 = (cosd(epsilonB)*sind(phiB)*yPos(n)+...
	        sind(epsilonB)*zPos(n));
	    B1(n) = exp(-1j*2*pi*tau_Beam1/lambda);
	end

	so phuc: _Complex_I = 1j

	*/

	double epsilonB = PC8.V4.E_Angle * M_PI / 180.0; // Convert degrees to radians
	double phiB 	= PC8.V4.H_Angle * M_PI / 180.0; // Convert degrees to radians

	for(k = 0; k< 16; k++)
	{
		PC8.V4.tau[k] = (cos(epsilonB)*sin(phiB)*PC8.yPos[k]+sin(epsilonB)*PC8.zPos[k]);
		// #define _Complex_I 1.0fi
		PC8.V4.A[k]   = cexp(-_Complex_I * 2 * M_PI * PC8.V4.tau[k] / PC8.Lamda);

		PC8.V4._Image 	= cimag(PC8.V4.A[k]) * 32767;
		PC8.V4._Real 	= creal(PC8.V4.A[k]) * 32767;

		if(PC8.V4._Image >= 0.0) PC8.V4.Image[k] = (uint16_t)(PC8.V4._Image);
		else  PC8.V4.Image[k] = (uint16_t)(PC8.V4._Image + 65536);

		if(PC8.V4._Real >= 0.0) PC8.V4.Real[k] = (uint16_t)(PC8.V4._Real);
		else  PC8.V4.Real[k] = (uint16_t)(PC8.V4._Real + 65536);
	}

	PC8.V4.Innit_done = 1;

}

void Update_V1(void)
{
	Radar_Update_V1(0, PC8.V1.Image[0], PC8.V1.Real[0]);
	Radar_Update_V1(1, PC8.V1.Image[1], PC8.V1.Real[1]);
	Radar_Update_V1(2, PC8.V1.Image[2], PC8.V1.Real[2]);
	Radar_Update_V1(3, PC8.V1.Image[3], PC8.V1.Real[3]);
	Radar_Update_V1(4, PC8.V1.Image[4], PC8.V1.Real[4]);
	Radar_Update_V1(5, PC8.V1.Image[5], PC8.V1.Real[5]);
	Radar_Update_V1(6, PC8.V1.Image[6], PC8.V1.Real[6]);
	Radar_Update_V1(7, PC8.V1.Image[7], PC8.V1.Real[7]);
	Radar_Update_V1(8, PC8.V1.Image[8], PC8.V1.Real[8]);
	Radar_Update_V1(9, PC8.V1.Image[9], PC8.V1.Real[9]);
	Radar_Update_V1(10, PC8.V1.Image[10], PC8.V1.Real[10]);
	Radar_Update_V1(11, PC8.V1.Image[11], PC8.V1.Real[11]);
	Radar_Update_V1(12, PC8.V1.Image[12], PC8.V1.Real[12]);
	Radar_Update_V1(13, PC8.V1.Image[13], PC8.V1.Real[13]);
	Radar_Update_V1(14, PC8.V1.Image[14], PC8.V1.Real[14]);
	Radar_Update_V1(15, PC8.V1.Image[15], PC8.V1.Real[15]);
}

void Update_V2(void)
{
	Radar_Update_V2(0, PC8.V2.Image[0], PC8.V2.Real[0]);
	Radar_Update_V2(1, PC8.V2.Image[1], PC8.V2.Real[1]);
	Radar_Update_V2(2, PC8.V2.Image[2], PC8.V2.Real[2]);
	Radar_Update_V2(3, PC8.V2.Image[3], PC8.V2.Real[3]);
	Radar_Update_V2(4, PC8.V2.Image[4], PC8.V2.Real[4]);
	Radar_Update_V2(5, PC8.V2.Image[5], PC8.V2.Real[5]);
	Radar_Update_V2(6, PC8.V2.Image[6], PC8.V2.Real[6]);
	Radar_Update_V2(7, PC8.V2.Image[7], PC8.V2.Real[7]);
	Radar_Update_V2(8, PC8.V2.Image[8], PC8.V2.Real[8]);
	Radar_Update_V2(9, PC8.V2.Image[9], PC8.V2.Real[9]);
	Radar_Update_V2(10, PC8.V2.Image[10], PC8.V2.Real[10]);
	Radar_Update_V2(11, PC8.V2.Image[11], PC8.V2.Real[11]);
	Radar_Update_V2(12, PC8.V2.Image[12], PC8.V2.Real[12]);
	Radar_Update_V2(13, PC8.V2.Image[13], PC8.V2.Real[13]);
	Radar_Update_V2(14, PC8.V2.Image[14], PC8.V2.Real[14]);
	Radar_Update_V2(15, PC8.V2.Image[15], PC8.V2.Real[15]);
}

void Update_V3(void)
{
	Radar_Update_V3(0, PC8.V3.Image[0], PC8.V3.Real[0]);
	Radar_Update_V3(1, PC8.V3.Image[1], PC8.V3.Real[1]);
	Radar_Update_V3(2, PC8.V3.Image[2], PC8.V3.Real[2]);
	Radar_Update_V3(3, PC8.V3.Image[3], PC8.V3.Real[3]);
	Radar_Update_V3(4, PC8.V3.Image[4], PC8.V3.Real[4]);
	Radar_Update_V3(5, PC8.V3.Image[5], PC8.V3.Real[5]);
	Radar_Update_V3(6, PC8.V3.Image[6], PC8.V3.Real[6]);
	Radar_Update_V3(7, PC8.V3.Image[7], PC8.V3.Real[7]);
	Radar_Update_V3(8, PC8.V3.Image[8], PC8.V3.Real[8]);
	Radar_Update_V3(9, PC8.V3.Image[9], PC8.V3.Real[9]);
	Radar_Update_V3(10, PC8.V3.Image[10], PC8.V3.Real[10]);
	Radar_Update_V3(11, PC8.V3.Image[11], PC8.V3.Real[11]);
	Radar_Update_V3(12, PC8.V3.Image[12], PC8.V3.Real[12]);
	Radar_Update_V3(13, PC8.V3.Image[13], PC8.V3.Real[13]);
	Radar_Update_V3(14, PC8.V3.Image[14], PC8.V3.Real[14]);
	Radar_Update_V3(15, PC8.V3.Image[15], PC8.V3.Real[15]);
}

void Update_V4(void)
{
	Radar_Update_V4(0, PC8.V4.Image[0], PC8.V4.Real[0]);
	Radar_Update_V4(1, PC8.V4.Image[1], PC8.V4.Real[1]);
	Radar_Update_V4(2, PC8.V4.Image[2], PC8.V4.Real[2]);
	Radar_Update_V4(3, PC8.V4.Image[3], PC8.V4.Real[3]);
	Radar_Update_V4(4, PC8.V4.Image[4], PC8.V4.Real[4]);
	Radar_Update_V4(5, PC8.V4.Image[5], PC8.V4.Real[5]);
	Radar_Update_V4(6, PC8.V4.Image[6], PC8.V4.Real[6]);
	Radar_Update_V4(7, PC8.V4.Image[7], PC8.V4.Real[7]);
	Radar_Update_V4(8, PC8.V4.Image[8], PC8.V4.Real[8]);
	Radar_Update_V4(9, PC8.V4.Image[9], PC8.V4.Real[9]);
	Radar_Update_V4(10, PC8.V4.Image[10], PC8.V4.Real[10]);
	Radar_Update_V4(11, PC8.V4.Image[11], PC8.V4.Real[11]);
	Radar_Update_V4(12, PC8.V4.Image[12], PC8.V4.Real[12]);
	Radar_Update_V4(13, PC8.V4.Image[13], PC8.V4.Real[13]);
	Radar_Update_V4(14, PC8.V4.Image[14], PC8.V4.Real[14]);
	Radar_Update_V4(15, PC8.V4.Image[15], PC8.V4.Real[15]);
}

void _Check_Beam_config(void)
{

	if((PC8.V1.H_Anglei != PC8.V1.H_Angle)||
	   (PC8.V1.E_Anglei != PC8.V1.E_Angle))
		{
			PC8.V1.H_Anglei	= PC8.V1.H_Angle;
			PC8.V1.E_Anglei = PC8.V1.E_Angle;

			Caculator_V1_Value();
			Update_V1();
		}

	if((PC8.V2.H_Anglei != PC8.V2.H_Angle)||
	   (PC8.V2.E_Anglei != PC8.V2.E_Angle))
		{
			PC8.V2.H_Anglei	= PC8.V2.H_Angle;
			PC8.V2.E_Anglei = PC8.V2.E_Angle;

			Caculator_V2_Value();
			Update_V2();
		}

	if((PC8.V3.H_Anglei != PC8.V3.H_Angle)||
	   (PC8.V3.E_Anglei != PC8.V3.E_Angle))
		{
			PC8.V3.H_Anglei	= PC8.V3.H_Angle;
			PC8.V3.E_Anglei = PC8.V3.E_Angle;

			Caculator_V3_Value();
			Update_V3();
		}

	if((PC8.V4.H_Anglei != PC8.V4.H_Angle)||
	   (PC8.V4.E_Anglei != PC8.V4.E_Angle))
		{
			PC8.V4.H_Anglei	= PC8.V4.H_Angle;
			PC8.V4.E_Anglei = PC8.V4.E_Angle;

			Caculator_V4_Value();
			Update_V4();
		}

}

void Innit_Beam_data(void)
{

	/*
	Ts = 1/400e6;
	sysclk = Ts*1e9;
	c = physconst('LightSpeed');
	Fc = 9.2e9; % tan so
	lambda = c/Fc;
	dA = 1.75*lambda;
	nElements = 16;
	xPos = zeros(1, nElements);
	yPos = [-3*dA, -dA, dA, 3*dA, -3*dA, -dA, dA, 3*dA,...
	    -3*dA, -dA, dA, 3*dA, -3*dA, -dA, dA, 3*dA];
	zPos = [3*dA,3*dA,3*dA,3*dA, dA,dA,dA,dA,...
	    -dA,-dA,-dA,-dA, -3*dA,-3*dA,-3*dA,-3*dA];
	phi1 = 0.0;
	epxilon1 = 1.0;
	tau_1 = zeros(1,nElements);
	nphase = 16;
	Abit = 16;
	nddc = 16;
	A = zeros(nElements,1);
	for k = 1:nElements
	    tau_1(k) = (cosd(epxilon1)*sind(phi1)*yPos(k)+...
	        sind(epxilon1)*zPos(k));
	    A(k) = exp(1j*2*pi*tau_1(k)/lambda);
	end
	*/

	PC8.C 		=  299792458;
	PC8.Fc 		= 9200000000;
	PC8.Lamda 	= PC8.C / PC8.Fc;
	PC8.dB 		= 1.756134969325153 * PC8.Lamda;
	PC8.dA 		= 1.725460122699387 * PC8.Lamda;

	// toa do V1[-2; 0]
	PC8.V1.H_Angle 	= -2;
	PC8.V1.E_Angle	= 0;
	// toa do V2[ 2; 0]
	PC8.V2.H_Angle 	= 2;
	PC8.V2.E_Angle	= 0;
	// toa do V3[ 0; 2]
	PC8.V3.H_Angle 	= 0;
	PC8.V3.E_Angle	= 2;
	// toa do V4[ 0;-2]
	PC8.V4.H_Angle 	= 0;
	PC8.V4.E_Angle	= -2;


	PC8.V1.H_Anglei	= -2;
	PC8.V1.E_Anglei = 0;

	PC8.V2.H_Anglei	= 2;
	PC8.V2.E_Anglei = 0;

	PC8.V3.H_Anglei	= 0;
	PC8.V3.E_Anglei = 2;

	PC8.V4.H_Anglei	= 0;
	PC8.V4.E_Anglei = -2;

	PC8.xPos[0]  = 0;
	PC8.xPos[1]  = 0;
	PC8.xPos[2]  = 0;
	PC8.xPos[3]  = 0;
	PC8.xPos[4]  = 0;
	PC8.xPos[5]  = 0;
	PC8.xPos[6]  = 0;
	PC8.xPos[7]  = 0;
	PC8.xPos[8]  = 0;
	PC8.xPos[9]  = 0;
	PC8.xPos[10] = 0;
	PC8.xPos[11] = 0;
	PC8.xPos[12] = 0;
	PC8.xPos[13] = 0;
	PC8.xPos[14] = 0;
	PC8.xPos[15] = 0;

	PC8.yPos[0]  = -3*PC8.dA;
	PC8.yPos[1]  =   -PC8.dA;
	PC8.yPos[2]  =    PC8.dA;
	PC8.yPos[3]  =  3*PC8.dA;
	PC8.yPos[4]  = -3*PC8.dA;
	PC8.yPos[5]  =   -PC8.dA;
	PC8.yPos[6]  =    PC8.dA;
	PC8.yPos[7]  =  3*PC8.dA;
	PC8.yPos[8]  = -3*PC8.dA;
	PC8.yPos[9]  =   -PC8.dA;
	PC8.yPos[10] =    PC8.dA;
	PC8.yPos[11] =  3*PC8.dA;
	PC8.yPos[12] = -3*PC8.dA;
	PC8.yPos[13] =   -PC8.dA;
	PC8.yPos[14] =    PC8.dA;
	PC8.yPos[15] =  3*PC8.dA;

	PC8.zPos[0]  = 3*PC8.dB;
	PC8.zPos[1]  = 3*PC8.dB;
	PC8.zPos[2]  = 3*PC8.dB;
	PC8.zPos[3]  = 3*PC8.dB;
	PC8.zPos[4]  = PC8.dB;
	PC8.zPos[5]  = PC8.dB;
	PC8.zPos[6]  = PC8.dB;
	PC8.zPos[7]  = PC8.dB;
	PC8.zPos[8]  = -PC8.dB;
	PC8.zPos[9]  = -PC8.dB;
	PC8.zPos[10] = -PC8.dB;
	PC8.zPos[11] = -PC8.dB;
	PC8.zPos[12] = -3*PC8.dB;
	PC8.zPos[13] = -3*PC8.dB;
	PC8.zPos[14] = -3*PC8.dB;
	PC8.zPos[15] = -3*PC8.dB;

	Caculator_V1_Value();
	Caculator_V2_Value();
	Caculator_V3_Value();
	Caculator_V4_Value();

	Update_V1();
	Update_V2();
	Update_V3();
	Update_V4();

}
