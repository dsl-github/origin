#ifndef	__MOTOR_CTRL_H__
#define	__MOTOR_CTRL_H__

#include "pvtInt.h"

/* 阀体流道位置 */
typedef enum
{
	VALVE_POS_ZERO=0,  			//堵转
	VALVE_POS_WORK,				//工作
	VALVE_POS_REFILL,			//注水
	VALVE_POS_FULL_REGEN = VALVE_POS_REFILL,
	VALVE_POS_DISSOLVE,	        //溶盐
	VALVE_POS_BRINE,			//吸盐
	VALVE_POS_BACKWASH,			//反洗
	VALVE_POS_PART_REGEN = VALVE_POS_BACKWASH,
	VALVE_POS_RINSE,			//正洗
	VALVE_POS_BYPASS,			//旁通
	VALVE_POS_UNKOWN,			//未知
	VALVE_POS_MAX,

	VALVE_GOTO_SET_POS=0x80,	//移动至设置位置
}ValvePos_EN;

/* 阀体活塞控制参数 */
typedef struct 
{
	u8  ucMotorRunSta;			//电机运行状态
	u8  ucMotorRunErr;			//电机运行错误

	u8  ucCurPos;				//阀体当前位置
	u8  ucDesPos;				//阀体目标位置
	u16 usPosHoldTime;      	//当前位置保持时间(0:立即切换位置)

	u8  ucPulseInterval;		//处理器计数器Cx脉冲间隔时间
	u16 usPulseNum;				//处理器计数器Cx的计数值(THx,TLx)
}ValveCtrl_ST;

void vMotorCtrlGpioConfig(void);

void vMotorPulseCounterConfig(void);
void vValveRun(u8 usSrcPos, u8 usDesPos);
void vValveStop(void);
void vValveMoveProc(void);
void vMotorLockRotorMonitorPolling_10ms(void);
void vValvePosSwitchProcPolling_1s(void);
void vValveCtrlParmsInit(void);

/* 阀体移动至目标位置 */
void vValveMoveToRegenPos(u8 ucTargePos);
/* 阀体移动至下一个位置 */
void vValveMoveToNextPos(u8 ucTargePos);
/* 设置阀体位置保持时间 */
void vSetValvePosHoldTime(u8 ucPos, u16 usSec);
/* 阀体修改位置保持时间 */
void vValveChgPosHoldTime(u16 ucHoldTime);



#endif
