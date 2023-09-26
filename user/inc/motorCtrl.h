#ifndef	__MOTOR_CTRL_H__
#define	__MOTOR_CTRL_H__

#include "pvtInt.h"

/* ��������λ�� */
typedef enum
{
	VALVE_POS_ZERO=0,  			//��ת
	VALVE_POS_WORK,				//����
	VALVE_POS_REFILL,			//עˮ
	VALVE_POS_FULL_REGEN = VALVE_POS_REFILL,
	VALVE_POS_DISSOLVE,	        //����
	VALVE_POS_BRINE,			//����
	VALVE_POS_BACKWASH,			//��ϴ
	VALVE_POS_PART_REGEN = VALVE_POS_BACKWASH,
	VALVE_POS_RINSE,			//��ϴ
	VALVE_POS_BYPASS,			//��ͨ
	VALVE_POS_UNKOWN,			//δ֪
	VALVE_POS_MAX,

	VALVE_GOTO_SET_POS=0x80,	//�ƶ�������λ��
}ValvePos_EN;

/* ����������Ʋ��� */
typedef struct 
{
	u8  ucMotorRunSta;			//�������״̬
	u8  ucMotorRunErr;			//������д���

	u8  ucCurPos;				//���嵱ǰλ��
	u8  ucDesPos;				//����Ŀ��λ��
	u16 usPosHoldTime;      	//��ǰλ�ñ���ʱ��(0:�����л�λ��)

	u8  ucPulseInterval;		//������������Cx������ʱ��
	u16 usPulseNum;				//������������Cx�ļ���ֵ(THx,TLx)
}ValveCtrl_ST;

void vMotorCtrlGpioConfig(void);

void vMotorPulseCounterConfig(void);
void vValveRun(u8 usSrcPos, u8 usDesPos);
void vValveStop(void);
void vValveMoveProc(void);
void vMotorLockRotorMonitorPolling_10ms(void);
void vValvePosSwitchProcPolling_1s(void);
void vValveCtrlParmsInit(void);

/* �����ƶ���Ŀ��λ�� */
void vValveMoveToRegenPos(u8 ucTargePos);
/* �����ƶ�����һ��λ�� */
void vValveMoveToNextPos(u8 ucTargePos);
/* ���÷���λ�ñ���ʱ�� */
void vSetValvePosHoldTime(u8 ucPos, u16 usSec);
/* �����޸�λ�ñ���ʱ�� */
void vValveChgPosHoldTime(u16 ucHoldTime);



#endif
