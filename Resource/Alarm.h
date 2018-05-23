/**
  ******************************************************************************
  * @file    /PH.h 
  * @author  MMY
  * @version V0.0.1
  * @date    12-9-2016
  * @brief   a package of PH function
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2016 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	//#include "stm32f4xx.h"

#ifndef  __ALARM_H
#define  __ALARM_H

#define ALARM_NUM 3
#include "cdv_include.h"

/*±¨¾¯²éÑ¯*/
void AlarmQuery11(u8 ID);
//void AlarmCancel(u8 num);
void AlarmCancel(u8 no);

int FlashToAlarmStruct(void);
void AlarmQuery(u8* rxBuf,u8 rxLen,CMD_ARG *arg);
typedef struct 
{
	char cAlarmNo;
	char cValNo;
	int iValMax;
	int iValMin;
	int iAlarmGrade;
	
} AlarmInfo;

extern int nAlarmCount;
extern AlarmInfo strAlarmInfo [ALARM_NUM];



#endif





