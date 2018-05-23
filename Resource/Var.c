/**
  ******************************************************************************
  * @file    /Var.c 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-3-2
  * @brief   a package of var function
  * 
@verbatim  
20150106：增加不等于和等待不等于
               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "Var.h"
	
	#define DW_CDV_VAR(A) g_var[A] 
	//CDV_INT32S g_var[CDV_VAR_NUM] = {0};
	CDV_INT32S* g_var = (CDV_INT32S*)g_modbusReg.reg;
	
	//函数声明
//	void ValAdd(CDV_INT32U no, CDV_INT32S num);
//	void ValSub(CDV_INT32U no, CDV_INT32S num);
//	void ValMul(CDV_INT32U no, CDV_INT32S num);
//	void ValDiv(CDV_INT32U no, CDV_INT32S num);

//	void ValAnd(CDV_INT32U no, CDV_INT32S num);
//	void ValOr (CDV_INT32U no, CDV_INT32S num);
//	void ValNot(CDV_INT32U no, CDV_INT32S num);
//	void ValGre(CDV_INT32U no, CDV_INT32S num);
//	void ValEqu(CDV_INT32U no, CDV_INT32S num);
//	void ValNotEqu(CDV_INT32U no, CDV_INT32S num);
//	void ValLes(CDV_INT32U no, CDV_INT32S num);
//	RET_STATUS ValWaitGre(CDV_INT32U no, CDV_INT32S num);
//	RET_STATUS ValWaitEqu(CDV_INT32U no, CDV_INT32S num);
//	RET_STATUS ValWaitNotEqu(CDV_INT32U no, CDV_INT32S num);
//	RET_STATUS ValWaitLes(CDV_INT32U no, CDV_INT32S num);
//	void ValMod(CDV_INT32U no, CDV_INT32S num);
//	
//	
//	void ValParRead(CDV_INT32U no , CDV_INT32U flag , CDV_INT08U* buf , CDV_INT08U* len);
//	
//	

/**
  * @brief  变量赋值
  *  
  * @param  no     要操作的变量号
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
void ValSet(CDV_INT32U no, CDV_INT32S num) {
	ASSERT(no < CDV_VAR_NUM);
	DW_CDV_VAR(no) = num;
	//WRITE_VAR( no , num);
}
/**
  * @brief  变量读值
  *  
  * @param  no     要操作的变量号
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
CDV_INT32S VarGet(CDV_INT32U no) {
	ASSERT(no < CDV_VAR_NUM);
	return DW_CDV_VAR(no);
	//WRITE_VAR( no , num);
}
/**
  * @brief  变量赋值flash
  *  
  * @param  no     要操作的变量号
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
void ValFlashSet(CDV_INT32U no, CDV_INT32S num) {
	ASSERT(no < CDV_VAR_NUM);
	DW_CDV_VAR(no) = num;
	SPI_Flash_Write((CDV_INT08U *)&num, VAR_ADDR(no), 4);
}
/**
  * @brief  所有变量赋值flash
  *  
  * @param  no起始变量
						num操作变量个数
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
void ValToFlash(CDV_INT32U no, CDV_INT16U num) {
	ASSERT(no + num <= CDV_VAR_NUM);
	SPI_Flash_Write((CDV_INT08U *)&DW_CDV_VAR(no), VAR_ADDR(no), (num<<2));
}
/**
  * @brief  所有变量赋值flash
  *  
  * @param  no起始变量
						num操作变量个数
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
void FlashToVal(CDV_INT32U no, CDV_INT16U num) {
	ASSERT(no + num <= CDV_VAR_NUM);
	SPI_Flash_Read((CDV_INT08U *)&DW_CDV_VAR(no), VAR_ADDR(no), (num<<2));
}
/**
  * @brief  变量读flash
  *  
  * @param  no     要操作的变量号
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
CDV_INT32S ValFlashGet(CDV_INT32U no) {
	CDV_INT32S num;
	ASSERT(no < CDV_VAR_NUM);
	SPI_Flash_Read((CDV_INT08U *)&num, VAR_ADDR(no), 4);
	DW_CDV_VAR(no) = num;
	return num;
}
/**
  * @brief  变量加
  *  
  * @param  no     要操作的变量号
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
void ValAdd(CDV_INT32U no, CDV_INT32S num) {
	//CDV_INT32S val = R_VAR(no) + num;
	ASSERT(no < CDV_VAR_NUM);
	DW_CDV_VAR(no) += num;
	//WRITE_VAR( no , val);
}
/**
  * @brief  变量减
  *  
  * @param  no     要操作的变量号
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
void ValSub(CDV_INT32U no, CDV_INT32S num) {
	//CDV_INT32S val = R_VAR(no) - num;
	ASSERT(no < CDV_VAR_NUM);
	DW_CDV_VAR(no) -= num;
	//WRITE_VAR( no , val);
}
/**
  * @brief  变量乘
  *  
  * @param  no     要操作的变量号
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
void ValMul(CDV_INT32U no, CDV_INT32S num) {
//	CDV_INT32S val = R_VAR(no) * num;
//	WRITE_VAR( no , val);
	ASSERT(no < CDV_VAR_NUM);
	DW_CDV_VAR(no) *= num;
}
/**
  * @brief  变量除
  *  
  * @param  no     要操作的变量号
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
void ValDiv(CDV_INT32U no, CDV_INT32S num) {
//	CDV_INT32S val;
	ASSERT(no < CDV_VAR_NUM);
//	ASSERT(0 != num);
	if(!num)
		return;
//	val = R_VAR(no) / num;
//	WRITE_VAR( no , val);
	DW_CDV_VAR(no) /= num;
}
/**
  * @brief  变量与
  *  
  * @param  no     要操作的变量号
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
void ValAnd(CDV_INT32U no, CDV_INT32S num) {
	ASSERT(no < CDV_VAR_NUM);
	DW_CDV_VAR(no) = DW_CDV_VAR(no) && num;
}
/**
  * @brief  变量或
  *  
  * @param  no     要操作的变量号
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
void ValOr(CDV_INT32U no, CDV_INT32S num) {
	ASSERT(no < CDV_VAR_NUM);
  DW_CDV_VAR(no) = DW_CDV_VAR(no) || num;
}
/**
  * @brief  变量非
  *  
  * @param  no     要操作的变量号
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
void ValNot(CDV_INT32U no, CDV_INT32S num) {
	ASSERT(no < CDV_VAR_NUM);
  DW_CDV_VAR(no) = !DW_CDV_VAR(no);
}
/**
  * @brief  变量大于
  *  
  * @param  no     要操作的变量号
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
void ValGre(CDV_INT32U no, CDV_INT32S num) {
	ASSERT(no < CDV_VAR_NUM);
  DW_CDV_VAR(no) = DW_CDV_VAR(no) > num;
}
/**
  * @brief  变量等于
  *  
  * @param  no     要操作的变量号
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
void ValEqu(CDV_INT32U no, CDV_INT32S num) {
	ASSERT(no < CDV_VAR_NUM);
  DW_CDV_VAR(no) = DW_CDV_VAR(no) == num;
}
/**
  * @brief  变量不等于
  *  
  * @param  no     要操作的变量号
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
void ValNotEqu(CDV_INT32U no, CDV_INT32S num) {
	ASSERT(no < CDV_VAR_NUM);
  DW_CDV_VAR(no) = DW_CDV_VAR(no) != num;
}

/**
  * @brief  变量小于
  *  
  * @param  no     要操作的变量号
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
void ValLes(CDV_INT32U no, CDV_INT32S num) {
	ASSERT(no < CDV_VAR_NUM);
  DW_CDV_VAR(no) = DW_CDV_VAR(no) < num;
}

/**
  * @brief  变量等待大于
  *  
  * @param  no     要操作的消息号
  *   
  * @retval OPT_SUCCESS等到了
						OPT_FAILURE还没等到
  *
  * @note   无溢出处理
  */
RET_STATUS ValWaitGre(CDV_INT32U no, CDV_INT32S num){
  RET_STATUS ret = OPT_FAILURE;
	ASSERT(no < CDV_VAR_NUM);
	
	if(DW_CDV_VAR(no) > num){
		ret = OPT_SUCCESS;
	}
	
	return ret;
}
/**
  * @brief  变量等待等于
  *  
  * @param  no     要操作的消息号
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
RET_STATUS ValWaitEqu(CDV_INT32U no, CDV_INT32S num){
  RET_STATUS ret = OPT_FAILURE;
	ASSERT(no < CDV_VAR_NUM);
	
	if(DW_CDV_VAR(no) == num){
		ret = OPT_SUCCESS;
	}
	return ret;
}
/**
  * @brief  变量等待不等于
  *  
  * @param  no     要操作的消息号
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
RET_STATUS ValWaitNotEqu(CDV_INT32U no, CDV_INT32S num){
  RET_STATUS ret = OPT_FAILURE;
	ASSERT(no < CDV_VAR_NUM);
	
	if(DW_CDV_VAR(no) != num){
		ret = OPT_SUCCESS;
	}
	return ret;
}
/**
  * @brief  变量等待小于
  *  
  * @param  no     要操作的消息号
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
RET_STATUS ValWaitLes(CDV_INT32U no, CDV_INT32S num){
  RET_STATUS ret = OPT_FAILURE;
	ASSERT(no < CDV_VAR_NUM);
	
	if(DW_CDV_VAR(no) < num){
		ret = OPT_SUCCESS;
	}
	return ret;
}
/**
  * @brief  求余
  *  
  * @param  no     要操作的变量号
  *   
  * @retval 
  *
  * @note   无溢出处理
  */
void ValMod(CDV_INT32U no, CDV_INT32S num){
	ASSERT(no < CDV_VAR_NUM);
  DW_CDV_VAR(no) %= num;
}

/**
  * @brief  Val参数读取
  *  
  * @param  no     要设置的I号
  *         flag   设置的参数标记
						buf    参数buf
  *   
  * @retval 无
  *
  * @note   
  */
void ValParRead(CDV_INT32U no , CDV_INT32U flag , CDV_INT08U* buf , CDV_INT08U* len) {
	*len = 0;
	ASSERT(no < CDV_VAR_NUM);
	
	//if (flag & 0x0001) 
	{
		buf[(*len) ++] = (CDV_INT08U)(DW_CDV_VAR(no));
		//(*len) ++;
		buf[(*len) ++] = (CDV_INT08U)(DW_CDV_VAR(no) >> 8);
		buf[(*len) ++] = (CDV_INT08U)(DW_CDV_VAR(no) >> 16);
		buf[(*len) ++] = (CDV_INT08U)(DW_CDV_VAR(no) >> 24);
	}
}

/**
  * @brief  变量脚本命令
  *  
  * @param  scriptNo     待分析工人号  
  *   
  * @retval 返回值说明OPT_SUCCESS，OPT_FAILURE
  *
  * @note   资源号（00-FF）	00启动设置		FF	uint32数值低位	uint32数值	uint32数值	uint32数值高位					字符串
																					枚举号（00-03	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	
														01运行中修改	FF	uint32数值低位	uint32数值	uint32数值	uint32数值高位					
																					枚举号（00-03	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	
														02赋值				FF	uint32数值低位	uint32数值	uint32数值	uint32数值高位					
																					枚举号（00-03									
														03自加				FF	uint32数值低位	uint32数值	uint32数值	uint32数值高位					
														04自减				FF	uint32数值低位	uint32数值	uint32数值	uint32数值高位					
            使用到全局变量R_APP_SET_NUM，由于是启动脚本才有的，故不会发生冲突
  */

RET_STATUS ValCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg) { 
//	OS_ERR err;
	//CDV_INT08U len ;//, row ;
	CDV_INT08U opt , type /*, strLen , *txBuf = NULL*/;
	CDV_INT32U no;
	CDV_INT32S num;
	RET_STATUS ret = OPT_SUCCESS;
	CDV_INT08U tmpPar[VAL_PAR_NUM] = {0} , tmpLen = 0;
//	strLen = rxBuf[rxLen - 1];//获取最后一个字节，表示的是前面的中文串长度
//	no   = rxBuf[0];
//	opt  = rxBuf[1];
//	type = rxBuf[2];
	memcpy(&no, rxBuf, 4);
	opt  = rxBuf[4];
	type = rxBuf[5];
	
	if(no >= CDV_VAR_NUM)
		return OPT_FAILURE;
	if (0xff == type) {//数值型
//		num = (CDV_INT32S)((rxBuf[3]<<24) | (rxBuf[4]<<16) | (rxBuf[5]<<8) | rxBuf[6]);//大端
		memcpy(&num, rxBuf + 6, 4);//小端
//		len = strLen + 9;
	} else if (0xfe == type){//变量型
		if(*(CDV_INT32U*)(rxBuf + 6) >= CDV_VAR_NUM)
		  return OPT_FAILURE;
		num = DW_CDV_VAR(*(CDV_INT32U*)(rxBuf + 6));
	} else {
		num = type;
//		len = strLen + 13;
	}
  
	switch(opt) {
//		case 0x00:/*启动设置*/
//		case 0x01:/*运行修改*/
////			if (CDV_START != g_cdvStat || debug)
////				return OPT_SUCCESS;//提前结束
//			if(0x01 == opt)
//				num = ValFlashGet(no);
//			else
//				num =0xffffffff;
//			
//			ValSet(no , num);
//		  //OSSemPend(&TX_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //请求信号量
//			//txBuf = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*len);
//			NEW08U(txBuf , len);
//			//USART_TX_ADD_WITH_LEN(len);//开辟空间
//			txBuf[0] = APP_ID;
//			txBuf[1] = 0x01;
//			txBuf[2] = (APP_SET_NUM)++;//第几条设置命令，不能用no
//		  txBuf[3] = opt;
//			txBuf[4] = type;
//			switch (type) {
//				case 0xFF:
//					txBuf[5] = (CDV_INT08U)(num>>24);
//					txBuf[6] = (CDV_INT08U)(num>>16);
//					txBuf[7] = (CDV_INT08U)(num>>8);
//					txBuf[8] = (CDV_INT08U)num;
//					//memcpy(txBuf + 5, rxBuf+3 , strLen+4);
//					memcpy(txBuf + 9, rxBuf+7 , strLen);
//					break;
//				case 0x00:
//				case 0x01:
//				case 0x02:
//				case 0x03:
//					//USART_TX_BUF(4) = *(SCRIPT_CMD_ENUM_ADDR(scriptNo, type));
//					//USART_TX_BUF(5) = *(SCRIPT_CMD_ENUM_ADDR(scriptNo, type)+1);
//					memcpy(txBuf + 5, rxBuf+3 , strLen+8);
//					break;
//				default:
//					DELETE (txBuf);
//					return OPT_SUCCESS;
//		//			break;
//			}
//			AddRequestTx(txBuf , len , uartNo);
//			DELETE (txBuf);
//			break;
		case 0x02:/*赋值*/
			ValSet(no , num);
			break;
		case 0x03:/*加*/
			ValAdd(no , num);
			break;
		case 0x04:/*减*/
			ValSub(no , num);
			break;
		case 0x05:/*乘*/
			ValMul(no , num);
			break;
		case 0x06:/*除*/
			ValDiv(no , num);
			break;
		case 0x07:/*与*/
			ValAnd(no , num);
			break;
		case 0x08:/*或*/
			ValOr(no , num);
			break;
		case 0x09:/*非*/
			ValNot(no , num);
			break;
		case 0x0A:/*大于*/
			ValGre(no , num);
			break;
		case 0x0B:/*等于*/
			ValEqu(no , num);
			break;
		case 0x0C:/*小于*/
			ValLes(no , num);
			break;
		case 0x0D:/*等待大于*/
			ret = ValWaitGre(no , num);
			break;
		case 0x0E:/*等待等于*/
			ret = ValWaitEqu(no , num);
			break;
		case 0x0F:/*等待小于*/
			ret = ValWaitLes(no , num);
			break;
		case 0x10:/*读取*/
			//if(0xFF == type) 
			{
			  ValParRead(no , /*rxBuf[3]<<8 | rxBuf[4]*/num , tmpPar , &tmpLen);
				ResRequest(arg->buf, arg->len, tmpPar, tmpLen, arg);//ResParaRequest(rxBuf, rxLen , tmpPar, tmpLen,arg->uart);
			}
			break;
		case 0x11:/*不等于*/
			ValNotEqu(no , num);
			break;
		case 0x12:/*等待不等于*/
			ret = ValWaitNotEqu(no , num);
			break;
		case 0x13:/*求余*/
			ValMod(no , num);
			break;
		case 0x14:/*赋初值*/
			ValFlashSet(no , num);
			break;
		default:
			break;
		
	}
//	if(OPT_FAILURE == ret) {
//		OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_HMSM_STRICT,&err);   //延时 5ms
//	}
	return ret;
}