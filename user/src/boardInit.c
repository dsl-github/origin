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
/* PCA��ʱ�����س�ֵ */
#define PCA_TIMER_RELOAD (65536UL - (MAIN_FOSC / (12*100)))//100Hz(10ms)
#define SALT_PWD_nOPEN(n)   	P22 = (n)
/* ���δ���������1s */
void vPCA_Init(void)
{
	/* PCA��ʱ����ʼ��,�������10ms */
	PCA_Timer_CLK_12T();
	PCA_Use_P24P25P26P27();
	PCA_Timer_IRQ_Enable();
	PCA_SetHighPolity();
	Set_PCA_TimerVal(PCA_TIMER_RELOAD);

	/* PCA0���������� */
	PCA0_Capture_Rise();
	
	PCA_Timer_Run();

	P2_OUT_OD_Set(GPIO_Px2);//SALT_PWR
	SALT_PWD_nOPEN(0);
}

u16 xdata ulSaltSigFallCaptureVal;  	/* �½��ز���ֵ */
u16 xdata ulSaltSigRiseCaptureVal;		/* �����ز���ֵ */
u16 xdata ulSaltSigOverflowCnt;			/* ����źŸ��� */
u8  xdata ucSaltSigDetRiseEdge = FALSE;	/* ��⵽������ */

u8 xdata ucSaltRet = 0;
/* PCA�жϺ��� */
void PCA_IrqHandler(void) interrupt PCA_VECTOR
{
	u32 ret = 0;
	if(Get_PCA_Timer_CIF())	//PCA����ж�
	{
		Clr_PCA_Timer_CIF();//��PCA����жϱ�־
		Set_PCA_TimerVal(PCA_TIMER_RELOAD);//���س�ֵ
		vSetSysTick10msFlg(TRUE);

		ulSaltSigOverflowCnt++;
	}

	if(Get_PCA0_CCIF())	//PCA0�ж�
	{
		Clr_PCA0_CCIF();//��PCA0�жϱ�־
		if (PCA0_IsCaptureRise())
		{
			ulSaltSigOverflowCnt 	= 0;
			ulSaltSigRiseCaptureVal = Get_PCA0_CaptureVal();	
			ucSaltSigDetRiseEdge 	= TRUE;
			/* �л��½��ؼ�� */
			PCA0_Capture_Fall();
		}
		else if (PCA0_IsCaptureFall())
		{
			if (ucSaltSigDetRiseEdge)
			{
				ulSaltSigFallCaptureVal = Get_PCA0_CaptureVal();
				/* ���ڵ��δ������ߵ�ƽʱ�䳬��PCA������ʱ�䣬��Ҫͬʱ��������жϺͲ����жϼ���ߵ�ƽʱ��
									_____________________________________________
                ���δ������ź�______|                                             |________________

							   CF       CF       CF       CF       CF       CF       CF
				PCA��ʱ��		|����������������|����������������|����������������|����������������|����������������|����������������|     �������10ms
				PCA0����	       |��������������������������������������������������������������������������������������������|
                                  CCF0                                          CCF0

				ʱ��ͳ��˵��        |�����������˶�ʱ��=65535-�����ز���ֵ
								         �������������������������������������������������������������������������˶�ʱ��=10ms*(����жϸ���-1)	
										                                     ��������|�˶�ʱ��=�½��ز���ֵ-��ʱ����ֵ   
				 */
				ret = 65536UL - (u32)ulSaltSigRiseCaptureVal + (u32)ulSaltSigFallCaptureVal - PCA_TIMER_RELOAD;
				ret = ret*1000/(MAIN_FOSC/12);
				if (ulSaltSigOverflowCnt>=1)
				{
					ret += (ulSaltSigOverflowCnt-1)*10;
				}
				/* ���δ������ֱ���1cm/ms,����Ϊ0ʱ���5ms */
				ucSaltRet = (ret > 5) ? (ret-5) : 0;
				// vDebugLog("Salt Dis=",ret,"\n");
				ucSaltSigDetRiseEdge = FALSE;
				PCA0_Capture_Rise();//�л��Ͻ��ؼ��	
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

/* INT0�жϺ��� */
void Ext0_IrqHandler (void) interrupt INT0_VECTOR		//���ж�ʱ�Ѿ������־
{
	Timer0_Stop();
}








