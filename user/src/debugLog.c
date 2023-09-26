#include "config.h"
#include "c51Lib.h"

/* UART Init *******************************************************/
#define	UARTx_BAUDRATE		115200UL	//ѡ������
#define	TIMER2_RELOAD_VAL	(65536UL -(MAIN_FOSC / 4 / UARTx_BAUDRATE))		//Timer 2 ��װֵ�� ��Ӧ300KHZ

UartRT_ST xdata xDebugUartParm={0};
#define	DEBUG_UART_TX_LENTH	128
#define	DEBUG_UART_RX_LENTH	128
u8	xdata ucDebugUartTxBuffer[DEBUG_UART_TX_LENTH];	//���ͻ���
u8 	xdata ucDebugUartRxBuffer[DEBUG_UART_RX_LENTH];	//���ջ���

/* ���Դ������� */
void vDebugUartConfig(pUartRxCb pRxCbFun)
{
#if (DEBUG_LOG==1)

#if (DEBUG_COM==1)
	/* UART1 */
	UART1_8Bit();					//8λ����
	UART1_Use_P30P31();				//��1ʹ��P3.0 P3.1�� Ĭ��
	UART1_IRQ_Enable();				//����1�ж�
	UART1_Rx_Enable();				//����1����
	UART1_BRT_UseTimer2();          //ʹ�ö�ʱ��2��Ϊ�����ʷ�����
#else
	/* UART2 */
	UART2_8Bit();					//8λ����
	UART2_Use_P10P11();				//��2ʹ��1.0 P1.1�� Ĭ��
	UART2_IRQ_Enable();				//����2�ж�
	UART2_Rx_Enable();				//����2����
#endif
	/* �����ʷ����� */
	Timer2_Stop();					//Timer stop
	Timer2_AsTimer();				//Timer2 set As Timer
	Timer2_CLK_1T();				//Timer2 set as 1T mode
	Timer2_Load(TIMER2_RELOAD_VAL);	//װ�ض�ʱ��2��ֵ
	Timer2_Run();					//Timer run enable
	xDebugUartParm.pRxCb = pRxCbFun;
#endif
}

/* ���ط�����������������ʵ�ʷ������ж���� */
static void vDebugUartLoadByteToBuffer(u8 ucDat)
{//����̫�죬���ܳ��ֶ�������
	/* ������� */
	ucDebugUartTxBuffer[xDebugUartParm.ucTxEnq++] = ucDat;//װ������������
	xDebugUartParm.ucTxEnq %= DEBUG_UART_TX_LENTH;
	if (xDebugUartParm.ucTxBusy==FALSE)	//����
	{
		xDebugUartParm.ucTxBusy = TRUE;	//����æ
#if (DEBUG_COM==1)
		Set_UART1_TxIF();				//���������ж�
#else
		Set_UART2_TxIF();				//���������ж�
#endif
		
	}
}

/* ��ӡ�ַ��� */
u8 ucDebugPrintString(u8 *pucStr)
{
	u8 endFlg = FALSE;
	while ((pucStr != NULL)&&(*pucStr != 0))
	{	
		if (*pucStr=='\n')
		{
			endFlg = TRUE;
		}		
		vDebugUartLoadByteToBuffer(*pucStr++); 	//����ֹͣ��0����
	}
	return endFlg;
}
/* ��ӡ���� */
void vDebugPrintDigit(u32 usDigit)
{
	u8 i = 0;
	u8 str[12] = 0;//32λ���4,294,967,295
	do
	{
		str[i++] = (usDigit%10) + '0';
		usDigit /= 10;
	} while (usDigit>0);
	//�������
	do
	{
		i--;
		vDebugUartLoadByteToBuffer(str[i]);
	} while (i>0);
}
/* ��ӡ�ֽ� */
void vDebugPrintByte(u8 ucByte)
{
	vDebugUartLoadByteToBuffer(ucByte);
}

/* ���Դ�ӡ */
void vDebugLog(u8 *pucS, u32 usDigit, u8 *pucE)
{
#if (DEBUG_LOG==1)
	Set_IRQ_EA(0);
	if (ucDebugPrintString(pucS)==FALSE)
	{//�޻��з�
		vDebugPrintDigit(usDigit);
		ucDebugPrintString(pucE);
	}
	Set_IRQ_EA(1);
#endif
}

/* ���Դ��ڽ��ճ�ʱ���� */
void vDebugUartRxTimeout(void)
{
	if (xDebugUartParm.ucRxTimeOut>0)
	{//���ճ�ʱ
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

/* UART1�жϺ��� */
#if (DEBUG_COM==1)
void DebugUart_IrqHandler(void) interrupt UART1_VECTOR
{
	//��
	if (Get_UART1_RxIF() != 0)
	{
		Clr_UART1_RxIF();
		// ucDebugUartRxBuffer[xDebugUartParm.ucRxCnt++] = UART1_RdByte();
		// xDebugUartParm.ucRxCnt %= DEBUG_UART_RX_LENTH;
		// xDebugUartParm.ucRxTimeOut = 5;//������Ҫ����ʱ�жϽ�����ɣ�ͨ���ص�������������		
	}
	//��
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
	//��
	if (Get_UART2_RxIF() != 0)
	{
		Clr_UART2_RxIF();
		ucDebugUartRxBuffer[xDebugUartParm.ucRxCnt++] = UART2_RdByte();
		xDebugUartParm.ucRxCnt %= DEBUG_UART_RX_LENTH;
		xDebugUartParm.ucRxTimeOut = 3;//������Ҫ����ʱ�жϽ�����ɣ�ͨ���ص�������������		
	}
	//��
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
