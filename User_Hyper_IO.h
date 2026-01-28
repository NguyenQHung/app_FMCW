//#include "xparameters.h"
//#include "xil_types.h"

/************************* Hyper_GPIO *************************************************/
#define	_BW_20MHz			0
#define	_BW_18MHz			1
#define	_BW_16MHz			2
#define	_BW_14MHz			3
#define	_BW_12MHz			4
#define	_BW_10MHz			5
#define	_BW_8MHz			6
#define	_BW_6MHz			7
#define	_BW_4MHz			8

#define	_BW2_10MHz			0
#define	_BW2_9MHz			1
#define	_BW2_8MHz			2
#define	_BW2_7MHz			3
#define	_BW2_6MHz			4
#define	_BW2_5MHz			5
#define	_BW2_4MHz			6
#define	_BW2_3MHz			7
#define	_BW2_2MHz			8

/************************* Hyper_GPIO *************************************************/
#define	Hyper_Output_Erob_Freq			0
#define	Hyper_Output_Erob_Dir_Enable 	1
#define	Hyper_Output_CAN_Dir 			2
#define	Hyper_Output_CAN_Dir2 			3
#define	Hyper_Output_Erob_Freq2			4
#define	Hyper_Output_Erob_Dir_Enable2 	5
#define	Number_Pulse1 					6
#define	Pulse_Enable1 					7
#define	Erob_mode1 						8
#define	Erob_start1 					9
#define	Erob_Center1 					10
#define	Erob_stop1 						11
#define	Erob_Encoder1 					12
#define	Number_Pulse2 					13
#define	Pulse_Enable2 					14
#define	Erob_mode2 						15
#define	Erob_start2 					16
#define	Erob_Center2 					17
#define	Erob_stop2 						18
#define	Erob_Encoder2 					19
#define IP_rst 							20
#define	IP_Range 						21
#define	ATT_DATA1 						22
#define	ATT_DATA2 						23
#define	ATT_DATA3 						24
#define	ATT_DATA4 						25
#define	ATT_DATA5 						26
#define	ATT_DATA6 						27
#define	ATT_DATA7 						28
#define	ATT_DATA8 						29
#define	ATT_DATA9 						30
#define	ATT_DATA10 						31
#define	ATT_DATA11 						32
#define	ATT_DATA12 						33
#define	ATT_DATA13 						34
#define	ATT_DATA14 						35
#define	ATT_DATA15 						36
#define	ATT_DATA16 						37
#define	ATT_DATA17 						38
#define	Trigger_time1 					39
#define	Trigger_time2 					40
#define	ADC_Sel 						41
#define	RS485_DIR1 						42
#define	RS485_DIR2 						43
#define	RF_ON_OFF 						44
#define	Beta_in 						45
#define	Beta_start 						46
#define	Beta_stop 						47
#define	Beta_mode 						48 // 0: quay tron; 1 quet quat
#define	dmt 							49
#define	thang_culy 						50
#define	ATT_Dmax 						51
#define	IP_SW 							52
#define	Trigger_Mode 					53
#define	Trigger_Width 					54
#define	Trigger_Max 					55
#define	IQ_Channel_View2 				56
#define	TX_SEL 							57
#define	IP_code 						58
#define	RDY_EN 							59
#define	IQ_Channel_View1 				60
#define	FIFO_EN 						61
#define	ADC_View_Channel 				62
#define	Sinphase1 						63
#define	Cosphase1 						64
#define	Sinphase10						65
#define	Cosphase10 						66
#define	Sinphase11 						67
#define	Cosphase11 						68
#define	Sinphase12 						69
#define	Cosphase12 						70
#define	Sinphase13 						71
#define	Cosphase13 						72
#define	Sinphase14 						73
#define	Cosphase14 						74
#define	Sinphase15 						75
#define	Cosphase15 						76
#define	Sinphase16 						77
#define	Cosphase16 						78
#define	Sinphase2 						79
#define	Cosphase2 						80
#define	Sinphase3 						81
#define	Cosphase3 						82
#define	Sinphase4 						83
#define	Cosphase4 						84
#define	Sinphase5 						85
#define	Cosphase5 						86
#define	Sinphase6 						87
#define	Cosphase6 						88
#define	Sinphase7 						89
#define	Cosphase7 						90
#define	Sinphase8 						91
#define	Cosphase8 						92
#define	Sinphase9 						93
#define	Cosphase9 						94
#define	Phase_Start_Read 				95
#define	Beta_Epxilon_sel 				96
#define	BSS_Enable 						97
#define	BSS_Angle						98
#define	BSS_Start 						99
#define	BSS_Stop						100
#define	BSS_Channel 					101
#define	Beam_Channel 					102
#define	fb_Channel 						103
#define	BS_Sel 							104
#define	BS_Size 						105
#define	Beam_Sel 						106
#define	BSS_Start4 						107
#define	BSS_Stop4						108
#define	BS_Size4 						109
#define	Beam_start 						110
#define	Beam_stop 						111
#define	Beam_size 						112
#define	Beam_Max_Cell					113
#define Mixer_View						114
#define IP_Mixer_Sel					115
#define Scope_Delay						116
#define IP_target_Mode					117
#define IP_B17_Sel						118
#define IP_Bx_Sel						119
#define IP_code_Sellect					120
#define RF_ON_OFF2						121
#define	ADC_Sel2 						122
#define _FFT_Sel						123
#define _Sum_Sel						124

#define cosdac1							125 // 255
#define cosdac10						126 // 256
#define cosdac11						127 // 257
#define cosdac12						128 // 258
#define cosdac13						129 // 259
#define cosdac14						130 // 260
#define cosdac15						131 // 261
#define cosdac16						132 // 262
#define cosdac2							133 // 263
#define cosdac3							134 // 264
#define cosdac4							135 // 265
#define cosdac5							136 // 266
#define cosdac6							137 // 267
#define cosdac7							138 // 268
#define cosdac8							139 // 269
#define cosdac9							140 // 270
#define sindac1							141 // 272
#define sindac10						142 // 273
#define sindac11						143 // 274
#define sindac12						144 // 275
#define sindac13						145 // 276
#define sindac14						146 // 277
#define sindac15						147 // 278
#define sindac16						148 // 279
#define sindac2							149 // 280
#define sindac3							150 // 281
#define sindac4							151 // 282
#define sindac5							152 // 283
#define sindac6							153 // 284
#define sindac7							154 // 285
#define sindac8							155 // 286
#define sindac9							156 // 287
#define dmt1							157 // 287
#define IP_amplitude					158 // 287

#define	_Input_Pulse_Status				0
#define	_Input_Phi						1
#define	_Input_Epxilon					2
#define	_Input_trigger_sel				3
