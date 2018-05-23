/**
  ******************************************************************************
  * @file    /worker.h 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-3-15
  * @brief   
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _WORKER_
#define  _WORKER_

#include "cdv_include.h"

#define WORKER_MAX_NUM WORKER_NUM
/*data*/
typedef enum {                       /*��̬      ��̬*/
	WORKER_STOP,                       /*ֹͣ      ����ֹͣ*/
  WORKER_ONCE,                       /*����һ��  ����*/
	WORKER_LOOP,                       /*ѭ������  ����*/
	WORKER_PAUSE,                      /*��ͣ      ��ͣ*/
	WORKER_EXIT,                       /*�˳�      �˳�*/
	WORKER_MAX_STATUS,                 /*���״̬��ʶ*/
} WORKER_STATUS;

extern const char *WorkerStatusName[]; 

typedef struct {
	CDV_INT16U which;                  /*�ĸ�����Ҫ����*/
	CDV_INT32U startPos;               /*���˽ű���ʼλ��*/		
	CDV_INT16U cmdNum;					       /*�ű���������*/
	CDV_INT32U cmdPos;                 /*�ű��ĵ�ǰ�����flash��ʼ��ַ��������ȡflash�еĸ�������*/
	CDV_INT16U doNo;                   /*�ű�������ţ����ۼӣ�����ű��д洢����Ž��жԱȼ��*/	
	CDV_INT16U totalLen;               /*�ű��ܳ���*/
	CDV_INT08U *buf;                   /*���˽ű�����ָ�룬һ��*/
	CDV_INT08U len;                    /*���˽ű����泤�ȣ�һ��*/
} DEBUG_SCRIPT;

typedef struct {
	CDV_INT08U which;                  /*�ĸ�����ռ�ø��߳�*/
	CDV_INT16U step;                   /*ָ������λ��*/
	CDV_INT16U exitStep;               /*�˳�ָ������λ��*/
	WORKER_STATUS status;              /*�ĸ�״̬*/
	CDV_INT08U uartNo;                 /*ռ�ô���*/
} THREAD_INFO;

extern THREAD_INFO g_threadInfo[WORKER_MAX_NUM];
/*function*/
BOOL IsTaskExist(const OS_TCB tcb) ;
void WorkerTask(void *p_arg);
RET_STATUS ManagerControl(const WORKER_STATUS status);
RET_STATUS ManagerQueryStatus(WORKER_STATUS* pStatus);
RET_STATUS WorkerControl(const CDV_INT08U no, const WORKER_STATUS status);
RET_STATUS WorkerExitSet(CDV_INT08U no, CDV_INT32U step);
RET_STATUS WorkerQuery(const CDV_INT08U no, WORKER_STATUS *pStatus, CDV_INT32U* pStep);
RET_STATUS WorkerQueryStatus(const CDV_INT08U no, WORKER_STATUS* pStatus);
RET_STATUS WorkerQueryStep(const CDV_INT08U no, CDV_INT16U* pStep);
RET_STATUS WorkerCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen , CMD_ARG *arg);
RET_STATUS LineCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg);

RET_STATUS ToWorkerCmd(DEBUG_SCRIPT *debugWorker , CDV_INT16U cmdNo) ;

void AllWorkerCtrl(const WORKER_STATUS status);
#endif
