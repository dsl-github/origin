#ifndef	__REGEN_PROC_H__
#define	__REGEN_PROC_H__

#include "pvtInt.h"

#define MIN2SEC(n)	((u16)(n)*60)

typedef struct 
{
	u16 usNominalWaterYield;//��Ʋ�ˮ��
	u8 ucRegenCycleDays;    //�����������
	u8 ucRegenTimeHour;		//����ʱ���-Сʱ
	u8 ucRegenTimeMin;		//����ʱ���-����
	u8 ucMinuteRefill;		//עˮ����ʱ��
	u8 ucMinuteDissove;		//���γ���ʱ��
	u8 ucMinuteBrine;		//���γ���ʱ��
	u8 ucMinuteBackWash;	//��ϴ����ʱ��
	u8 ucMinuteRinse;		//��ϴ����ʱ��

	u8 ucRegenCycleDays_Vacation;	//�����������(����)
	u8 ucMinuteBackWash_Vacation;	//��ϴ����ʱ��(����)
	u8 ucMinuteRinse_Vacation;		//��ϴ����ʱ��(����)
	u16 usInletHardness;	//��ˮӲ��
	u16 usLowSaltThreshold; //���η�ֵ
}SystemParm_ST;


typedef struct 
{
    u8  ucRegenDayCnt;			//������������

	u8  ucVacationRegen; 		//��������������־
	u8  ucOnVacation; 			//�����б�־
	u8  ucVacationRegenDayCnt;  //����������������
}WorkParm_ST;

extern SystemParm_ST xdata xSysInfo;
extern WorkParm_ST /* xdata */ xWorkParm;

/* ϵͳ������ʼ�� */
void vSysParmInit(void);
/* �����¼���⣬����250ms */
void vRegenEventMonitorPolling_250ms(void);
/* ����������� */
void vRegenConditionClear(void);
/* ������������ */
void vPeriodRegenDayCount(void);
#endif
