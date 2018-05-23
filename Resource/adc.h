#ifndef __ADC_H
#define __ADC_H	
#include "cdv_include.h"  
//#include "sys.h" 
#define BUFFX_LEN 500
#define BUFFV_LEN 500

#define adc2_value_lens   300
#define adc3_value_lens   100


typedef struct 
{
	CDV_INT32U Slope[3];//斜率值
	CDV_INT32U Zero[3];//零位值
	CDV_INT32U Out_Min[3];//输出最小值
	CDV_INT32U Out_Max[3];//输出最大值
	CDV_INT32U Slope2[3];//临时斜率值
	CDV_INT32U Zero2[3];//临时零位值	
}ADC_STRUCT;
extern ADC_STRUCT adc_par;



extern u16 adc2_value[adc2_value_lens];
extern u16 adc3_value[adc3_value_lens];
 							   
void Adc_Init(void); 				//ADC通道初始化
u16  Get_Adc(u8 ch); 				//获得某个通道值 
u16 Get_Adc_Average(u8 ch,u8 times);//得到某个通道给定次数采样的平均值  

void Adc2_Init(void); 				//ADC通道初始化
void Adc2_Dma_Init(void);
u16  Get_Adc2(u8 ch); 				//获得某个通道值 
u16 Get_Adc2_Average(u8 ch,u8 times);//得到某个通道给定次数采样的平均值 

void Adc3_Dma_Init(void);
void Adc3_Init(void); 				//ADC3通道初始化	



u16 Grubbs(u16 x[],u16 n);
float AVRG(u16 x[],u16 n);
u32 SUM(u16 x[],u16 n);
void ADC1_DMA_Init(void);
void Adc2_Dma_Init2(void);
u16 Get_VFD_Speed(u16 *buf,u16 lens);
u8 Read_Adc_Par(u8 *buf,u8 no,u32 pflag);//ADC参数读取
RET_STATUS AdcCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg);
#endif 















