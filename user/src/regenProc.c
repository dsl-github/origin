#include "regenProc.h"
#include "rtcTime.h"
#include "flowCalc.h"
#include "motorCtrl.h"

SystemParm_ST xdata xSysInfo={0};
WorkParm_ST /* xdata */ xWorkParm={0};

/* ϵͳ������ʼ�� */
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
/* �����¼���⣬����250ms */
void vRegenEventMonitorPolling_250ms(void)
{
	u32 tmpVal = 0;
	u8 regenSta = 0;//0-��������1-����������2-��������
	if ((xSysInfo.ucRegenTimeHour==ucGetCurrTimeHour())
		&&(xSysInfo.ucRegenTimeMin==ucGetCurrTimeMin()))
	{
		/* �������� */
		if (xWorkParm.ucRegenDayCnt >= xSysInfo.ucRegenCycleDays)
		{
			vRegenConditionClear();
			regenSta = 1;
		}
		/* ������ʱ���� */
		if (ulEstimateTotalRegenWaterUse() >= xSysInfo.usNominalWaterYield)
		{/* ʣ���ˮ��������ά�ֵڶ�����ˮ�� */
			/* ����ʵ��עˮʱ�� */
			tmpVal = MIN2SEC(xSysInfo.ucMinuteRefill)*fGetRegenWaterUseRatio(xSysInfo.usNominalWaterYield);
			vSetValvePosHoldTime(VALVE_POS_REFILL,(u16)tmpVal);
			vRegenConditionClear();
			regenSta = 1;
		}
		/* ����ģʽ */
		if (xWorkParm.ucVacationRegenDayCnt >= xSysInfo.ucRegenCycleDays_Vacation)
		{
			vRegenConditionClear();
			if (xWorkParm.ucVacationRegen==FALSE)
			{//δ������
				regenSta = 1;
			}
			else
			{//��������ֻ��������ϴ
				regenSta = 2;
			}
		}

		/* ������������ */
		if (regenSta==1)
		{
			vSetValvePosHoldTime(VALVE_POS_BACKWASH,MIN2SEC(xSysInfo.ucMinuteBackWash));
			vSetValvePosHoldTime(VALVE_POS_RINSE,MIN2SEC(xSysInfo.ucMinuteRinse));
			vValveMoveToRegenPos(VALVE_POS_FULL_REGEN);
			/* ������... */
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

/* ����������� */
void vRegenConditionClear(void)
{
	/* ����ˮ������ */
	xOutletFlowInfo.ulRegenWaterUse = 0;
	/* ������������ */
	xWorkParm.ucRegenDayCnt = 0;
	/* ���������������� */
	xWorkParm.ucVacationRegenDayCnt = 0;
}
/* ������������ */
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



