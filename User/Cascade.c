/**
  ******************************************************************************
  * @file    /Cascade.c 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-3-29
  * @brief   级联
  * 
@verbatim  

@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	
	#include "Cascade.h"
	
  //开启主级联口
	#define	CASCADE_EN 			CASCADE_CS_EN;FPGA1_CS_DS;FPGA2_CS_DS;FLASH_CS_DS;
	//关闭主级联口
	#define	CASCADE_DS 			CASCADE_CS_DS;
	//主级联口发数据
	#define CASCADE_ReadWriteByte(a)				SPIx_ReadWriteByte(SPI2,(a))

  //级联资源表——版本号
	static char version[] = {3/*软件大版本*/, 2/*硬件大版本*/, 1/*硬件驱动版本*/, 5/*迭代小版本*/};
	//从机资源表
	CDV_INT08U *slaveTable = NULL;
	CDV_INT08U slaveTableLen = 0;
	
  OLCMD_CACHE g_portCmdCache = {NULL , 0  , 0 , 0};
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	/////////////Cascade Master Function///////////////////
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	
/** @brief  查询有没有从机
  * @param  
  * @retval 存在1，不存在0
  * @note   
  */
CDV_INT08U Cascade_HaveSlaveTable(void)   
{ 
  if (slaveTableLen > 5)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}  

/** @brief  查询从机回复（等待）
  * @param  ppBuffer 查询成功保存从机回复的字符串
	*         pLen    查询成功保存从机回复的字符串长度
	*         cnt     查询失败的循环次数
  * @retval 返回值说明
  * @note   使用上应该在Cascade_Master_Write之后查询
	          请勿随意调用
  */
void Cascade_Master_Query(CDV_INT08U** ppBuffer, CDV_INT08U* pLen, CDV_INT16U cnt)   
{ 
 	CDV_INT16U i = 0;    		
	CDV_INT08U len;
  OS_ERR err;	
	
	ASSERT(ppBuffer);
	ASSERT(pLen);
	

	CASCADE_EN;                            //使能器件  
	//DelayUS(1000);
	do{
		DelayUS(100);
		*pLen = CASCADE_ReadWriteByte(0xFF);   //循环读数  
		cnt--;
//		i++;
//		
//		if(i > 10 && 0x00 == *pLen)
//			break;
	} while((/*0xFF == *pLen ||*/ 0x00 == *pLen) && cnt);
	
	if(0x80 > *pLen && 0x00 != *pLen) {
		NEW08U(*ppBuffer, *pLen);
		
		for(i = 0; i < *pLen; i++)
		{
				(*ppBuffer)[i] = CASCADE_ReadWriteByte(0xFF);   //循环读数  
		}
	}
	else
	{
		*pLen = 0x00;
	}
	CASCADE_DS;                            //取消片选   

}  

/** @brief  查询从机回复（不等待）
  * @param  pBuffer          查询到的字符串
	*         NumByteToRead    查询的长度
  * @retval 返回值说明
  * @note   请勿随意调用
  */
void Cascade_Master_Read(CDV_INT08U* pBuffer, CDV_INT16U NumByteToRead)   
{ 
 	CDV_INT16U i;    		
  OS_ERR err;	
	
	ASSERT(pBuffer);
	

	CASCADE_EN;                            //使能器件  
	
	for(i = 0; i < NumByteToRead; i++)
	{
			pBuffer[i] = CASCADE_ReadWriteByte(0XFF);   //循环读数  
	}
	CASCADE_DS;                            //取消片选   

}  
/** @brief  主机发送命令
  * @param  pBuffer          发送的字符串
	*         NumByteToWrite   发送的字符串长度
  * @retval 返回值说明
  * @note   请勿随意调用
  */
void Cascade_Master_Write(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite)   
{
	//CDV_INT08U* temp = NULL;
 	CDV_INT16U i;    		
  OS_ERR err;	
	//NEW08U(temp, NumByteToWrite);
	
	ASSERT(pBuffer);
	

	CASCADE_EN;                            //使能器件  
	//DelayUS(1000);
	for(i = 0 ; i < NumByteToWrite; i++)
	{
			/*temp[i] = */CASCADE_ReadWriteByte(pBuffer[i]);   //循环读数  
	}
	CASCADE_DS;                            //取消片选   

	//DELETE(temp);
}

/** @brief  主机发送接收接口，用于加入信号量
  * @param  pTxBuf          发送的字符串 NULL 不发送
	*         txByte          发送的字符串长度
  *         ppRxBuf         查询成功保存从机回复的字符串
	*         pRxLen          查询成功保存从机回复的字符串长度
	*         rxCnt           查询失败的循环次数
  * @retval 返回值说明
  * @note   外部主级联封装发送命令必须使用该接口，或者与级联总线的其他设备资源易产生冲突
  */
void CascadeMasterTR2(CDV_INT08U* pTxBuf, CDV_INT16U txByte, CDV_INT08U** ppRxBuf, CDV_INT08U* pRxLen, CDV_INT16U rxCnt)   
{
  OS_ERR err;	
	u8 i,dat;
	OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
	
	
	if(pTxBuf && txByte) {
	  Cascade_Master_Write(pTxBuf, txByte);
	}
	DelayUS(10000);//给从机的中断一点反应的时间
	if(ppRxBuf && pRxLen) {
	  Cascade_Master_Query(ppRxBuf, pRxLen, rxCnt);
	}
	
	CASCADE_EN;
	for(i = 0 ; i < 3; i++)
	  dat = CASCADE_ReadWriteByte(0xFF);
	
	CASCADE_DS;
	DelayUS(10000);//给从机的中断一点反应的时间
//	CASCADE_CS_EN;
//	
//	CASCADE_CS_DS;
	OSSemPost (&MOTO_SEM,OS_OPT_POST_1,&err);
}

/** @brief  主机发送接收接口，用于加入信号量 ，CRC校验版本
  * @param  pTxBuf          发送的字符串 NULL 不发送
	*         txByte          发送的字符串长度
  *         ppRxBuf         查询成功保存从机回复的字符串
	*         pRxLen          查询成功保存从机回复的字符串长度
	*         rxCnt           查询失败的循环次数
  * @retval 返回值说明
  * @note   外部主级联封装发送命令必须使用该接口，或者与级联总线的其他设备资源易产生冲突
  */
void CascadeMasterTR(CDV_INT08U* pTxBuf, CDV_INT16U txByte, CDV_INT08U** ppRxBuf, CDV_INT08U* pRxLen, CDV_INT16U rxCnt)   
{
  u8 i,dat;
	CDV_INT08U slen = 0;
	CDV_INT16U crc;
  CDV_INT08U* sbuf = NULL;
	OS_ERR err;	
	
	OSSemPend(&MOTO_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量

	slen = txByte + 2 ;
	NEW08U(sbuf, slen);//最后是crc
	memcpy(sbuf, pTxBuf, txByte);
	//crc
	crc = getCRC16(pTxBuf, txByte);
	sbuf[txByte] = crc & 0xff; 
	sbuf[txByte + 1] = (crc >> 8) & 0xff;
	
	
	if(sbuf && slen)
	{
	  Cascade_Master_Write(sbuf, slen);
	}
	DELETE(sbuf);
	
	do
	{
		DelayUS(10000);//给从机的中断一点反应的时间
		if(ppRxBuf && pRxLen) {
			Cascade_Master_Query(ppRxBuf, pRxLen, rxCnt);
		}
		
		//下面这个是定制的，多读几次，保证spi从机出来的是0x00
		CASCADE_EN;
		for(i = 0 ; i < 3; i++)
			dat = CASCADE_ReadWriteByte(0xFF);
		
		CASCADE_DS;
		DelayUS(10000);//给从机的中断一点反应的时间

		//crc chk
		sbuf = *ppRxBuf;
		slen = *pRxLen;
		
		if(!sbuf || 2 > slen)
			break;
		
		crc = getCRC16(sbuf, slen-2); 
		
		if((sbuf[slen-1] == ((crc >> 8) & 0xff)) && (sbuf[slen-2] == (crc & 0xff)))
		{
			*pRxLen -= 2;
			break;
		}
		else
		{
			sbuf = NULL;
			slen = 8;
			NEW08U(sbuf, slen);//最后是crc
		  memcpy(sbuf, "GETRQ:", 6);
			//crc
			crc = getCRC16(sbuf, 6);
			sbuf[6] = crc & 0xff; 
			sbuf[7] = (crc >> 8) & 0xff;
			
			if(sbuf && slen)
			{
				Cascade_Master_Write(sbuf, slen);
			}
			DELETE(sbuf);
		}
	}while(1);
	
	OSSemPost (&MOTO_SEM,OS_OPT_POST_1,&err);
}

	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////
	/////////////Cascade Slave Function////////////////////
	///////////////////////////////////////////////////////
	///////////////////////////////////////////////////////

/** @brief  从机发送命令
  * @param  pBuffer          发送的字符串
	*         NumByteToWrite   发送的字符串长度
  * @retval RET_STATUS
  * @note   非线程安全，如果多个地方用，加锁
  */
RET_STATUS Cascade_Slave_Write2(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite)   
{ 
  if(!pBuffer || !NumByteToWrite)
		return OPT_FAILURE;
	
	
	
	return SPI3_Send(pBuffer, NumByteToWrite);
}  

/** @brief  从机发送命令
  * @param  pBuffer          发送的字符串
	*         NumByteToWrite   发送的字符串长度
  * @retval RET_STATUS
  * @note   非线程安全，如果多个地方用，加锁
  */
RET_STATUS Cascade_Slave_Write(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite)   
{ 
  CDV_INT08U* tmp = NULL;
	CDV_INT16U crc;
	RET_STATUS ret;
	
	if(!pBuffer || !NumByteToWrite)
		return OPT_FAILURE;
	
	ASSERT(pBuffer);
	ASSERT(NumByteToWrite);
	
	NEW08U(tmp, NumByteToWrite + 2);//最后是crc
	memcpy(tmp, pBuffer, NumByteToWrite);
	//crc
	crc = getCRC16(pBuffer, NumByteToWrite);
	tmp[NumByteToWrite] = crc & 0xff; 
	tmp[NumByteToWrite + 1] = (crc >> 8) & 0xff;
	ret = SPI3_Send(tmp, NumByteToWrite + 2);
  DELETE(tmp);
	return ret;
}  
/** @brief  非本机命令的转发
  * @param  ppBuffer      查询到的资源表
	*         pLen          资源表长度
  *         pCmdBuf       待发命令
  *         NumByteOfCmd  待发命令长度
  * @retval RET_STATUS
  * @note   ppBuffer需使用指针
  */
	RET_STATUS CascadeTranferCmd(CDV_INT08U** ppBuffer, CDV_INT08U* pLen, CDV_INT08U* pCmdBuf, CDV_INT16U NumByteOfCmd) {
		CDV_INT08U* tmp = NULL;
		CDV_INT16U crc;
	
		if(slaveTableLen < 6)
			return OPT_FAILURE;
		
		
		ASSERT(NULL != pCmdBuf);
		ASSERT(0 != NumByteOfCmd);
		ASSERT(NULL != pLen);
//		
//		NEW08U(tmp, NumByteOfCmd + 6 + 2);//最后是crc
//		memcpy(tmp, "TFCMD:", 6);
//		memcpy(tmp + 6, pCmdBuf, NumByteOfCmd);
//		//crc
//		crc = getCRC16(pCmdBuf, NumByteOfCmd);
//		tmp[NumByteOfCmd + 6] = crc & 0xff; 
//	  tmp[NumByteOfCmd + 6 + 1] = (crc >> 8) & 0xff;
		
		
		NEW08U(tmp, NumByteOfCmd + 6);//最后是crc
		memcpy(tmp, "TFCMD:", 6);
		memcpy(tmp + 6, pCmdBuf, NumByteOfCmd);
		
		CascadeMasterTR((CDV_INT08U*)tmp, NumByteOfCmd + 6/* + 2*/, ppBuffer, pLen, 0xFFFF);
		
		DELETE(tmp);
		return OPT_SUCCESS;
	}
	
/** @brief  CascadeTranferCmd之后的解析
  * @param  pBuf   命令
	*         len    命令长度
  * @retval RET_STATUS
  * @note   
  */
  RET_STATUS TFCmdParse (CDV_INT08U* pBuf, CDV_INT08U len) {
		char *pos = NULL;
		RET_STATUS ret = OPT_FAILURE;
		
		if(!pBuf || !len)
			return ret;
		
		if(pos = strstr((char *)pBuf,"VARSET:"))
		{
			ValSet((CDV_INT08U)pos[7], *(CDV_INT32S*)(pos + 8));
			ret = OPT_SUCCESS;
		}
		else if(pos = strstr((char *)pBuf,"ERROR"))
		{
      ret = OPT_FAILURE;
		}
		else if(len <= 6)
		{
			ret = OPT_FAILURE;
		}
		else if(pBuf[1] > 0x80)
		{
			ret = OPT_FAILURE;
		}
		else
		{
			ret = OPT_SUCCESS;
		}
		return ret;
	}
	
	
	
/** @brief  判断有没有从机
  * @param  void
  * @retval void
  * @note   ppBuffer需使用指针
  */
	RET_STATUS SlaveCheck(void) {
		CDV_INT08U *buf = NULL;
	  CDV_INT08U len = 0;
		RET_STATUS ret = OPT_FAILURE;

		CascadeMasterTR((CDV_INT08U*)"EXIST:", 6, &buf, &len, 0x10);
		
		if(5 <= len && (0 == strncmp((CDV_INT08C*)buf, "EXIST", 5)))
			ret = OPT_SUCCESS;

		DELETE(buf);
    return ret;
	}
/** @brief  主机命令,从机资源表查询
  * @param  ppBuffer   查询到的资源表
	*         pLen       资源表长度
  * @retval RET_STATUS
  * @note   ppBuffer需使用指针
  */
	void CascadeCheck(CDV_INT08U** ppBuffer, CDV_INT08U* pLen, CDV_INT08U num) {
		char tmp[10]={0};
		ASSERT(NULL != pLen);
		
		sprintf(tmp, "CHECK:%02d", num);

		CascadeMasterTR((CDV_INT08U*)tmp, 8, ppBuffer, pLen, 0xffff);
	}

/** @brief  主从资源表合并,递归所有从机
  * @param  ppBuffer   查询到的资源表
	*         pLen       资源表长度
  * @retval RET_STATUS
  * @note   
  */
  void CascadeCombine(CDV_INT08U no) {
		
		CDV_INT08U *buf = NULL;
	  CDV_INT08U len = 0;

		if(OPT_SUCCESS == SlaveCheck())
		{
		  CascadeCheck(&buf, &len, no);//本机测试时注释
		}
			
		NEW08U(slaveTable, 5 + len);
		slaveTableLen = 5 + len;
		
		slaveTable[0] = no;
		memcpy(slaveTable + 1, version, 4);
		if(NULL != buf && 0 != len)
			memcpy(slaveTable + 5, buf, len);
		
		DELETE(buf);
	}
	
/** @brief  资源表,只有本机
  * @param  
  * @retval 
  * @note   
  */
  void CascadeLocal(CDV_INT08U no) {
		NEW08U(slaveTable, 5);
		slaveTableLen = 5;
		slaveTable[0] = no;
		memcpy(slaveTable + 1, version, 4);
	}
	
/** @brief  资源表发送给上位机
  * @param  
  * @retval 
  * @note   
  */
	void CascadeTableSend(CDV_INT08U* buf, CDV_INT08U len,  CMD_ARG *arg) {
		ResRequest(buf, len, slaveTable, slaveTableLen, arg);
	}
/** @brief  获取本机号
  * @param  
  * @retval 本机号。0xFF没有本机号
  * @note   
  */
	CDV_INT08U CascadeGetNativeNo(void) {
		if(NULL == slaveTable || 0 == slaveTableLen)
			return 0xFF;
		
		return slaveTable[0];
	}
	
	
	/**
  * @brief  从机命令放入cache
  *         
  *  
  * @param  rxBuf     接收命令
  *         rxLen     接收长度
  *           
  *   
  * @retval 返回值说明0失败1成功
  *
  * @note   
  */
CDV_INT08U PortCmdCache(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo) {
	OS_ERR err;
	if(rxBuf == NULL || rxLen == 0)
		return 0;
	
	while(g_portCmdCache.mark)
		return 0;
	
	g_portCmdCache.len = rxLen;
	NEW08U(g_portCmdCache.buf,g_portCmdCache.len);
	memcpy(g_portCmdCache.buf , rxBuf , g_portCmdCache.len);
	g_portCmdCache.uart = uartNo;
	g_portCmdCache.mark = 1;
	return 1;
}
/**
  * @brief  从机清命令cache
  *         
  *  
  * @param  
  *           
  *   
  * @retval 返回值说明
  *
  * @note   
  */
CDV_INT08U ClearPortCmdCache(void) {
	DELETE(g_portCmdCache.buf);
	g_portCmdCache.len = 0;
	g_portCmdCache.uart = 0;
	g_portCmdCache.mark = 0;		
	return 1;
}


/** @brief  从机收到命令的解析
  * @param  pBuf   命令
	*         len    命令长度
  * @retval RET_STATUS
  * @note   
  */
  RET_STATUS CascadeSlaveParse (char* pBuf, CDV_INT08U len) {
		char* pos = strchr((char*)pBuf,':') + 1;
		int par = atoi(pos);
			
		if(NULL == pos)
			return OPT_FAILURE;
		
		if(0 == strncmp((CDV_INT08C*)pBuf, "CHECK", pos - pBuf - 1))
		{
			CascadeCombine(par + 1);//序号+1为本机序号
			Cascade_Slave_Write(slaveTable, slaveTableLen);
			
		} 
		else if(0 == strncmp((CDV_INT08C*)pBuf, "TFCMD", pos - pBuf - 1))
		{
			
			if(!slaveTable)
			{
				return OPT_FAILURE;
			}
			
			if(CascadeGetNativeNo() != pos[3])//非本机，转发
			{
			  CDV_INT08U* recvBuf = NULL;
			  CDV_INT08U  recvLen = 0;
			  CascadeTranferCmd(&recvBuf, &recvLen, (CDV_INT08U*)pos, len - (pos - pBuf));
				Cascade_Slave_Write(recvBuf, recvLen);
				DELETE(recvBuf);
			}
			else
			{
//				CMD_ARG arg;
//				INIT_CLEAR(arg);
//				arg.uart = 0xFF;
//				arg.len = len - (pos - pBuf);
//				arg.buf =	(CDV_INT08U*)pos;

//				CmdParse(arg.buf, arg.len, &arg);
//				
//				/*处理资源回复请求*/
//				if(OPT_FAILURE == DoSpecRequest(&arg))
//				  DoResRequest(&arg);
//				
//				CmdArgDelete(&arg);
				CDV_INT16U crc;
				CDV_INT08U* buf = (CDV_INT08U*)pos;
				CDV_INT16U bufLen = len - (pos - pBuf);
				
				crc = getCRC16(buf, bufLen-2); 
				if((buf[bufLen-1] == ((crc >> 8) & 0xff)) && (buf[bufLen-2] == (crc & 0xff)))//crc chk
				{
					if(0 == PortCmdCache(buf, bufLen-2, 0xFF)) 
					{
						return OPT_FAILURE;
					}
				}
				else
				{
					Cascade_Slave_Write((CDV_INT08U*)"ERROR", 5);
			    return OPT_FAILURE;
				}
		
				
			}
//			CascadeCombine(par+1);//序号+1为本机序号
//			Cascade_Slave_Write(slaveTable, slaveTableLen);
			
		}
		else if(0 == strncmp((CDV_INT08C*)pBuf, "EXIST", pos - pBuf - 1))
		{
			Cascade_Slave_Write((CDV_INT08U*)"EXIST", 5);
		}
		else
		{
			Cascade_Slave_Write((CDV_INT08U*)"ERROR", 5);
			return OPT_FAILURE;
		}
		return OPT_SUCCESS;
	}
	
	
/** @brief  从机收到命令的解析
  * @param  pBuf   命令
	*         len    命令长度
  * @retval RET_STATUS
  * @note   有全局CRC判断，后续不需要CRC
  */
  RET_STATUS CascadeSlaveParse2 (char* pBuf, CDV_INT08U len) {
		char* pos = strchr((char*)pBuf,':') + 1;
		int par = atoi(pos);
		
		CDV_INT16U crc;
		
		crc = getCRC16(pBuf, len-2); 
		if((pBuf[len-1] == ((crc >> 8) & 0xff)) && (pBuf[len-2] == (crc & 0xff)))//crc chk
		{
		}
		else
		{
			Cascade_Slave_Write((CDV_INT08U*)"ERROR", 5);
			return OPT_FAILURE;
		}
		
		if(NULL == pos)
			return OPT_FAILURE;
		
		if(0 == strncmp((CDV_INT08C*)pBuf, "CHECK", pos - pBuf - 1))
		{
			CascadeCombine(par + 1);//序号+1为本机序号
			Cascade_Slave_Write(slaveTable, slaveTableLen);
			
		} 
		else if(0 == strncmp((CDV_INT08C*)pBuf, "TFCMD", pos - pBuf - 1))
		{
			
			if(!slaveTable)
			{
				return OPT_FAILURE;
			}
			
			if(CascadeGetNativeNo() != pos[3])//非本机，转发
			{
			  CDV_INT08U* recvBuf = NULL;
			  CDV_INT08U  recvLen = 0;
			  CascadeTranferCmd(&recvBuf, &recvLen, (CDV_INT08U*)pos, len - (pos - pBuf));
				Cascade_Slave_Write(recvBuf, recvLen);
				DELETE(recvBuf);
			}
			else
			{
				if(0 == PortCmdCache((CDV_INT08U*)pos, len - (pos - pBuf) - 2, 0xFF)) 
				{
					return OPT_FAILURE;
				}
			}
//			CascadeCombine(par+1);//序号+1为本机序号
//			Cascade_Slave_Write(slaveTable, slaveTableLen);
			
		}
		else if(0 == strncmp((CDV_INT08C*)pBuf, "EXIST", pos - pBuf - 1))
		{
			Cascade_Slave_Write((CDV_INT08U*)"EXIST", 5);
		}
		else if(0 == strncmp((CDV_INT08C*)pBuf, "GETRQ", pos - pBuf - 1))
		{
			SPI3_ReSend();
		}
		else
		{
			Cascade_Slave_Write((CDV_INT08U*)"ERROR", 5);
			return OPT_FAILURE;
		}
		return OPT_SUCCESS;
	}