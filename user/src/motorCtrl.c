#include "config.h"
#include "motorCtrl.h"
#include "regenProc.h"
#include "debugLog.h"

/* 当前未使用PWM情况下，电机全速转动，大约85ms一个光电脉冲 */
#define PULSE_PERIOD_MS      (85)
/* 电机状态 */
typedef enum
{
	MOTOR_STOP=0,  	//停止
	MOTOR_RUN_POS,	//正转
	MOTOR_RUN_NEG	//反转
}MotorWorkSta_EN;

ValveCtrl_ST xdata xMotorCtrlPara={0,0,0,0};


/*  正转:目标位置脉冲数大于源位置脉冲数                                       										      */
/*  反转:目标位置脉冲数小于源位置脉冲数                                                                                    */
/* 脉冲数                                      堵转     工作     注水     溶盐     吸盐    反洗     正洗      旁通     未知*/
u16 code usWaterWayPlus[VALVE_POS_MAX]     = {0,      10,      50,       10,    150,    200,     250,     300,     0xFFF0};   
/* 保持时间(秒) */
u16 xdata usWaterWayDuration[VALVE_POS_MAX]= {0,       0,    4*60,   120*60,  45*60,   5*60,    5*60,       0,          0};   
/* 下一个位置 */
// u16 xdata usWaterWayNextPos[VALVE_POS_MAX] =  {0,       0,    4*60,   120*60,  45*60,   5*60,    5*60,       0,          0};   

/* Motor H桥控制IO *******************************************************/
#define MOTOR_DRV_nSLEEP(n)   	P17 = (n)
#define MOTOR_DRV_AIN1(n)    	P54 = (n)
#define MOTOR_DRV_AIN2(n)    	P55 = (n)
#define ENCODER_PWD_nOPEN(n)   	P40 = (n)

/* 电机控制IO初始化 */
void vMotorCtrlGpioConfig(void)
{
	/* 电机控制 */
	P1_OUT_PP_Set(GPIO_Px7);//DRV8848_nSLEEP
	P5_OUT_PP_Set(GPIO_Px4);//DRV8848_AIN1
	P5_OUT_PP_Set(GPIO_Px5);//DRV8848_AIN2

	P4_OUT_OD_Set(GPIO_Px0);//LIGH_PWD
}
/* 电机正转控制 */
static void vMotorRunPos(void)
{
	ENCODER_PWD_nOPEN(0);

	MOTOR_DRV_nSLEEP(1);
	MOTOR_DRV_AIN1(1);
	MOTOR_DRV_AIN2(0);
}
/* 电机反转控制 */
static void vMotorRunNeg(void)
{
	ENCODER_PWD_nOPEN(0);

	MOTOR_DRV_nSLEEP(1);
	MOTOR_DRV_AIN1(0);
	MOTOR_DRV_AIN2(1);
}
/* 电机停止转动控制 */
static void vMotorStop(void)
{
	MOTOR_DRV_nSLEEP(0);
	MOTOR_DRV_AIN1(0);
	MOTOR_DRV_AIN2(0);

	//编码器是否在此关闭？？
	ENCODER_PWD_nOPEN(1);
}


/* Motor 光感脉冲计数器 *******************************************************/
void vMotorPulseCounterConfig(void)
{
	Timer0_SetHighPolity();
	Timer0_16bit();
	Timer0_AsCounter();
	Timer0_CLKOUT_P35_Disable();
	// Timer0_Load(65536UL - 1);
	// Timer0_IRQ_Enable();	
	// Timer0_Run();
}

/* 电机脉冲计数器启动工作，计数usPulseBeat个脉冲后触发中断 */
static void vMotorPulseCounterStart(u16 usPulseBeat)
{
	xMotorCtrlPara.usPulseNum = 0xFFFF - usPulseBeat + 1;//或(65536UL - usPulseBeat)
	xMotorCtrlPara.ucPulseInterval = 0;
	vDebugLog("start:",xMotorCtrlPara.usPulseNum,"\n");
	Timer0_Load(xMotorCtrlPara.usPulseNum);
	Timer0_IRQ_Enable();	
	Timer0_Run();
}

/* 电机脉冲计数器停止工作 */
static void vMotorPulseCounterStop(void)
{
	Timer0_IRQ_Disable();	
	Timer0_Stop();
}

/* 电机堵转检测函数，调用周期10ms */
void vMotorLockRotorMonitorPolling_10ms(void)
{
	/* 
	1.未使用PWM情况下，电机全速转动，大约85ms一个光电脉冲 
	2.避免误判堵转，预留3个脉冲，即255ms检测脉冲数未变化判断堵转
	3.不可直接在255ms周期中直接调用检测函数，例如：周期进行到250ms，此时启动电机，那经过5ms就会检测计数器数值，导致误判计数器值未变化
	*/
	u16 Counter0_TC = 0;
	if (xMotorCtrlPara.ucMotorRunSta != MOTOR_STOP)
	{
		if ((++xMotorCtrlPara.ucPulseInterval)>=(PULSE_PERIOD_MS*3/10))//260ms,说明：3个脉冲时间，除以10因为调用周期为10ms
		{
			xMotorCtrlPara.ucPulseInterval = 0;
			/* 通过检测计数器值是否变化，判断电机堵转 */
			Counter0_TC = Timer0_Read_TC();
			if (Counter0_TC==xMotorCtrlPara.usPulseNum)
			{//电机转速为零，可能原因：1.堵转；2.光感/电机损坏
				vValveStop();
				/* 状态处理 */
				if (xMotorCtrlPara.ucCurPos == (VALVE_POS_ZERO | VALVE_GOTO_SET_POS))
				{//走向零位，出现堵转正常
					xMotorCtrlPara.ucCurPos = VALVE_POS_ZERO;
				}
				else
				{//出错
					xMotorCtrlPara.ucMotorRunErr = TRUE;
					/* 控制阀体向工作位移动,会先经过堵转位 */
					xMotorCtrlPara.ucCurPos = VALVE_POS_UNKOWN;
					xMotorCtrlPara.ucDesPos = VALVE_POS_WORK;
				}
				vDebugLog("Lock stop",0,"\n");
			}
			xMotorCtrlPara.usPulseNum = Counter0_TC;
		}
#if 0
		vDebugLog("TC:",Timer0_Read_TC(),"\n");
#endif
	}
}


/* Timer0中断函数 */
void MotorPulseCounter_IrqHandler(void) interrupt TIMER0_VECTOR
{//到达目标位置
	vValveStop();
	xMotorCtrlPara.ucCurPos &= ~VALVE_GOTO_SET_POS;
#if 0
	vDebugLog("counter stop",Timer0_Read_TC(),"\n");
#endif
}

/* 阀体活塞运动，从一个位置(usSrcPos)切换至另一个位置(usDesPos) */
static void vValveRun(u8 usSrcPos, u8 usDesPos)
{
	u16 pulseBeat = 0;
	if (xMotorCtrlPara.ucMotorRunSta!=MOTOR_STOP)
	{/* 电机运动中,不可切换至新位置 */
		return;
	}
	
	xMotorCtrlPara.ucCurPos = usDesPos | VALVE_GOTO_SET_POS;			//向目标位置移动状态
	xMotorCtrlPara.usPosHoldTime = usWaterWayDuration[usDesPos];		//目标位置保持时间
	if (usWaterWayPlus[usDesPos] < usWaterWayPlus[usSrcPos])
	{
		pulseBeat = usWaterWayPlus[usSrcPos] - usWaterWayPlus[usDesPos];	//位置切换，走位脉冲数量
		vMotorPulseCounterStart(pulseBeat);
		vMotorRunNeg();
		xMotorCtrlPara.ucMotorRunSta = MOTOR_RUN_NEG;						//电机反转状态
		vDebugLog("Step:",pulseBeat,"\n");
	}
	else if (usWaterWayPlus[usDesPos] > usWaterWayPlus[usSrcPos])
	{
		pulseBeat = usWaterWayPlus[usDesPos] - usWaterWayPlus[usSrcPos];
		vMotorPulseCounterStart(pulseBeat);
		vMotorRunPos();
		xMotorCtrlPara.ucMotorRunSta = MOTOR_RUN_POS;						//电机正转状态
		vDebugLog("Step:",pulseBeat,"\n");
	}
	else
	{//usDesPos == usSrcPos
		
	}
}

/* 阀体活塞停止运行 */
static void vValveStop(void)
{
	vMotorPulseCounterStop();
	vMotorStop();
	xMotorCtrlPara.ucMotorRunSta = MOTOR_STOP;
}

/* 阀体保持位置时间,时间为零切换位置 */
static u16 usValvePosHoldTimePolling_1s(void)
{
	if (xMotorCtrlPara.usPosHoldTime!=0)
	{
		xMotorCtrlPara.usPosHoldTime--;
	}

	return xMotorCtrlPara.usPosHoldTime;
}

/* 阀体控制参数初始化 */
void vValveCtrlParmsInit(void)
{
	xMotorCtrlPara.ucCurPos = VALVE_POS_UNKOWN;
	xMotorCtrlPara.ucDesPos = VALVE_POS_WORK;//后期从e2prom读取
}

/* 阀体移动至再生位置 */
void vValveMoveToRegenPos(u8 ucTargePos)
{
	u8 pos = xMotorCtrlPara.ucCurPos&(~VALVE_GOTO_SET_POS);
	if ((ucTargePos==VALVE_POS_FULL_REGEN)
		||(ucTargePos==VALVE_POS_PART_REGEN))
	{
		if (!(pos>=VALVE_POS_REFILL)&&(pos<=VALVE_POS_RINSE))
		{
			xMotorCtrlPara.ucDesPos = ucTargePos;
			vDebugLog("M",ucTargePos,"\n");
		}
	}
}

/* 阀体移动至下一个位置 */
void vValveMoveToNextPos(u8 ucTargePos)
{
	if (!(xMotorCtrlPara.ucCurPos&VALVE_GOTO_SET_POS))
	{
		xMotorCtrlPara.ucDesPos = ucTargePos;
		vDebugLog("M",ucTargePos,"\n");
	}
}

/* 阀体修改位置保持时间 */
void vValveChgPosHoldTime(u16 ucHoldTime)
{
	xMotorCtrlPara.usPosHoldTime = ucHoldTime;
	// vDebugLog("M",ucTargePos,"\n");
}

/* 设置阀体位置保持时间 */
void vSetValvePosHoldTime(u8 ucPos, u16 usSec)
{
	if ((ucPos>=VALVE_POS_REFILL)&&(ucPos<=VALVE_POS_RINSE))
	{
		usWaterWayDuration[ucPos] = usSec;
	}
}

/* 阀体位置切换轮训处理，周期1秒 */
void vValvePosSwitchProcPolling_1s(void)
{
	// vDebugLog("valve:",xMotorCtrlPara.ucCurPos,"\0");
	// vDebugLog("-",xMotorCtrlPara.ucDesPos,"\0");
	// vDebugLog(":",xMotorCtrlPara.usPosHoldTime,"\n");
	switch (xMotorCtrlPara.ucCurPos)
	{
	case VALVE_POS_ZERO://堵转
		{
			if (xMotorCtrlPara.ucDesPos!=VALVE_POS_UNKOWN)
			{
				vValveRun(VALVE_POS_ZERO, xMotorCtrlPara.ucDesPos);
			}		
		}
		break;
	case VALVE_POS_WORK://工作
		{
			if ((xMotorCtrlPara.ucDesPos==VALVE_POS_FULL_REGEN)
				||(xMotorCtrlPara.ucDesPos==VALVE_POS_PART_REGEN))
			{/* 启动再生 */
				vValveRun(VALVE_POS_WORK, xMotorCtrlPara.ucDesPos);
			}				
		}
		break;
	/* 以下再生代码可以提炼下 */
	case VALVE_POS_REFILL://注水
		{
			if ((usValvePosHoldTimePolling_1s()==0)
				||(xMotorCtrlPara.ucDesPos == VALVE_POS_DISSOLVE))
			{
				xMotorCtrlPara.ucDesPos = VALVE_POS_DISSOLVE;				
				vValveRun(VALVE_POS_REFILL, xMotorCtrlPara.ucDesPos);
			}
		}
		break;
	case VALVE_POS_DISSOLVE://溶盐
		{
			if ((usValvePosHoldTimePolling_1s()==0)
				||(xMotorCtrlPara.ucDesPos == VALVE_POS_BRINE))
			{
				xMotorCtrlPara.ucDesPos = VALVE_POS_BRINE;
				vValveRun(VALVE_POS_DISSOLVE, xMotorCtrlPara.ucDesPos);
			}
		}
		break;
	case VALVE_POS_BRINE://吸盐
		{
			if ((usValvePosHoldTimePolling_1s()==0)
				||(xMotorCtrlPara.ucDesPos == VALVE_POS_BACKWASH))
			{
				xMotorCtrlPara.ucDesPos = VALVE_POS_BACKWASH;
				vValveRun(VALVE_POS_BRINE, xMotorCtrlPara.ucDesPos);
			}
		}
		break;
	case VALVE_POS_BACKWASH://反洗
		{
			if ((usValvePosHoldTimePolling_1s()==0)
				||(xMotorCtrlPara.ucDesPos == VALVE_POS_RINSE))
			{
				xMotorCtrlPara.ucDesPos = VALVE_POS_RINSE;
				vValveRun(VALVE_POS_BACKWASH, xMotorCtrlPara.ucDesPos);
			}
		}
		break;
	case VALVE_POS_RINSE://正洗
		{
			if ((usValvePosHoldTimePolling_1s()==0)
				||(xMotorCtrlPara.ucDesPos == VALVE_POS_WORK))
			{
				xMotorCtrlPara.ucDesPos = VALVE_POS_WORK;
				vRegenConditionClear();
				vValveRun(VALVE_POS_RINSE, VALVE_POS_ZERO);//通过堵转回工作
			}
		}
		break;
	case VALVE_POS_BYPASS://旁通
		{
			if (xMotorCtrlPara.ucDesPos == VALVE_POS_WORK)
			{
				xMotorCtrlPara.ucDesPos = VALVE_POS_WORK;
				vValveRun(VALVE_POS_BYPASS, VALVE_POS_ZERO);//通过堵转回工作
			}			
		}
		break;
	case VALVE_POS_UNKOWN://未知
		{//控制阀上电，位置未知，需先回归零位
			vValveRun(VALVE_POS_UNKOWN,VALVE_POS_ZERO);	
		}
		break;
	default:
		{
			/* 运行中... */
			if (xMotorCtrlPara.ucCurPos&VALVE_GOTO_SET_POS)
			{/* 是否需要加超时处理 */
				/* code */
			}
		}
		break;
	}
}

