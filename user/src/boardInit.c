#include "boardInit.h"

/* GPIO Init *******************************************************/

/* Timer Init *******************************************************/


u8 xdata ucSysTick10msFlg = FALSE;
u8 ucGetSysTick10msFlg(void)
{
	return ucSysTick10msFlg;
}

void vSetSysTick10msFlg(u8 sta)
{
	ucSysTick10msFlg = sta;
}

/* PCA Init *******************************************************/
/* PCA定时器重载初值 */
#define PCA_TIMER_RELOAD (65536UL - (MAIN_FOSC / (12*100)))//100Hz(10ms)
#define SALT_PWD_nOPEN(n)   	P22 = (n)
/* 低盐传感器周期1s */
void vPCA_Init(void)
{
	/* PCA定时器初始化,溢出周期10ms */
	PCA_Timer_CLK_12T();
	PCA_Use_P24P25P26P27();
	PCA_Timer_IRQ_Enable();
	PCA_SetHighPolity();
	Set_PCA_TimerVal(PCA_TIMER_RELOAD);

	/* PCA0捕获功能配置 */
	PCA0_Capture_Rise();
	
	PCA_Timer_Run();

	P2_OUT_OD_Set(GPIO_Px2);//SALT_PWR
	SALT_PWD_nOPEN(0);
}

u16 xdata ulSaltSigFallCaptureVal;  	/* 下降沿捕获值 */
u16 xdata ulSaltSigRiseCaptureVal;		/* 上升沿捕获值 */
u16 xdata ulSaltSigOverflowCnt;			/* 溢出信号个数 */
u8  xdata ucSaltSigDetRiseEdge = FALSE;	/* 检测到上升沿 */

u8 xdata ucSaltRet = 0;
/* PCA中断函数 */
void PCA_IrqHandler(void) interrupt PCA_VECTOR
{
	u32 ret = 0;
	if(Get_PCA_Timer_CIF())	//PCA溢出中断
	{
		Clr_PCA_Timer_CIF();//清PCA溢出中断标志
		Set_PCA_TimerVal(PCA_TIMER_RELOAD);//重载初值
		vSetSysTick10msFlg(TRUE);

		ulSaltSigOverflowCnt++;
	}

	if(Get_PCA0_CCIF())	//PCA0中断
	{
		Clr_PCA0_CCIF();//清PCA0中断标志
		if (PCA0_IsCaptureRise())
		{
			ulSaltSigOverflowCnt 	= 0;
			ulSaltSigRiseCaptureVal = Get_PCA0_CaptureVal();	
			ucSaltSigDetRiseEdge 	= TRUE;
			/* 切换下降沿检测 */
			PCA0_Capture_Fall();
		}
		else if (PCA0_IsCaptureFall())
		{
			if (ucSaltSigDetRiseEdge)
			{
				ulSaltSigFallCaptureVal = Get_PCA0_CaptureVal();
				/* 由于低盐传感器高电平时间超出PCA最大溢出时间，需要同时借助溢出中断和捕捞中断计算高电平时长
									_____________________________________________
                低盐传感器信号______|                                             |________________

							   CF       CF       CF       CF       CF       CF       CF
				PCA定时器		|————————|————————|————————|————————|————————|————————|     溢出周期10ms
				PCA0捕捞	       |——————————————————————————————————————————————|
                                  CCF0                                          CCF0

				时间统计说明        |—————此段时间=65535-上升沿捕获值
								         ————————————————————————————————————此段时间=10ms*(溢出中断个数-1)	
										                                     ————|此段时间=下降沿捕获值-定时器初值   
				 */
				ret = 65536UL - (u32)ulSaltSigRiseCaptureVal + (u32)ulSaltSigFallCaptureVal - PCA_TIMER_RELOAD;
				ret = ret*1000/(MAIN_FOSC/12);
				if (ulSaltSigOverflowCnt>=1)
				{
					ret += (ulSaltSigOverflowCnt-1)*10;
				}
				/* 低盐传感器分辨率1cm/ms,距离为0时输出5ms */
				ucSaltRet = (ret > 5) ? (ret-5) : 0;
				// vDebugLog("Salt Dis=",ret,"\n");
				ucSaltSigDetRiseEdge = FALSE;
				PCA0_Capture_Rise();//切换上降沿检测	
			}
		}	
	}
}


/* Ext Init *******************************************************/
void vExt0Confit(void)
{
	EXT0_Mode_RiseFall();
	EXT0_IRQ_Enable();
}

/* INT0中断函数 */
void Ext0_IrqHandler (void) interrupt INT0_VECTOR		//进中断时已经清除标志
{
	Timer0_Stop();
}








