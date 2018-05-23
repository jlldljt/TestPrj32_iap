/**
  ******************************************************************************
  * @file    /worker.c 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-3-15
  * @brief   
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
	
	#include "worker.h"
	
const char *WorkerStatusName[] = {"WORKER_STOP","WORKER_ONCE","WORKER_LOOP","WORKER_PAUSE","WORKER_EXIT","WORKER_MAX_STATUS"};
THREAD_INFO g_threadInfo[WORKER_MAX_NUM] = {0};

/** @brief  ���˿���
  * @param  no ���Ź���
  *         status Ŀ��״̬���޴�״̬������OPT_FAILURE
  * @retval OPT_FAILURE �л�ʧ��| OPT_SUCCESS �л��ɹ�
  * @note   
  */
BOOL IsTaskExist(const OS_TCB tcb) {
	if(OS_TASK_STATE_DEL == tcb.TaskState
			|| NULL == tcb.StkPtr)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
	
}
	


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////���ù��˼��������̵Ŀ���/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////������MEM�еĽű�///////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @brief  ��ָ�buf
  * @param  
  * @retval ����ֵ˵��
  * @note   
  */
RET_STATUS ReadWorkerBuf(DEBUG_SCRIPT *debugWorker) {

	RET_STATUS ret;
	
	Mem_Read(&debugWorker->len, debugWorker->cmdPos + 2, 1);
	debugWorker->len += 3;
	//NEW08U(debugWorker->buf , debugWorker->len);
  //Mem_Read(debugWorker->buf, debugWorker->cmdPos + 3, (CDV_INT16U)(debugWorker->len));	
  debugWorker->buf = Mem_Ptr(debugWorker->cmdPos/* + 3*/);
	ASSERT(NULL != debugWorker->buf);
	return ret;
}


/** @brief  ��ת�����˵�ĳ������
  * @param  void
  * @retval ����ֵ˵��
  * @note   �����ڳ�ʼ��֮��ִ��
  */
RET_STATUS ToWorkerCmd(DEBUG_SCRIPT *debugWorker , CDV_INT16U cmdNo) {
  CDV_INT32U staddr = debugWorker->startPos - FLASH_WORK_ADDR;
	debugWorker->doNo = cmdNo;

	debugWorker->cmdPos = Mem_GetCmdPos(cmdNo , debugWorker->cmdNum , staddr);
	
	ReadWorkerBuf(debugWorker);
  return OPT_SUCCESS;
}



/** @brief  ��ʼ�����Թ���
  * @param  no     ����ȡ���˺�
  * @retval ����ֵ˵��
  * @note   
  */
RET_STATUS InitWorker(const CDV_INT08U no, DEBUG_SCRIPT *debugWorker) {
	
//	CDV_INT08U i = no/* ,*buf = NULL*/;
	CDV_INT32U staddr;
	CDV_INT16U len = 0;
	//DebugClear();
  if(!g_line.init) {
		return OPT_FAILURE;
	}
	
	if(no >= g_line.workerAddr.len) {
		return OPT_FAILURE;
	}
	
	///////////////////////////
	
	len = 0;
	/*��ȡ�ű���Ϣ*/
	
	debugWorker->startPos = SCRIPADDR(no);//��ʼ��ַ
	Mem_Read((CDV_INT08U*)&len, SCRIP_LEN(no) - FLASH_WORK_ADDR, 2);
  debugWorker->totalLen = len;
	
	if(2 < len) {
		debugWorker->cmdNum =  Mem_TestCmdNum(debugWorker->startPos - FLASH_WORK_ADDR , len);
	} else {
		debugWorker->cmdNum = 0;
	}
	
	if(0 == debugWorker->cmdNum)
		return OPT_FAILURE;
	
	ToWorkerCmd(debugWorker, 0);//
	debugWorker->which = no;
	return OPT_SUCCESS;
}
	

/** @brief  ��һ������
  * @param  void
  *         loop ѭ����1ѭ����0��ѭ��
  * @retval OPT_SUCCESS �ɹ�ָ����һ������
  *         OPT_FAILURE ʧ��
  * @note   pos����ת����һ�����ʼ��
20151228 ��ԭ�����ж�����������Ƿ�ű������ĳ��ж�flash��ַƫ�ƴ�С
  */
RET_STATUS NextWorkerCmd(DEBUG_SCRIPT *debugWorker , CDV_INT08U loop) {
		
  CDV_INT32U staddr = debugWorker->startPos - FLASH_WORK_ADDR;
	
	
  debugWorker->cmdPos = Mem_GetNextCmdPos(debugWorker->cmdPos);

	if(debugWorker->cmdPos - staddr >= debugWorker->totalLen) {
		if(loop) {

			debugWorker->cmdPos = staddr;

			debugWorker->doNo = 0;	
		} else {
			return OPT_FAILURE;
		}
	}	else {
		
		debugWorker->doNo++;
	}
	
	ReadWorkerBuf(debugWorker);
	
	debugWorker->doNo = Mem_GetNoFromPos(staddr,debugWorker->cmdPos);
	return OPT_SUCCESS;
}



/** @brief  ������ǰ���ִ��
  * @param  scriptNo     ���������˺�
  * @retval ����ֵ˵��
  * @note   ���뱣֤ͬʱֻ��һ���߳��ڷ�����ͬscriptNo�Ľű�
            �ڼ�������λ��	�ڼ�������λ��	����ʣ���ֽ�	    ��Դ����	������1	        ������2	    ������3
            XX	            XX	            ʣ�೤�ȣ�00-FF��	00	      ��Դ�ţ�00-FF��	00�����˶�	FF
  */
RET_STATUS WorkerCmdParse(DEBUG_SCRIPT *debugWorker , CDV_INT08U uartNo) {
	RET_STATUS ret = OPT_SUCCESS;
	CMD_ARG arg;
	INIT_CLEAR(arg);
	if(CascadeGetNativeNo() == debugWorker->buf[3] || 
				(1 == debugWorker->buf[0] && 0x10 > debugWorker->buf[1]))
	{
		arg.uart = uartNo;
		arg.ptrWorker = debugWorker;
		ret = CmdParse (debugWorker->buf , debugWorker->len , &arg);
		//ret =  ResParse (debugWorker->buf , debugWorker->len ,(void* )&debugWorker->which, 0, &arg);
	}
	else
	{
		CDV_INT08U* recvBuf = NULL;
		CDV_INT08U  recvLen = 0;
		//g_olCache.buf[3] = 2;
		
		
		if(OPT_SUCCESS == (ret = CascadeTranferCmd(&recvBuf, &recvLen, debugWorker->buf , debugWorker->len)))
		  ret = TFCmdParse(recvBuf, recvLen);

		DELETE(recvBuf);
	}
	CmdArgDelete(&arg);
	return ret;
}


/** @brief  mem�ű�����
  * @param  void
  *         ret   1 ����0����
  *               0�� ��������
						loop ѭ����1ѭ����0��ѭ��
  * @retval ����ֵ˵��OPT_SUCCESS����ִ�У�OPT_FAILUREִֹͣ�У��ű�ִ�е����һ��
  * @note   2-9�Žű�
  */
RET_STATUS RunWorkerCmd(DEBUG_SCRIPT *debugWorker , CDV_INT08U loop) {

	if(OPT_SUCCESS == WorkerCmdParse(debugWorker, loop)) {		
	    return NextWorkerCmd(debugWorker, loop);
	} else {
		return OPT_SUCCESS;
	}

}
/** @brief  ���ù���
  * @param  
  * @retval ����ֵ˵��
  * @note   �����ڻ�ȡ�깤�����̺����
  */
void RunWorker(THREAD_INFO *pCtrl) {
  CDV_INT08U loop = 0;
  DEBUG_SCRIPT worker = {0, 0, 0, 0, 0, 0, NULL, 0};
	OS_ERR err;
	
	if (OPT_SUCCESS != InitWorker(pCtrl->which, &worker)) {
		pCtrl->status = WORKER_STOP;
    return;		
	}
	
	if(1/*0 != pCtrl->which*/) {/*�ǹ�������*/
		do {
			
			switch (pCtrl->status) {
				case WORKER_LOOP:
					loop = 1;
					break;
				case WORKER_ONCE:
					loop = 0;
					break;
				case WORKER_STOP:
					return;
				case WORKER_PAUSE:
					while (WORKER_PAUSE == pCtrl->status)
						OS_TaskSuspend((OS_TCB*)0,&err);
					break;
				case WORKER_EXIT:
					if (worker.doNo == pCtrl->exitStep) {
						pCtrl->status = WORKER_STOP;
						return;
					}
					break;
				default:
					break;
			}
			pCtrl->step = worker.doNo;
			//if(8 == pCtrl->which && 29 == pCtrl->step )
			//	pCtrl->step = worker.doNo;
		} while (OPT_SUCCESS == RunWorkerCmd (&worker , loop));
	}	else {/*��������*/
		do {
			int i = 0;
			int flag = 0;
			switch (pCtrl->status) {
				case WORKER_LOOP:
					
					break;
				case WORKER_ONCE:
					loop = 0;
					break;
				case WORKER_STOP:
					do {
						flag = 0;
						for( i = 0; i < WORKER_MAX_NUM; i++)                                    /*�жϹ����Ƿ��Ѿ�����*/
						{
							if (IsTaskExist(WorkerTaskTCB[i]))
							{
								WorkerControl(g_threadInfo[i].which, WORKER_STOP);
								flag = 1;
							}
						}
					} while(flag);/*�ȴ�ֱ�����й����˳�*/
					return;
				case WORKER_PAUSE:
					
					break;
				case WORKER_EXIT:
					
					break;
				default:
					loop = 1;
					break;
			}
			pCtrl->step = worker.doNo;
		} while (OPT_SUCCESS == RunWorkerCmd (&worker , loop));
		
	}
	
	pCtrl->status = WORKER_STOP;
//	for ( worker.doNo = 0; worker.doNo < worker.cmdNum ;) {
//		if (OPT_FAILURE == RunWorkerCmd(worker.doNo, uartNo)) {

//		} else {
//		   worker.doNo++;
//		}
//	}	
//	return OPT_SUCCESS;
}
void WorkerTask(void *p_arg) {
	
	RunWorker((THREAD_INFO *)p_arg);
}

/** @brief  �����̷߳���
  * @param  
  * @retval ����ֵ˵��
  * @note   �ⲿ�迼���߳��Ƿ������У�����ᵼ�¸��߳��������ݱ����
  */
RET_STATUS WorkerThreadRun(const CDV_INT08U tNo, const CDV_INT08U no, const WORKER_STATUS status) {
	OS_ERR err;
	RET_STATUS ret = OPT_FAILURE;
	ASSERT(tNo < WORKER_MAX_NUM);
  g_threadInfo[tNo].which = no;
	g_threadInfo[tNo].status = status;
	//TaskState 0xffΪ�˳��̣߳����̺߳���ִ�����or������del
//	if(OS_TASK_STATE_DEL == WorkerTaskTCB[tNo].TaskState || NULL == WorkerTaskTCB[tNo].StkPtr)
	{
	  OSTaskCreate((OS_TCB* )&WorkerTaskTCB[tNo],
				(CPU_CHAR* )workerName[tNo],
				(OS_TASK_PTR )WorkerTask,
				(void* )&g_threadInfo[tNo],
				(OS_PRIO )WORKER_TASK_PRIO,
				(CPU_STK* )&WORKER_TASK_STK[tNo][0],
				(CPU_STK_SIZE )WORKER_STK_SIZE/10,
				(CPU_STK_SIZE )WORKER_STK_SIZE,
				(OS_MSG_QTY )0,
				(OS_TICK )0,
				(void* )0,
				(OS_OPT )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
				(OS_ERR* )&err);	
	if (err == OS_ERR_NONE)
		ret = OPT_SUCCESS;
							
	}
	
	return ret;
}



/** @brief  ���˿���
  * @param  no ���Ź���
  *         status Ŀ��״̬���޴�״̬������OPT_FAILURE
  * @retval OPT_FAILURE �л�ʧ��| OPT_SUCCESS �л��ɹ�
  * @note   
  */
RET_STATUS WorkerControl(const CDV_INT08U no, const WORKER_STATUS status) {
	int i = 0;
  OS_ERR err;
	RET_STATUS ret = OPT_FAILURE;
	
	if (status >= WORKER_MAX_STATUS)
		return OPT_FAILURE;
	
	OSSemPend(&WORKER_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
	
	for( i = 0; i < WORKER_MAX_NUM; i++)                                    /*�жϹ����Ƿ��Ѿ�����*/
	{
		if (no == g_threadInfo[i].which && IsTaskExist(WorkerTaskTCB[i]))
		{
			//g_threadInfo[i].status = status;
			break;
		}
	}
	
	switch (status) {
		case WORKER_STOP:
		case WORKER_PAUSE:
		case WORKER_EXIT:
			if (WORKER_MAX_NUM != i) {                                          /*�����Ѿ���i�߳�����*/
				g_threadInfo[i].status = status;
				OSTaskResume((OS_TCB*)&WorkerTaskTCB[i],&err);
			}
			ret = OPT_SUCCESS;
			break;
		case WORKER_ONCE:
		case WORKER_LOOP:
			if (WORKER_MAX_NUM != i) {
				g_threadInfo[i].status = status;
				OSTaskResume((OS_TCB*)&WorkerTaskTCB[i],&err);	
				ret = OPT_SUCCESS;
			} else {
				for( i = 0; i < WORKER_MAX_NUM; i++)                              /*���ҿ����߳�*/
				{
					if(!IsTaskExist(WorkerTaskTCB[i]))
//					if(OS_TASK_STATE_DEL == WorkerTaskTCB[i].TaskState
//						|| NULL == WorkerTaskTCB[i].StkPtr)
					{
						ret = WorkerThreadRun(i, no, status);                        /*������������*/
						break;
					}
				}
			}
			break;
		default:
			break;
	}
  
  OSSemPost (&WORKER_SEM,OS_OPT_POST_1,&err);
	return ret;
}


/** @brief  �����˳�����
  * @param  no ���Ź���
  *         step �ڼ���
  * @retval OPT_FAILURE ����ʧ��| OPT_SUCCESS ���óɹ�
  * @note   
  */
RET_STATUS WorkerExitSet(CDV_INT08U no, CDV_INT32U step) {
	int i = 0;
	RET_STATUS ret = OPT_FAILURE;
	for( i = 0; i < WORKER_MAX_NUM; i++)                                    /*�жϹ����Ƿ��Ѿ�����*/
	{
		if (no == g_threadInfo[i].which)
		{
			g_threadInfo[i].exitStep = step;
			ret = OPT_SUCCESS;
			break;
		}
	}
	return ret;
}

/** @brief  ����״̬��ѯ
  * @param  no ���Ź���
  *         status Ŀ��״̬
  * @retval OPT_FAILURE ��ѯʧ��| OPT_SUCCESS ��ѯ�ɹ�
  * @note   
  */
RET_STATUS WorkerQuery(const CDV_INT08U no, WORKER_STATUS* pStatus, CDV_INT32U* pStep) {
	int i = 0;
	RET_STATUS ret = OPT_FAILURE;
	ASSERT(pStatus != NULL);
	ASSERT(pStep != NULL);
	for( i = 0; i < WORKER_MAX_NUM; i++)                                    /*�жϹ����Ƿ��Ѿ�����*/
	{
		if (no == g_threadInfo[i].which)
		{
			*pStatus = g_threadInfo[i].status;
			*pStep = g_threadInfo[i].step;
			ret = OPT_SUCCESS;
			break;
		}
	}
	return ret;
}

/** @brief  ����״̬��ѯ
  * @param  no ���Ź���
  *         status Ŀ��״̬
  * @retval OPT_FAILURE ��ѯʧ��| OPT_SUCCESS ��ѯ�ɹ�
  * @note   
  */
RET_STATUS WorkerQueryStatus(const CDV_INT08U no, WORKER_STATUS* pStatus) {
	int i = 0;
	RET_STATUS ret = OPT_FAILURE;
	ASSERT(pStatus != NULL);
	for( i = 0; i < WORKER_MAX_NUM; i++)                                    /*�жϹ����Ƿ��Ѿ�����*/
	{
		if (no == g_threadInfo[i].which)
		{
			*pStatus = g_threadInfo[i].status;
			ret = OPT_SUCCESS;
			break;
		}
	}
	return ret;
}

/** @brief  ����״̬��ѯ
  * @param  no ���Ź���
  *         status Ŀ��״̬
  * @retval OPT_FAILURE ��ѯʧ��| OPT_SUCCESS ��ѯ�ɹ�
  * @note   
  */
RET_STATUS WorkerQueryStep(const CDV_INT08U no, CDV_INT16U* pStep) {
	int i = 0;
	RET_STATUS ret = OPT_FAILURE;
	ASSERT(pStep != NULL);
	for( i = 0; i < WORKER_MAX_NUM; i++)                                    /*�жϹ����Ƿ��Ѿ�����*/
	{
		if (no == g_threadInfo[i].which)
		{
			*pStep = g_threadInfo[i].step;
			ret = OPT_SUCCESS;
			break;
		}
	}
	return ret;
}


/** @brief  �������˿���
  * @param  status Ŀ��״̬
  * @retval OPT_FAILURE �л�ʧ��| OPT_SUCCESS �л��ɹ�
  * @note   
  */
RET_STATUS ManagerControl(const WORKER_STATUS status) {
	return WorkerControl(0, status);
}

/** @brief  ����״̬��ѯ
  * @param  no ���Ź���
  *         status Ŀ��״̬
  * @retval OPT_FAILURE ��ѯʧ��| OPT_SUCCESS ��ѯ�ɹ�
  * @note   
  */
RET_STATUS ManagerQueryStatus(WORKER_STATUS* pStatus) {
	return WorkerQueryStatus(0, pStatus);
}

void AllWorkerCtrl(const WORKER_STATUS status)
{
	CDV_INT08U i;
	
	for(i = 0; i < g_line.workerAddr.len; i++)
	{
		WorkerControl(i, status);
	}
	
}
/******************************************************************
*******************************************************************
*******************************************************************
*******************************************************************
*******************************************************************
*******************************************************************
*******************************************************************
*******************************************************************
******************************************************************/

/** @brief  ���˹�������
  * @param    
  * @retval ����ֵ˵��OPT_SUCCESS��OPT_FAILURE
  * @note   							
  */
RET_STATUS WorkerCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen , CMD_ARG *arg) {
	OS_ERR err;
	CDV_INT08U opt, type;
	CDV_INT32U no, par;
	CDV_INT32S num;
	RET_STATUS ret =OPT_SUCCESS;
	
	no   = *(CDV_INT32U*)(rxBuf);/*���˴���*/
	opt  = rxBuf[4];/**/
	type = rxBuf[5];
	par  = *(CDV_INT32U*)(rxBuf + 6);
  
	switch(opt) {
		case 0x01:/*����*/
			ret = WorkerControl(no, WORKER_ONCE);
			break;
		case 0x04:/*�˳�*/
			ret = WorkerControl(no, WORKER_EXIT);
			break;
		case 0x03:/*��ͣ*/
			ret = WorkerControl(no, WORKER_PAUSE);
			break;
		case 0x00:/*��ͣ*/
			ret = WorkerControl(no, WORKER_STOP);
			break;
		case 0x02:/*ѭ������*/
			ret = WorkerControl(no, WORKER_LOOP);
			break;
		case 0x05://״̬��ѯ
			switch(type) {
				case 0xED:
					ret = WorkerQueryStatus(no, (WORKER_STATUS*)&num);
				  ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&num), 4, arg);
					break;
				case 0xFE:
					ret = WorkerQueryStatus(no, (WORKER_STATUS*)&num);
					ValSet(par, num);
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	
	return ret;
}

/** @brief  ���˹�������
  * @param    
  * @retval ����ֵ˵��OPT_SUCCESS��OPT_FAILURE
  * @note   							
  */
RET_STATUS LineCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg) {
//	OS_ERR err;
//	CDV_INT08U opt/* , type , strLen*/;
//	CDV_INT32S num;
//	RET_STATUS ret =OPT_SUCCESS;
//	
//	//no   = 0;/*���˴���*/
//	opt  = rxBuf[4];/**/
//	//type = rxBuf[2];
//  
//	switch(opt) {
//		case 0x00:/*����*/
//			ret = ManagerControl(WORKER_ONCE);
//			break;
//		case 0x01:/*�˳�*/
//			ret = ManagerControl(WORKER_EXIT);
//			break;
//		case 0x02:/*��ͣ*/
//			ret = ManagerControl(WORKER_PAUSE);
//			break;
//		case 0x03:/*��ͣ*/
//			ret = ManagerControl(WORKER_STOP);
//			break;
//		case 0x04:/*ѭ������*/
//			ret = ManagerControl(WORKER_LOOP);
//			break;
//		case 0x05:/*����״̬��������*/
//			ret = ManagerQueryStatus((WORKER_STATUS*)&num);
//		  ValSet(rxBuf[3], num);
//			break;
//		default:
//			break;
//		
//	}
//	
//	return ret;
	///////////////////////////////////////////////////
	OS_ERR err;
	CDV_INT08U opt, type;
	CDV_INT32U par;
	CDV_INT32S num;
	RET_STATUS ret =OPT_SUCCESS;
	
	opt  = rxBuf[4];/**/
	type = rxBuf[5];
	par  = *(CDV_INT32U*)(rxBuf + 6);
  
	switch(opt) {
		case 0x00://״̬�л�
			switch(par) {
				case 0x01:/*����*/
					ret = ManagerControl(WORKER_ONCE);
					break;
				case 0x04:/*�˳�*/
					ret = ManagerControl(WORKER_EXIT);
					break;
				case 0x03:/*��ͣ*/
					ret = ManagerControl(WORKER_PAUSE);
					break;
				case 0x00:/*��ͣ*/
					ret = ManagerControl(WORKER_STOP);
					break;
				case 0x02:/*ѭ������*/
					ret = ManagerControl(WORKER_LOOP);
					break;
				default:
					break;
			}
			break;
		case 0x01://״̬��ѯ
			switch(type) {
				case 0xED:
					ret = ManagerQueryStatus((WORKER_STATUS*)&num);
				  ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&num), 4, arg);
					break;
				case 0xFE:
					ret = ManagerQueryStatus((WORKER_STATUS*)&num);
					ValSet(par, num);
					break;
				default:
					break;
			}
			break;
		case 0x02://����
			ResRequest(arg->buf, arg->len, (CDV_INT08U*)NULL, 0, arg);
			delay_ms(500);
		  ResetCdv();
			break;
		default:
			break;
	}
	
	return ret;
}

