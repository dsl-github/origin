#include "regenProc.h"
#include "rtcTime.h"
#include "flowCalc.h"
#include "motorCtrl.h"

SystemParm_ST xdata xSysInfo={0};
WorkParm_ST /* xdata */ xWorkParm={0};

/* 系统参数初始化 */
void vSysParmInit(void)
{
	xSysInfo.usNominalWaterYield 	= 100;
	xSysInfo.ucRegenCycleDays   = 7;
	xSysInfo.ucRegenTimeHour   	= 2;
	xSysInfo.ucRegenTimeMin    	= 0;

	xSysInfo.ucMinuteRefill  	= 4;
	xSysInfo.ucMinuteDissove 	= 120;
	xSysInfo.ucMinuteBrine   	= 45;
	xSysInfo.ucMinuteBackWash	= 4;
	xSysInfo.ucMinuteRinse   	= 2;

	xSysInfo.ucRegenCycleDays_Vacation  = 4;
	xSysInfo.ucMinuteBackWash_Vacation	= 2;
	xSysInfo.ucMinuteRinse_Vacation   	= 2;

	xSysInfo.usLowSaltThreshold	= 35;
	xSysInfo.usInletHardness	= 200;

	xWorkParm.ucRegenDayCnt   		= 0;
	xWorkParm.ucVacationRegen 		= FALSE;
	xWorkParm.ucOnVacation 			= FALSE;
	xWorkParm.ucVacationRegenDayCnt = 0;
}
/* 再生事件监测，周期250ms */
void vRegenEventMonitorPolling_250ms(void)
{
	u32 tmpVal = 0;
	u8 regenSta = 0;//0-无再生；1-完整再生；2-部分再生
	if ((xSysInfo.ucRegenTimeHour==ucGetCurrTimeHour())
		&&(xSysInfo.ucRegenTimeMin==ucGetCurrTimeMin()))
	{
		/* 周期再生 */
		if (xWorkParm.ucRegenDayCnt >= xSysInfo.ucRegenCycleDays)
		{
			vRegenConditionClear();
			regenSta = 1;
		}
		/* 流量延时再生 */
		if (ulEstimateTotalRegenWaterUse() >= xSysInfo.usNominalWaterYield)
		{/* 剩余产水量不足以维持第二天用水量 */
			/* 计算实际注水时间 */
			tmpVal = MIN2SEC(xSysInfo.ucMinuteRefill)*fGetRegenWaterUseRatio(xSysInfo.usNominalWaterYield);
			vSetValvePosHoldTime(VALVE_POS_REFILL,(u16)tmpVal);
			vRegenConditionClear();
			regenSta = 1;
		}
		/* 假日模式 */
		if (xWorkParm.ucVacationRegenDayCnt >= xSysInfo.ucRegenCycleDays_Vacation)
		{
			vRegenConditionClear();
			if (xWorkParm.ucVacationRegen==FALSE)
			{//未再生过
				regenSta = 1;
			}
			else
			{//再生过，只需正反冲洗
				regenSta = 2;
			}
		}

		/* 触发再生操作 */
		if (regenSta==1)
		{
			vSetValvePosHoldTime(VALVE_POS_BACKWASH,MIN2SEC(xSysInfo.ucMinuteBackWash));
			vSetValvePosHoldTime(VALVE_POS_RINSE,MIN2SEC(xSysInfo.ucMinuteRinse));
			vValveMoveToRegenPos(VALVE_POS_FULL_REGEN);
			/* 假日中... */
			if (xWorkParm.ucOnVacation == TRUE)
			{
				xWorkParm.ucVacationRegen = TRUE;
			}
		}
		else if (regenSta==2)
		{
			vSetValvePosHoldTime(VALVE_POS_BACKWASH,MIN2SEC(xSysInfo.ucMinuteBackWash_Vacation));
			vSetValvePosHoldTime(VALVE_POS_RINSE,MIN2SEC(xSysInfo.ucMinuteRinse_Vacation));
			vValveMoveToRegenPos(VALVE_POS_PART_REGEN);
		}
	}
}

/* 再生条件清除 */
void vRegenConditionClear(void)
{
	/* 再生水量清零 */
	xOutletFlowInfo.ulRegenWaterUse = 0;
	/* 再生天数清零 */
	xWorkParm.ucRegenDayCnt = 0;
	/* 假日再生天数清零 */
	xWorkParm.ucVacationRegenDayCnt = 0;
}
/* 再生天数计数 */
void vPeriodRegenDayCount(void)
{
	xWorkParm.ucRegenDayCnt++;
	if (usGetVacationDaysBasedOnFlow()!=0)
	{
		xWorkParm.ucOnVacation = TRUE;
		xWorkParm.ucVacationRegenDayCnt++;
	}
	else
	{
		xWorkParm.ucOnVacation = FALSE;
		xWorkParm.ucVacationRegenDayCnt = 0;
		xWorkParm.ucVacationRegen = FALSE;
	}
}



