#ifndef	__REGEN_PROC_H__
#define	__REGEN_PROC_H__

#include "pvtInt.h"

#define MIN2SEC(n)	((u16)(n)*60)

typedef struct 
{
	u16 usNominalWaterYield;//标称产水量
	u8 ucRegenCycleDays;    //再生间隔天数
	u8 ucRegenTimeHour;		//再生时间点-小时
	u8 ucRegenTimeMin;		//再生时间点-分钟
	u8 ucMinuteRefill;		//注水持续时间
	u8 ucMinuteDissove;		//溶盐持续时间
	u8 ucMinuteBrine;		//吸盐持续时间
	u8 ucMinuteBackWash;	//反洗持续时间
	u8 ucMinuteRinse;		//正洗持续时间

	u8 ucRegenCycleDays_Vacation;	//再生间隔天数(假日)
	u8 ucMinuteBackWash_Vacation;	//反洗持续时间(假日)
	u8 ucMinuteRinse_Vacation;		//正洗持续时间(假日)
	u16 usInletHardness;	//进水硬度
	u16 usLowSaltThreshold; //低盐阀值
}SystemParm_ST;


typedef struct 
{
    u8  ucRegenDayCnt;			//再生天数计数

	u8  ucVacationRegen; 		//假日完整再生标志
	u8  ucOnVacation; 			//假日中标志
	u8  ucVacationRegenDayCnt;  //假日再生天数计数
}WorkParm_ST;

extern SystemParm_ST xdata xSysInfo;
extern WorkParm_ST /* xdata */ xWorkParm;

/* 系统参数初始化 */
void vSysParmInit(void);
/* 再生事件监测，周期250ms */
void vRegenEventMonitorPolling_250ms(void);
/* 再生条件清除 */
void vRegenConditionClear(void);
/* 再生天数计数 */
void vPeriodRegenDayCount(void);
#endif
