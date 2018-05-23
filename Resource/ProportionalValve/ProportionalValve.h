#ifndef __PROPORTIONALVALVE_H	
#define __PROPORTIONALVALVE_H	 
#include "cdv_include.h" 
#define RX_ADC1_DMA 200 //������1000
#define ITV2050 150     //�������ͺ�2050��Ӧ270���������ͺ�2030��Ӧ150��
#define fen_ya_val 1.606 //�������ķ�ѹ����ϵ�� 5.3k��/3.3K��=1.606�����û�ӷ�ѹ�������ó�1
typedef struct {
	CDV_FP64  SetPoint;          
	//  �趨Ŀ�� Desired Value      
	CDV_FP64  LastSetPoint; 
	//  ��һ�ε����趨��Value  
	CDV_FP64  Proportion;         
	//  �������� Proportional Const   
	CDV_FP64  Integral;         
  //  ���ֳ��� Integral Const       
  CDV_FP64  Derivative;        
	//  ΢�ֳ��� Derivative Const  
	CDV_FP64  LastError;          //  Error[-1]  
	CDV_FP64  PrevError;          //  Error[-2]  
	CDV_FP64  SumError;           //  Sums of Errors
	CDV_FP64  Integral_Limit;     //  ��������
	
 	CDV_INT32U  Ctrl_Time;     // ���Ƽ�� ��λms
 }PID;


typedef struct 
{
	CDV_INT16U Inflating_Times;	//��ѹʱ��
	CDV_INT16U Ptarget;//Ŀ����ѹ	Kpa
	CDV_INT16U Ptarget_Rel;//Ŀ��ѹ�� KG	
}ProportionalValve;
void LcdSet_UI(ProportionalValve* ProportionalValve_Struct);
void LcdRun_UI(void);
void ProportionalValve_Init(void);
void RunProcess(ProportionalValve* ProportionalValve_Struct);
//void Down_Slow(void);
//void Up_Slow(void);
u16 Ptarget_Offset(u16 Target);
extern ProportionalValve Struct_Valve;
extern double Present_Valve_DA  ;
extern float Present_Pressure_AD  ;
extern u16 R_Pressure_AD;
extern double R_Pressure;	
extern u8  ADSigPre_Flag,Lcd_Refresh_Flag;
extern u16 PressureValveDA_Zero ;
extern u8 Process_Start,Process_Exit;
extern u8 Sequence;
extern double LastPressure_Sys;
extern double Pressure_Coefficient;
extern u8 Run_Stat;
extern CDV_INT32U Run_time;
extern u8 ValveDAadjust_Flag; //50ms
extern u16 Ptarget_Num;
extern CDV_INT16U ADC_ConvertedValue[RX_ADC1_DMA];
extern PID Valve_Pid;
extern u16 Dish_Weight;
void PIDInit_Valve(void);
void PIDClear_Valve(void);
extern double Present_Pressure_Rel;
extern u16 PressureAD_Zero,PressureAD_Zero_1,PressureAD_Zero_2;
extern double AD_COEFFICIENT,AD_COEFFICIENT_1,AD_COEFFICIENT_2;
extern u16 R_PressureAD_Zero,R_PressureAD_Zero_1,R_PressureAD_Zero_2;
extern double R_AD_COEFFICIENT,R_AD_COEFFICIENT_1,R_AD_COEFFICIENT_2;
CDV_FP64 PIDcal(PID *pp,CDV_FP64 PresPoint,CDV_FP64 SetPoint);
void R_Pressure_Measure(void);
void Valve_Debug(void);
void Valve_PSensor_Coe_Init(void);
void Valve_PSensor_Coe_Switchover(void);
#endif
