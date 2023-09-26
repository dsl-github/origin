#include "config.h"
#include "c51Lib.h"

/* UART Init *******************************************************/
#define	UARTx_BAUDRATE		115200UL	//选择波特率
#define	TIMER2_RELOAD_VAL	(65536UL -(MAIN_FOSC / 4 / UARTx_BAUDRATE))		//Timer 2 重装值， 对应300KHZ

UartRT_ST xdata xDebugUartParm={0};
#define	DEBUG_UART_TX_LENTH	128
#define	DEBUG_UART_RX_LENTH	128
u8	xdata ucDebugUartTxBuffer[DEBUG_UART_TX_LENTH];	//发送缓冲
u8 	xdata ucDebugUartRxBuffer[DEBUG_UART_RX_LENTH];	//接收缓冲

/* 调试串口配置 */
void vDebugUartConfig(pUartRxCb pRxCbFun)
{
#if (DEBUG_LOG==1)

#if (DEBUG_COM==1)
	/* UART1 */
	UART1_8Bit();					//8位数据
	UART1_Use_P30P31();				//串1使用P3.0 P3.1口 默认
	UART1_IRQ_Enable();				//允许串1中断
	UART1_Rx_Enable();				//允许串1接收
	UART1_BRT_UseTimer2();          //使用定时器2作为波特率发生器
#else
	/* UART2 */
	UART2_8Bit();					//8位数据
	UART2_Use_P10P11();				//串2使用1.0 P1.1口 默认
	UART2_IRQ_Enable();				//允许串2中断
	UART2_Rx_Enable();				//允许串2接收
#endif
	/* 波特率发生器 */
	Timer2_Stop();					//Timer stop
	Timer2_AsTimer();				//Timer2 set As Timer
	Timer2_CLK_1T();				//Timer2 set as 1T mode
	Timer2_Load(TIMER2_RELOAD_VAL);	//装载定时器2初值
	Timer2_Run();					//Timer run enable
	xDebugUartParm.pRxCb = pRxCbFun;
#endif
}

/* 加载发送数据至缓冲区，实际发送由中断完成 */
static void vDebugUartLoadByteToBuffer(u8 ucDat)
{//加载太快，可能出现丢包现象
	/* 数据入队 */
	ucDebugUartTxBuffer[xDebugUartParm.ucTxEnq++] = ucDat;//装载数据至缓冲
	xDebugUartParm.ucTxEnq %= DEBUG_UART_TX_LENTH;
	if (xDebugUartParm.ucTxBusy==FALSE)	//空闲
	{
		xDebugUartParm.ucTxBusy = TRUE;	//发送忙
#if (DEBUG_COM==1)
		Set_UART1_TxIF();				//触发发送中断
#else
		Set_UART2_TxIF();				//触发发送中断
#endif
		
	}
}

/* 打印字符串 */
u8 ucDebugPrintString(u8 *pucStr)
{
	u8 endFlg = FALSE;
	while ((pucStr != NULL)&&(*pucStr != 0))
	{	
		if (*pucStr=='\n')
		{
			endFlg = TRUE;
		}		
		vDebugUartLoadByteToBuffer(*pucStr++); 	//遇到停止符0结束
	}
	return endFlg;
}
/* 打印数字 */
void vDebugPrintDigit(u32 usDigit)
{
	u8 i = 0;
	u8 str[12] = 0;//32位最大4,294,967,295
	do
	{
		str[i++] = (usDigit%10) + '0';
		usDigit /= 10;
	} while (usDigit>0);
	//反向输出
	do
	{
		i--;
		vDebugUartLoadByteToBuffer(str[i]);
	} while (i>0);
}
/* 打印字节 */
void vDebugPrintByte(u8 ucByte)
{
	vDebugUartLoadByteToBuffer(ucByte);
}

/* 调试打印 */
void vDebugLog(u8 *pucS, u32 usDigit, u8 *pucE)
{
#if (DEBUG_LOG==1)
	Set_IRQ_EA(0);
	if (ucDebugPrintString(pucS)==FALSE)
	{//无换行符
		vDebugPrintDigit(usDigit);
		ucDebugPrintString(pucE);
	}
	Set_IRQ_EA(1);
#endif
}

/* 调试串口接收超时处理 */
void vDebugUartRxTimeout(void)
{
	if (xDebugUartParm.ucRxTimeOut>0)
	{//接收超时
		if ((--xDebugUartParm.ucRxTimeOut)==0)
		{
			if (xDebugUartParm.pRxCb!=NULL)
			{
				xDebugUartParm.pRxCb(ucDebugUartRxBuffer,xDebugUartParm.ucRxCnt);
				xDebugUartParm.ucRxCnt = 0;
			}
		}
	}
}

/* UART1中断函数 */
#if (DEBUG_COM==1)
void DebugUart_IrqHandler(void) interrupt UART1_VECTOR
{
	//收
	if (Get_UART1_RxIF() != 0)
	{
		Clr_UART1_RxIF();
		// ucDebugUartRxBuffer[xDebugUartParm.ucRxCnt++] = UART1_RdByte();
		// xDebugUartParm.ucRxCnt %= DEBUG_UART_RX_LENTH;
		// xDebugUartParm.ucRxTimeOut = 5;//后续需要做超时判断接收完成，通过回调函数解析数据		
	}
	//发
	if(Get_UART1_TxIF() != 0)
	{
		Clr_UART1_TxIF();
		if(xDebugUartParm.ucTxDeq != xDebugUartParm.ucTxEnq)
		{
		 	UART1_WrByte(ucDebugUartTxBuffer[xDebugUartParm.ucTxDeq++]); 
			xDebugUartParm.ucTxDeq %= DEBUG_UART_TX_LENTH;
		}
		else	
		{
			xDebugUartParm.ucTxBusy = FALSE;
		}
	}
}
#else
void DebugUart_IrqHandler(void) interrupt UART2_VECTOR
{
	//收
	if (Get_UART2_RxIF() != 0)
	{
		Clr_UART2_RxIF();
		ucDebugUartRxBuffer[xDebugUartParm.ucRxCnt++] = UART2_RdByte();
		xDebugUartParm.ucRxCnt %= DEBUG_UART_RX_LENTH;
		xDebugUartParm.ucRxTimeOut = 3;//后续需要做超时判断接收完成，通过回调函数解析数据		
	}
	//发
	if(Get_UART2_TxIF() != 0)
	{
		Clr_UART2_TxIF();
		if(xDebugUartParm.ucTxDeq != xDebugUartParm.ucTxEnq)
		{
		 	UART2_WrByte(ucDebugUartTxBuffer[xDebugUartParm.ucTxDeq++]); 
			xDebugUartParm.ucTxDeq %= DEBUG_UART_TX_LENTH;
		}
		else	
		{
			xDebugUartParm.ucTxBusy = FALSE;
		}
	}
}
#endif
