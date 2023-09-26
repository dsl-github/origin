#include "config.h"
#include "flowCalc.h"

OutletFlowInfo_ST xdata xOutletFlowInfo={0};

/* ������������������� */
void vFlowCounterConfig(void)
{
	Timer1_16bitAutoReload();
	Timer1_AsCounter();
	Timer1_CLKOUT_P34_Disable();
	Timer1_Load(65536UL - 30);//30������1Lˮ
	Timer1_IRQ_Enable();
	Timer1_Run();
}

/* ��ˮ������ */
void vWaterUseInc(u16 usInc)
{
	xOutletFlowInfo.ulTotalWaterUse += usInc;
	xOutletFlowInfo.ulSumWaterUse += usInc;
	xOutletFlowInfo.ulRegenWaterUse += usInc;
}

/* Timer1�жϺ��� */
void FlowCounter_IrqHandler(void) interrupt TIMER1_VECTOR
{
	xOutletFlowInfo.ulTotalWaterUse ++;
	xOutletFlowInfo.ulSumWaterUse ++;
	xOutletFlowInfo.ulRegenWaterUse ++;
}

/* ���ټ��㣬��λL/min��ÿ���ӵ���һ�� */
void vFlowRateCalc(void)
{
	static u32 xdata oneMinuteAgoVal = 0;

	if (xOutletFlowInfo.ulSumWaterUse>oneMinuteAgoVal)
	{
		xOutletFlowInfo.usFlowRate = xOutletFlowInfo.ulSumWaterUse-oneMinuteAgoVal;
	}
	else
	{
		xOutletFlowInfo.usFlowRate = 0;
	}
	oneMinuteAgoVal = xOutletFlowInfo.ulSumWaterUse;
}

/* ǰһСʱ��ˮ�����㣬��λL��ÿСʱ����һ�� */
void vLastHourWaterUseCalc(void)
{
	static u32 xdata oneHourAgoVal = 0;

	if (xOutletFlowInfo.ulSumWaterUse>oneHourAgoVal)
	{
		xOutletFlowInfo.usLastHourWaterUse = xOutletFlowInfo.ulSumWaterUse-oneHourAgoVal;
	}
	else
	{
		xOutletFlowInfo.usLastHourWaterUse = 0;
	}
	oneHourAgoVal = xOutletFlowInfo.ulSumWaterUse;
}

/* ������ˮ�����㣬��λL��ÿ�����һ�� */
void vYesterdayWaterUseCalc(void)
{
	static u32 xdata oneDayAgoVal = 0;

	if (xOutletFlowInfo.ulSumWaterUse>oneDayAgoVal)
	{
		xOutletFlowInfo.usYesterdayWaterUse = xOutletFlowInfo.ulSumWaterUse-oneDayAgoVal;
	}
	else
	{
		xOutletFlowInfo.usYesterdayWaterUse = 0;
	}
	oneDayAgoVal = xOutletFlowInfo.ulSumWaterUse;
}

/* ��ƽ����ˮ�����㣬��λL/day��ÿ�����һ�� */
void vAvgDailyWaterUseCalc(void)
{
	static u32 xdata waterQueue[8] = {0};
	static u8 xdata enqIdx = 0;
	static u8 xdata dayCnt = 0;

	waterQueue[enqIdx] = xOutletFlowInfo.ulSumWaterUse;	
	if (dayCnt==0)
	{
		dayCnt++;
	}
	else if (dayCnt<=6)
	{
		xOutletFlowInfo.usAvgDailyWaterUse = (waterQueue[dayCnt]-waterQueue[0])/dayCnt;
		dayCnt++;
	}
	else
	{
		xOutletFlowInfo.usAvgDailyWaterUse = (waterQueue[enqIdx]-waterQueue[(enqIdx+1)%8])/dayCnt;
	}
	enqIdx = (++enqIdx)%8;
}
/* Ԥ���ܹ���������ˮ�� */
u32 ulEstimateTotalRegenWaterUse(void)
{//��������ˮ��������ƽ����ˮ������������յ���������ˮ��
	return (xOutletFlowInfo.ulRegenWaterUse+xOutletFlowInfo.usAvgDailyWaterUse);
}
/* ��������ˮ��ʹ�ñ���=��������ˮ��/��Ʋ�ˮ�� */
f32 fGetRegenWaterUseRatio(u32 ulNominalWaterYield)
{
	return ((f32)xOutletFlowInfo.ulRegenWaterUse/(f32)ulNominalWaterYield);
}

/* ��������ͳ�ƣ�ÿ�����һ�� */
void vVacationDaysStatistics(void)
{
	if ((xOutletFlowInfo.usYesterdayWaterUse>=VACATION_DAILY_WATERUSE)
		||(xOutletFlowInfo.usFlowRate>=VACATION_FLOWRATE))
	{//������ˮ��>=18L����˲ʱ����>=6L/min
		xOutletFlowInfo.usVactaionDays = 0;
	}
	else if (xOutletFlowInfo.usYesterdayWaterUse<VACATION_DAILY_WATERUSE)
	{//������ˮ��<18L
		xOutletFlowInfo.usVactaionDays++;
	}
}
/* ���������жϵõ�����ʱ�� */
u16 usGetVacationDaysBasedOnFlow(void)
{
	return xOutletFlowInfo.usVactaionDays;
}



