/**
  ******************************************************************************
  * @file    /uartx.c 
  * @author  MMY
  * @version V0.0.1
  * @date    2017-4-1
  * @brief   a package of uartx
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
	
	#include "uartx.h"
	#include "uart1.h"
	#include "uart2.h"
	#include "uart3.h"
	#include "uart4.h"
	#include "uart5.h"
	#include "uart6.h"

/*USART����*/
void USART_Configuration(void){
//  USART1_Configuration(115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
//	USART2_Configuration(115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
//	USART3_Configuration(115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
	UART4_Configuration(115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
//	UART5_Configuration(115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);
//	USART6_Configuration(115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No);

}
void (*p_UartSet[])(u32 , u16 , u16 , u16) =
{
	USART1_Configuration,
	
#if _NPC_VERSION_ == 2u
	USART2_Configuration,
	USART3_Configuration,
#elif _NPC_VERSION_ == 3u
	NULL,
	NULL,
#endif
	UART4_Configuration,
	UART5_Configuration,
	USART6_Configuration,
};

/**
  * @brief  ��������������
  * @param  buf
	*					len
	*					no :1 - 6
  * @retval void
  * @note   ��������ͳһ���
  */
void USARTSet(u32 bound, u16 wordLength, u16 stopBits, u16 parity, u8 no) {
	ASSERT(0 < no && 6 >= no);
  ASSERT(IS_USART_BAUDRATE(bound));  
  ASSERT(IS_USART_WORD_LENGTH(wordLength));
  ASSERT(IS_USART_STOPBITS(stopBits));
  ASSERT(IS_USART_PARITY(parity));
	
	if(!p_UartSet[no - 1])
		return;
		
	p_UartSet[no-1](bound, wordLength, stopBits, parity);
}


void (*p_UartSend[])(u8* ,u16) =
{
	USART1_Send,
#if _NPC_VERSION_ == 2u
	USART2_Send,
	USART3_Send,
#elif _NPC_VERSION_ == 3u
	NULL,
	NULL,
#endif
	UART4_Send,
	UART5_Send,
	USART6_Send,
};

	

/**
  * @brief  ���ڷ���������
  * @param  buf
	*					len
	*					no :1 - 6
  * @retval void
  * @note   ���ڷ���ͳһ���
  */
void USARTSend(u8 *buf ,u16 len ,u8 no) {
	ASSERT(0 < no && 6 >= no);
	if(!p_UartSend[no - 1])
		return;
	p_UartSend[no-1](buf ,len);
	
}

void (*p_UartTR[])(u8 *,u16  ,u8*  ,u8 ,u8* ) =
{
	USART1_TR,
#if _NPC_VERSION_ == 2u
	USART2_TR,
	USART3_TR,
#elif _NPC_VERSION_ == 3u
	NULL,
	NULL,
#endif
	UART4_TR,
	UART5_TR,
	USART6_TR,
};

	

/**
  * @brief  ���ڷ��ͽ���������
  * @param  txbuf  Ҫ���͵��ַ���
  *         txlen  Ҫ���͵��ַ�������
  *         rxbuf  ���յ��ַ�������
  *         rxbufLen �����ַ�������ĳ���
  *         rxlen  ���յ����ַ�������
	*					no :1 - 6�����Ĵ��ں�
  * @retval void
  * @note   ���ڷ��ͽ���ͳһ���
  */
void USARTTR(u8 *txbuf,u16 txlen ,u8* rxbuf ,u8 rxbufLen,u8* rxlen ,u8 no) {
	ASSERT(0 < no && 6 >= no);
	if(!p_UartTR[no - 1])
		return;
	p_UartTR[no-1](txbuf, txlen , rxbuf , rxbufLen, rxlen);
}
/*ͳһ�жϴ���*/
void USARTx_IRQHandler(USART_TypeDef* USARTx)
{
	CDV_INT08U Res;
  //tm1Re = 0;
	tm1Re = ReadClock1ms();
	if(USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)   /*�����ж�()*/
	{
		USART_ClearITPendingBit(USARTx,USART_IT_RXNE);
		Res =USART_ReceiveData(USARTx);                       /*(USARTx->DR);	//��ȡ���յ�������*/
		switch(g_cdvStat){
			case CDV_RECV:
				g_scriptRecv.buf[g_scriptRecv.rxPos][g_scriptRecv.len[g_scriptRecv.rxPos]++] = Res;//���浽����
			  if (QUE_LEN <=  g_scriptRecv.len[g_scriptRecv.rxPos])
					MAX_SELF_ADD(g_scriptRecv.rxPos, QUE_NUM);
				break;
			default:
				USART_RX_BUF_ADD_CHAR(Res);
				break;
		}
  } 
}