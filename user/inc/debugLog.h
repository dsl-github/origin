#ifndef	__DEBUG_LOG_H__
#define	__DEBUG_LOG_H__

#include "pvtInt.h"

/* ������־���� */
#define DEBUG_LOG 1  //0-��ӡ��;1-��ӡ��

/* ѡ����Դ��� */
#define DEBUG_COM  2  //1-UART1;2-UART2

/* �����շ�������� */
typedef void (*pUartRxCb)(u8 *,u16);
typedef struct
{ 
	u8	ucTxEnq;		//�������
	u8	ucTxDeq;		//���ͳ���
	u8  ucTxBusy;		//����æ��־

	u8 	ucRxCnt;		//�����ֽڼ���
	u8	ucRxTimeOut;	//���ճ�ʱ
	pUartRxCb pRxCb;	//�ص�����
} UartRT_ST; 

/* ���Դ������� */
void vDebugUartConfig(pUartRxCb pRxCbFun);
/* ���Դ��ڽ��ճ�ʱ���� */
void vDebugUartRxTimeout(void);

/* ���Դ�ӡ */
void vDebugLog(u8 *pucS, u32 usDigit, u8 *pucE);
/* ��ӡ�ַ��� */
u8 ucDebugPrintString(u8 *pucStr);
/* ��ӡ���� */
void vDebugPrintDigit(u32 usDigit);
/* ��ӡ�ֽ� */
void vDebugPrintByte(u8 ucByte);


#endif
