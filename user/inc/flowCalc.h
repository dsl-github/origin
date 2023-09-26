#ifndef	__FLOW_CALC_H__
#define	__FLOW_CALC_H__

#include "pvtInt.h"

#define VACATION_DAILY_WATERUSE (18)
#define VACATION_FLOWRATE (6)
/* 出水口流量信息 */
typedef struct 
{
	u32 ulTotalWaterUse;	//总用水量，一直累加不清零,每次上电从存储器中获取
	
	u32 ulSumWaterUse;		//累计用水量，开机一直累加，重启清零
	u32 ulRegenWaterUse;	//树脂再生后产水量,每次再生清零
	u16 usAvgDailyWaterUse;	//日平均用水量
	u16 usFlowRate;	   		//流速(L/min)
	u16 usLastHourWaterUse;	//前一小时用水量
	u16 usYesterdayWaterUse;//昨天用水量
	u16 usVactaionDays;     //假日天数
}OutletFlowInfo_ST;

extern OutletFlowInfo_ST xdata xOutletFlowInfo;

/* 用水量增加 */
void vWaterUseInc(u16 usInc);
/* 流量计脉冲计数器配置 */
void vFlowCounterConfig(void);
/* 流速计算，单位L/min，每分钟调用一次 */
void vFlowRateCalc(void);
/* 前一小时用水量计算，单位L，每小时调用一次 */
void vLastHourWaterUseCalc(void);
/* 昨日用水量计算，单位L，每天调用一次 */
void vYesterdayWaterUseCalc(void);
/* 日平均用水量计算，单位L/day，每天调用一次 */
void vAvgDailyWaterUseCalc(void);
/* 预估总共再生后用水量 */
u32 ulEstimateTotalRegenWaterUse(void);
/* 再生后用水量使用比例=再生后用水量/标称产水量 */
f32 fGetRegenWaterUseRatio(u32 ulNominalWaterYield);
/* 假日天数统计，每天调用一次 */
void vVacationDaysStatistics(void);
/* 基于流量判断得到假日时长 */
u16 usGetVacationDaysBasedOnFlow(void);

#endif
