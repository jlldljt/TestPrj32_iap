#include "dac.h"
 	
DAC_STRUCT dac_par;//dac�����ṹ

CDV_INT32S  transducer1_speed;//��Ƶ1��ת��
CDV_INT32S  transducer1_time;//��Ƶ1������ʱ��
float transducer1_vol;//��Ƶ��1ת�ٶ�Ӧ��ѹֵ

u16 transducer2_speed;//��Ƶ2��ת��
u8 transducer2_time;//��Ƶ2������ʱ��
float transducer2_vol;//��Ƶ��2ת�ٶ�Ӧ��ѹֵ
//DACͨ��1�����ʼ��
void Dac1_Init(void)
{  
  GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //ʹ��GPIOAʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE); //ʹ��DACʱ��
	   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN; //ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
  GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��

	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //��ʼ��DACͨ��1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //ʹ��DACͨ��1
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
}
//����ͨ��1�����ѹ
//vol:0~3300,����0~3.3V
void Dac1_Set_Vol(u16 vol)
{
//	double temp=vol;
//	temp/=1000;
//	temp=temp*4095/3.3;
//	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
	if(vol>4095)vol=4095;
	DAC_SetChannel1Data(DAC_Align_12b_R,vol);//12λ�Ҷ������ݸ�ʽ����DACֵ
}

//DACͨ��2�����ʼ��
void Dac2_Init(void)
{  
  GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//ʹ��DACʱ��
	   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��

	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
  DAC_Init(DAC_Channel_2,&DAC_InitType);	 //��ʼ��DACͨ��2

	DAC_Cmd(DAC_Channel_2, ENABLE);  //ʹ��DACͨ��2
  
  DAC_SetChannel2Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
}
//����ͨ��2�����ѹ
//vol:0~3300,����0~3.3V
void Dac2_Set_Vol(u16 vol)
{
	double temp=vol;
	if(temp > 3300 )temp = 3300;
	temp/=1000;
	temp=temp*4095/3.3;
	DAC_SetChannel2Data(DAC_Align_12b_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
}

//*******************************************
//CDV dac ����
//start_val����ʼ��ѹ mv
//end_val��Ŀ���ѹ mv
//run_time :����ʱ��ms
//*******************************************
RET_STATUS CDV_DacRun(u32 start_val,u32 end_val,u32 run_time)
{
	u8 flag;
	u32 i;
	float val;//��ֵ
	u32 dotimes;
	float pval;
	float run_val=0;
	if(end_val>start_val)
	{
		val=end_val-start_val;
		flag=1;
	}	
	else
	{
		val=start_val-end_val;
		flag=0;
	}	
	dotimes=run_time/100;//ÿ100msһ��
	pval=val/dotimes;
	run_val=start_val+pval;
	for(i=0;i<dotimes;i++)
	{
		Dac1_Set_Vol(run_val);
		delay_ms(100);
		if(flag==1)
		{	
			run_val=run_val+pval;
		}
    else
    {
			run_val=run_val+pval;
		} 			
	}
	return OPT_SUCCESS;
}

//*******************************************
//CDV dac ֹͣ
//*******************************************
RET_STATUS CDV_DacStop()
{
	return OPT_SUCCESS;
}

//*******************************************
//CDV dac �ȴ�
//*******************************************
RET_STATUS CDV_DacWait()
{
	return OPT_SUCCESS;
}

//*******************************************
//CDV dac
//
//
//
//
//*******************************************
RET_STATUS DacCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen,CMD_ARG *arg)
{
	u8 i=0 , j=0;
	RET_STATUS  ret =OPT_SUCCESS;
	CDV_INT08U  opt , type ;  //������ ����������
	CDV_INT32U  no ;    //��Դ��
	CDV_INT32U  pflag;   //�������
	u8  Para_len;   //��ȡ��������
	u8  Read_Par_Buf[48];   // ��ȡ��������
  CDV_INT32U  var_num;  //�������
	u32  var_val;   //�����ֵ
	u8  da_num;   //da��
	u16  transducer_vol;  //��Ƶ��ת�ٶ�Ӧ��ѹֵ
	
	no  = *(CDV_INT32U*)(rxBuf);    
	opt = rxBuf[4];
	type = rxBuf[5];
  da_num = rxBuf[0];
	pflag = (rxBuf[9]<<24)+(rxBuf[8]<<16)+(rxBuf[7]<<8)+rxBuf[6];
//	pflag = *(CDV_INT32U*)&rxBuf[6];
	
	
//	no   = rxBuf[0];
//	opt  = rxBuf[1];
//	type = rxBuf[2];
//	pflag= (rxBuf[3]<<8)+rxBuf[4];
//  da_num = rxBuf[0];
	switch(opt) 
	{
		//��������
		case 0x00:		
                if((type==0xFF)||(type==0x2F))
								{	
										if(pflag & 0x00000004)//б��ֵ
										{	
											dac_par.Slope[no]=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}	
										if(pflag & 0x00000008)//��λֵ
										{	
											dac_par.Zero[no]=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}	
										if(pflag & 0x00000010)//�����Сֵ
										{	
											dac_par.Out_Min[no]=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}	
										if(pflag & 0x00000020)//������ֵ
										{	
											dac_par.Out_Max[no]=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}
										if(pflag & 0x00000040)//PID��������P
										{	
											dac_par.PID_proportion[no]=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];											
											i=i+4;
										}
										if(pflag & 0x00000080)//PID���ֲ���I
										{	
											dac_par.PID_integral[no]=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}
										if(pflag & 0x00000100)//PID΢�ֲ���D
										{	
											dac_par.PID_Derivative[no]=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}
										if(pflag & 0x00000200)//PID���Ƽ��
										{
											dac_par.PID_Ctrl_Time[no]=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}										
										if(pflag & 0x00000400)//PID��������
										{
											dac_par.PID_Integral_Limit[no]=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i];
											i=i+4;
										}
										PIDInit_Valve();
								}		
							 else if((type==0xFE)||(type==0x2E))//�����������
							 {
								    var_num=(rxBuf[13+j]<<24 )+(rxBuf[12+j]<<16)+(rxBuf[11+j]<<8)+rxBuf[10+j]; //ȡ������
										var_val = VarGet(var_num);//ȡ����ֵ
								    if(pflag & 0x00000004)//б��ֵ
										{	
											dac_par.Slope[no]=var_val;
										}	
										if(pflag & 0x00000008)//��λֵ
										{	
											dac_par.Zero[no]=var_val;
										}	
										if(pflag & 0x00000010)//�����Сֵ
										{	
											dac_par.Out_Min[no]=var_val;
										}	
										if(pflag & 0x00000020)//������ֵ
										{	
											dac_par.Out_Max[no]=var_val;
										}
										if(pflag & 0x00000040)//PID��������P
										{	
											dac_par.PID_proportion[no]=var_val;
										}
										if(pflag & 0x00000080)//PID���ֲ���I
										{	
											dac_par.PID_integral[no]=var_val;
										}
										if(pflag & 0x00000100)//PID΢�ֲ���D
										{	
											dac_par.PID_Derivative[no]=var_val;
										}
										if(pflag & 0x00000200)//PID���Ƽ��
										{
											dac_par.PID_Ctrl_Time[no]=var_val;
											//i=i+4;		
										}											
										if(pflag & 0x00000400)//PID��������
										{
											dac_par.PID_Integral_Limit[no]=var_val;
											//i=i+4;
										}
										PIDInit_Valve();
							 } 	 
							if((type==0x2F) || (type==0x2E))//�����������޸Ĳ������������ֵ��flash
	            {								 
								SPI_Flash_Write((CDV_INT08U *)&dac_par, DAC_PAR_ADDR, sizeof(dac_par)); //д��flash
							}								
							break; 			
//			        if(da_num==0)
//							{	
//								PressureAD_Zero	=	(rxBuf[5+i]<<24)+(rxBuf[6+i]<<16)+(rxBuf[7+i]<<8)+rxBuf[8+i];
//							}
//              else if(da_num==1)
//              {
//								AD_COEFFICIENT	=	((rxBuf[5+i]<<24)+(rxBuf[6+i]<<16)+(rxBuf[7+i]<<8)+rxBuf[8+i])/10000;
//							}			
//		          break;
		//������ȡ
		case 0x01:
					if((type==0xFF)||(type==0x2F))
						{	
							Para_len=Read_Dac_Par(Read_Par_Buf,no,pflag);
							ResRequest(arg->buf, arg->len, Read_Par_Buf,Para_len,arg);
						}	
		        else if((type==0xFE)||(type==0x2E))//������������
						{	
							var_val = 0;
							Para_len=Read_Dac_Par(Read_Par_Buf,no,pflag);
							for(i=0;i<Para_len;i++)
							{
								var_val=var_val+(Read_Par_Buf[i]<<(8*(Para_len-i-1)));//buf��4Byte��һ��ֻ��һ��������ÿ����������ȡ4���ֽ���ֵ
							}
							ValSet(rxBuf[10],var_val);
							
						}
		        break;
		//����	     
		case 0x02:
			        if(no==0)//����������
							{	
									if(type==0xff)
									{	
										if(pflag & 0x00000001)//���ֵ
										{	
											Struct_Valve.Ptarget=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i]; //��ȡѹ��ֵ
											if(Struct_Valve.Ptarget>280)Struct_Valve.Ptarget = 280;
											i=i+4;
										}	
										if(pflag & 0x00000002)//����ʱ��
										{	
											Struct_Valve.Inflating_Times=(rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i]; //��ȡ����ʱ�䣬��λS
											i=i+4;
										}
										ProportionalValve_Init();
								 }
								 else if(type==0xfe)	
								 {
										if(pflag & 0x00000001)//���ֵ
										{	
											Struct_Valve.Ptarget=VarGet((rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i]);
											//Struct_Valve.Ptarget_Rel = (Dish_Weight-Struct_Valve.Ptarget)*1.2522;
											Struct_Valve.Ptarget_Rel = (242-Struct_Valve.Ptarget)*1.2522;
											i=i+4;
										}	
										if(pflag & 0x00000002)//����ʱ��
										{	
											Struct_Valve.Inflating_Times=VarGet((rxBuf[13+i]<<24)+(rxBuf[12+i]<<16)+(rxBuf[11+i]<<8)+rxBuf[10+i]);
											i=i+4;
										}
										ProportionalValve_Init();
								 }
								 else
								 {
									 return ret;
								 }	
						 }
             else  if(no==1)//DAC2��Ƶ��1����
						 {
								if(type==0xff)
								{	
									if(pflag & 0x00000001)//���ת��
									{	
//										transducer1_speed=(rxBuf[7]<<8)+rxBuf[8];
//										transducer1_time=rxBuf[12];
										transducer1_speed=(rxBuf[13]<<24)+(rxBuf[12]<<16)+(rxBuf[11]<<8)+rxBuf[10];
										transducer1_time=(rxBuf[17]<<24)+(rxBuf[16]<<16)+(rxBuf[15]<<8)+rxBuf[14];
										if(transducer1_speed>TRA1_SPEED_LIMIT)transducer1_speed = TRA1_SPEED_LIMIT;
										
									}	
										
								 }
								 else if(type==0xfe)	
								 {
										if(pflag & 0x00000001)//���ֵ
										{	
//											transducer1_speed=VarGet(rxBuf[5]);
//											transducer1_time=VarGet(rxBuf[6]);
											transducer1_speed=VarGet((rxBuf[13]<<24)+(rxBuf[12]<<16)+(rxBuf[11]<<8)+rxBuf[10]);
											transducer1_time=VarGet((rxBuf[17]<<24)+(rxBuf[16]<<16)+(rxBuf[15]<<8)+rxBuf[14]);
											if(transducer1_speed>TRA1_SPEED_LIMIT)transducer1_speed = TRA1_SPEED_LIMIT; 
										}	
										
								 }
								 else
								 {
									 return ret;
								 }
								 transducer1_vol=(float)transducer1_speed/33*1000;//10ת/V
								 transducer1_vol=transducer1_vol-(float)dac_par.Zero[no];
								 transducer1_vol=transducer1_vol*((float)dac_par.Slope[no]/10000);//*��б��

								 Dac2_Set_Vol(transducer1_vol);
						 }	
             else if(no==2)//DAC3��Ƶ��2����
						 {
								if(type==0xff)
								{	
									if(pflag & 0x00000001)//���ת��
									{	
//										transducer2_speed=(rxBuf[7]<<8)+rxBuf[8];
//										transducer2_time=rxBuf[12];
										transducer2_speed=(rxBuf[13]<<24)+(rxBuf[12]<<16)+(rxBuf[11]<<8)+rxBuf[10];
										transducer2_time=(rxBuf[17]<<24)+(rxBuf[16]<<16)+(rxBuf[15]<<8)+rxBuf[14];
										if(transducer2_speed>TRA2_SPEED_LIMIT) transducer2_speed = TRA2_SPEED_LIMIT;
										
									}	
										
								 }
								 else if(type==0xfe)	
								 {
										if(pflag & 0x00000001)//���ֵ
										{	
//											transducer2_speed=VarGet(rxBuf[5]);
//											transducer2_time=VarGet(rxBuf[6]);
											transducer2_speed=VarGet((rxBuf[13]<<24)+(rxBuf[12]<<16)+(rxBuf[11]<<8)+rxBuf[10]);
											transducer2_time=VarGet((rxBuf[17]<<24)+(rxBuf[16]<<16)+(rxBuf[15]<<8)+rxBuf[14]);
											if(transducer2_speed>TRA2_SPEED_LIMIT) transducer2_speed = TRA2_SPEED_LIMIT; 
										}	
										
								 }
								 else
								 {
									 return ret;
								 }
								 //transducer_vol=transducer_speed/33*1000; //10ת/V
								 transducer2_vol=(float)transducer2_speed/33*1000; //10ת/V��DA����Ŵ�3.3����3.3*10=33����ѹ��λmv
								 transducer2_vol=transducer2_vol-(float)dac_par.Zero[no];
								 transducer2_vol=transducer2_vol*((float)dac_par.Slope[no]/10000); //*��б��
								 
								 WriteToDAC7512(transducer2_vol);
								 // Dac2_Set_Vol(transducer_vol);
						 }														

		      break;
		//ֹͣ
		case 0x03:
			   Process_Start =0; 				
				 PressureAD_Zero = PressureAD_Zero_1;	//ѹ����λ
				 AD_COEFFICIENT=AD_COEFFICIENT_1;
				 R_PressureAD_Zero = R_PressureAD_Zero_1;
				 R_AD_COEFFICIENT = R_AD_COEFFICIENT_1;
		     break;
		//�ȴ�
		case 0x04:
			        if(Run_Stat==1)
							{
								ret=OPT_SUCCESS;
							}	
							else
							{
								ret=OPT_FAILURE;
								//delay_ms(1);
							}	
		     break;
		default:
			    break;
		
	}	
	return ret;
	
}
//****************************************************
//��ȡ����
//no:�豸��
//pflag:�������
//buf:��������
//****************************************************
u8 Read_Dac_Par(u8 *buf,u8 no,u32 pflag)
{
	u8 i,j;
	u32 FPGA_POS;
	u16 FPGA_POSH;
	u16 FPGA_POSL;
	i=0;
	 if(pflag & 0x00000004)//б��ֵ
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=dac_par.Slope[no]>>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x00000008)//��λֵ
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=dac_par.Zero[no]>>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x00000010)//�����Сֵ
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=dac_par.Out_Min[no]>>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x00000020)//������ֵ
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=dac_par.Out_Max[no]>>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x00000040)//PID��������P
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=dac_par.PID_proportion[no] >>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x00000080)//PID���ֲ���I    
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=dac_par.PID_integral[no] >>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x00000100)//PID΢�ֲ���D     
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=dac_par.PID_Derivative[no] >>(24-8*j);
			}
			i=i+4;
	 }
	 if(pflag & 0x00000200)//PID���Ƽ��     
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=dac_par.PID_Ctrl_Time[no] >>(24-8*j);
			}
			i=i+4;
	 }	 
	 if(pflag & 0x00000400)//PID��������
	 {
			for(j=0;j<4;j++)
			{
				*(buf+i+j)=dac_par.PID_Integral_Limit[no] >>(24-8*j);
			}
			i=i+4;
	 }	 	 
	return i;//���س���
	 
}	
//RET_STATUS DacCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen,u8 uartNo)
//{
//	u8 i=0;
//	RET_STATUS ret =OPT_SUCCESS;
//	CDV_INT08U no , opt , type; //��Դ�ţ������룬��������
//	u16 pflag;//�������
//	u32 end_val=0;//�����������
//	u32 run_time=0;//����ʱ��
//	float current_val;
//	float pre_val;//ѹ��ֵKp
//	float get_pre_val;//��ȡ��ѹ��ֵKp
//	no   = rxBuf[0];
//	opt  = rxBuf[1];
//	type = rxBuf[2];
//	pflag= (rxBuf[3]<<8)+rxBuf[4];
//  
//	switch(opt) 
//	{
//		//��������
//		case 0x00:
//			    
//		       break;
//		//������ȡ
//		case 0x01:
//			 
//		       break;
//		//����	     
//		case 0x02:
//			        if(type==0xff)
//							{	
//								if(pflag & 0x0001)//���ֵ
//								{	
//									pre_val=(rxBuf[5+i]<<24)+(rxBuf[6+i]<<16)+(rxBuf[7+i]<<8)+rxBuf[8+i];
//									end_val=pre_val/50*1000;//mv
//									i=i+4;
//								}	
//								if(pflag & 0x0002)//����ʱ��
//								{	
//									run_time=(rxBuf[5+i]<<24)+(rxBuf[6+i]<<16)+(rxBuf[7+i]<<8)+rxBuf[8+i];
//									i=i+4;
//								}
//						 }
//						 else if(type==0xfe)	
//						 {
//								if(pflag & 0x0001)//���ֵ
//								{	
//									end_val=DW_CDV_VAR(rxBuf[5+i]);
//									i=i+1;
//								}	
//								if(pflag & 0x0002)//����ʱ��
//								{	
//									run_time=DW_CDV_VAR(rxBuf[5+i]);
//									i=i+1;
//								}
//						 }
//             else
//						 {
//							 return ret;
//						 }	
//             current_val=Get_Adc_Average(ADC_Channel_0,20);
//						 current_val=current_val/4095*3.3;
//						 if(current_val<1)
//						 {
//								current_val=0;
//						 }	
//             else
//             {							 
//							  current_val=(current_val-1);
//						 }	 
//						 get_pre_val=current_val*125;
//						 current_val=get_pre_val/50*1000;//mv
//						 //current_val=current_val*(3.3/4095)*1000;
//						 CDV_DacRun((u32)current_val,end_val,run_time);						 
//		      break;
//		//ֹͣ
//		case 0x03:
//			
//		     break;
//		//�ȴ�
//		case 0x04:
//			
//		     break;
//		default:
//			    break;
//		
//	}	
//	return ret;
//	
//}














































