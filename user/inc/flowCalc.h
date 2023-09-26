#ifndef	__FLOW_CALC_H__
#define	__FLOW_CALC_H__

#include "pvtInt.h"

#define VACATION_DAILY_WATERUSE (18)
#define VACATION_FLOWRATE (6)
/* ��ˮ��������Ϣ */
typedef struct 
{
	u32 ulTotalWaterUse;	//����ˮ����һֱ�ۼӲ�����,ÿ���ϵ�Ӵ洢���л�ȡ
	
	u32 ulSumWaterUse;		//�ۼ���ˮ��������һֱ�ۼӣ���������
	u32 ulRegenWaterUse;	//��֬�������ˮ��,ÿ����������
	u16 usAvgDailyWaterUse;	//��ƽ����ˮ��
	u16 usFlowRate;	   		//����(L/min)
	u16 usLastHourWaterUse;	//ǰһСʱ��ˮ��
	u16 usYesterdayWaterUse;//������ˮ��
	u16 usVactaionDays;     //��������
}OutletFlowInfo_ST;

extern OutletFlowInfo_ST xdata xOutletFlowInfo;

/* ��ˮ������ */
void vWaterUseInc(u16 usInc);
/* ������������������� */
void vFlowCounterConfig(void);
/* ���ټ��㣬��λL/min��ÿ���ӵ���һ�� */
void vFlowRateCalc(void);
/* ǰһСʱ��ˮ�����㣬��λL��ÿСʱ����һ�� */
void vLastHourWaterUseCalc(void);
/* ������ˮ�����㣬��λL��ÿ�����һ�� */
void vYesterdayWaterUseCalc(void);
/* ��ƽ����ˮ�����㣬��λL/day��ÿ�����һ�� */
void vAvgDailyWaterUseCalc(void);
/* Ԥ���ܹ���������ˮ�� */
u32 ulEstimateTotalRegenWaterUse(void);
/* ��������ˮ��ʹ�ñ���=��������ˮ��/��Ʋ�ˮ�� */
f32 fGetRegenWaterUseRatio(u32 ulNominalWaterYield);
/* ��������ͳ�ƣ�ÿ�����һ�� */
void vVacationDaysStatistics(void);
/* ���������жϵõ�����ʱ�� */
u16 usGetVacationDaysBasedOnFlow(void);

#endif
