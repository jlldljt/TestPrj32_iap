/**
  ******************************************************************************
  * @file    /PH.c 
  * @author  MMY
  * @version V0.0.1
  * @date    12-9-2016
  * @brief   a package of PH function
  * 
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2016 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */

#include "Alarm.h"
AlarmInfo strAlarmInfo [ALARM_NUM];
int nAlarmCount;


/**
  * @brief  读取PH信息代号
  *  
  * @param  ID     要操作的仪设备号
  *   
  * @retval void
  *
  * @note   查询间隔不能小于500ms（检测仪规定的）
  */
void AlarmQuery11(u8 ID)
{
	CDV_INT32S val = 0;
	u16 data;
	u8 send_buf[16]={0x02,0x30,0x00,0x00,0x00,0x00,0x41,0xFD};
	u8 recv_buf[16] = {0};
	u8 recv_len=0;
	
	if((ID>32)||(ID<1))
		send_buf[2]=0x01;
	else
	  send_buf[2]=ID;
	
	data=getCRC16(send_buf,6);
	send_buf[6]=data & 0x00ff;
  send_buf[7]=(data >> 8) & 0x00ff;
  //UART4_Send(send_buf,8);
	//ResParaRequest( rxBuf ,rxLen , send_buf, 8, CDV_INT08U uartNo);
	delay_ms(100);
	//RS485_Receive_Data(recv_buf,&recv_len);
	data=getCRC16(recv_buf,recv_len-2);
	if((recv_buf[recv_len-2]==(data & 0x00ff)) && (recv_buf[recv_len-1]==((data >> 8) & 0x00ff)))
	{
	  val=(recv_buf[3]<<8)+recv_buf[4];
	}
	//delay_ms(300);
//	return val;
}





int FlashToAlarmStruct(void)
{
	int i;
	for(  i = 0; i < ALARM_NUM; i++)
	{
		memset(&strAlarmInfo[i], -1, sizeof(AlarmInfo));
		
	}
	SPI_Flash_Read((u8*)strAlarmInfo,SCRIP_ALARM,sizeof(AlarmInfo)*ALARM_NUM);
	
	strAlarmInfo[0].cAlarmNo = 0x53;
	strAlarmInfo[0].cValNo = 0x01;
	strAlarmInfo[0].iValMax = 6;
	strAlarmInfo[0].iValMin =  -2;
	
	strAlarmInfo[1].cAlarmNo = 0x54;
	strAlarmInfo[1].cValNo = 0x05;
	strAlarmInfo[1].iValMax = 256;
	strAlarmInfo[1].iValMin =  10;
	
	nAlarmCount = 2;
	return 0;
}


/***************报警处理*****************/
void AlarmCancel(u8 no)
{ 
	if(no == 0xFF)
		
	  WorkerControl(no,	WORKER_EXIT);  
	else
		WorkerControl(no , WORKER_ONCE);
	
}


/***************报警查询*****************/
void AlarmQuery(u8* rxBuf,u8 rxLen,CMD_ARG *arg)
{
	int i = 0;
	int Para_len = 0;
	int cValNo;
	u8 Para_Buf[ALARM_NUM];
	for( i = 0; i < nAlarmCount; i++)
	{
		 cValNo = strAlarmInfo[i].cValNo;
		if (cValNo>=0 && cValNo<= 255)
		{
			int iVal = VarGet(cValNo);
			int iValMax = strAlarmInfo[i].iValMax;
			int iValMin = strAlarmInfo[i].iValMin;
			if (iVal < iValMin || iVal > iValMax)
				{
					
					char cAlarmNo = strAlarmInfo[i].cAlarmNo;
					Para_Buf[Para_len] = cAlarmNo;
					Para_len++;
				
					
				}
		}
		else
		{
			break;
		}
	}
	ResRequest(arg->buf, arg->len, Para_Buf, Para_len, arg);
	
			
}

//delay_ms(300);