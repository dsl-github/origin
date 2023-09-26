#include "config.h"
#include "flowCalc.h"

OutletFlowInfo_ST xdata xOutletFlowInfo={0};

/* 流量计脉冲计数器配置 */
void vFlowCounterConfig(void)
{
	Timer1_16bitAutoReload();
	Timer1_AsCounter();
	Timer1_CLKOUT_P34_Disable();
	Timer1_Load(65536UL - 30);//30个脉冲1L水
	Timer1_IRQ_Enable();
	Timer1_Run();
}

/* 用水量增加 */
void vWaterUseInc(u16 usInc)
{
	xOutletFlowInfo.ulTotalWaterUse += usInc;
	xOutletFlowInfo.ulSumWaterUse += usInc;
	xOutletFlowInfo.ulRegenWaterUse += usInc;
}

/* Timer1中断函数 */
void FlowCounter_IrqHandler(void) interrupt TIMER1_VECTOR
{
	xOutletFlowInfo.ulTotalWaterUse ++;
	xOutletFlowInfo.ulSumWaterUse ++;
	xOutletFlowInfo.ulRegenWaterUse ++;
}

/* 流速计算，单位L/min，每分钟调用一次 */
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

/* 前一小时用水量计算，单位L，每小时调用一次 */
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

/* 昨日用水量计算，单位L，每天调用一次 */
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

/* 日平均用水量计算，单位L/day，每天调用一次 */
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
/* 预估总共再生后用水量 */
u32 ulEstimateTotalRegenWaterUse(void)
{//再生后用水量加上日平均用水量，估算出今日的再生后用水量
	return (xOutletFlowInfo.ulRegenWaterUse+xOutletFlowInfo.usAvgDailyWaterUse);
}
/* 再生后用水量使用比例=再生后用水量/标称产水量 */
f32 fGetRegenWaterUseRatio(u32 ulNominalWaterYield)
{
	return ((f32)xOutletFlowInfo.ulRegenWaterUse/(f32)ulNominalWaterYield);
}

/* 假日天数统计，每天调用一次 */
void vVacationDaysStatistics(void)
{
	if ((xOutletFlowInfo.usYesterdayWaterUse>=VACATION_DAILY_WATERUSE)
		||(xOutletFlowInfo.usFlowRate>=VACATION_FLOWRATE))
	{//昨日用水量>=18L或者瞬时流量>=6L/min
		xOutletFlowInfo.usVactaionDays = 0;
	}
	else if (xOutletFlowInfo.usYesterdayWaterUse<VACATION_DAILY_WATERUSE)
	{//昨日用水量<18L
		xOutletFlowInfo.usVactaionDays++;
	}
}
/* 基于流量判断得到假日时长 */
u16 usGetVacationDaysBasedOnFlow(void)
{
	return xOutletFlowInfo.usVactaionDays;
}



