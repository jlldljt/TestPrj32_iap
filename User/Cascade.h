/**
  ******************************************************************************
  * @file    /Cascade.h 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-3-29
  * @brief   
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */




#ifndef  _CASCADE_
#define  _CASCADE_

#include "cdv_include.h"

CDV_INT08U Cascade_HaveSlaveTable(void);
void CascadeCombine(CDV_INT08U no);
void CascadeLocal(CDV_INT08U no);
RET_STATUS CascadeSlaveParse (char* pBuf, CDV_INT08U len);
void CascadeTableSend(CDV_INT08U* buf, CDV_INT08U len,  CMD_ARG *arg);
CDV_INT08U CascadeGetNativeNo(void);
RET_STATUS CascadeTranferCmd(CDV_INT08U** ppBuffer, CDV_INT08U* pLen, CDV_INT08U* pCmdBuf, CDV_INT16U NumByteOfCmd);
RET_STATUS TFCmdParse (CDV_INT08U* pBuf, CDV_INT08U len);
RET_STATUS Cascade_Slave_Write(CDV_INT08U* pBuffer, CDV_INT16U NumByteToWrite);
#endif

