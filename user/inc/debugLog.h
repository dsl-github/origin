#ifndef	__DEBUG_LOG_H__
#define	__DEBUG_LOG_H__

#include "pvtInt.h"

/* 调试日志开关 */
#define DEBUG_LOG 1  //0-打印关;1-打印开

/* 选择调试串口 */
#define DEBUG_COM  2  //1-UART1;2-UART2

/* 串口收发处理参数 */
typedef void (*pUartRxCb)(u8 *,u16);
typedef struct
{ 
	u8	ucTxEnq;		//发送入队
	u8	ucTxDeq;		//发送出队
	u8  ucTxBusy;		//发送忙标志

	u8 	ucRxCnt;		//接收字节计数
	u8	ucRxTimeOut;	//接收超时
	pUartRxCb pRxCb;	//回调函数
} UartRT_ST; 

/* 调试串口配置 */
void vDebugUartConfig(pUartRxCb pRxCbFun);
/* 调试串口接收超时处理 */
void vDebugUartRxTimeout(void);

/* 调试打印 */
void vDebugLog(u8 *pucS, u32 usDigit, u8 *pucE);
/* 打印字符串 */
u8 ucDebugPrintString(u8 *pucStr);
/* 打印数字 */
void vDebugPrintDigit(u32 usDigit);
/* 打印字节 */
void vDebugPrintByte(u8 ucByte);


#endif
