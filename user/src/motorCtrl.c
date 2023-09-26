#include "config.h"
#include "motorCtrl.h"
#include "regenProc.h"
#include "debugLog.h"

/* ��ǰδʹ��PWM����£����ȫ��ת������Լ85msһ��������� */
#define PULSE_PERIOD_MS      (85)
/* ���״̬ */
typedef enum
{
	MOTOR_STOP=0,  	//ֹͣ
	MOTOR_RUN_POS,	//��ת
	MOTOR_RUN_NEG	//��ת
}MotorWorkSta_EN;

ValveCtrl_ST xdata xMotorCtrlPara={0,0,0,0};


/*  ��ת:Ŀ��λ������������Դλ��������                                       										      */
/*  ��ת:Ŀ��λ��������С��Դλ��������                                                                                    */
/* ������                                      ��ת     ����     עˮ     ����     ����    ��ϴ     ��ϴ      ��ͨ     δ֪*/
u16 code usWaterWayPlus[VALVE_POS_MAX]     = {0,      10,      50,       10,    150,    200,     250,     300,     0xFFF0};   
/* ����ʱ��(��) */
u16 xdata usWaterWayDuration[VALVE_POS_MAX]= {0,       0,    4*60,   120*60,  45*60,   5*60,    5*60,       0,          0};   
/* ��һ��λ�� */
// u16 xdata usWaterWayNextPos[VALVE_POS_MAX] =  {0,       0,    4*60,   120*60,  45*60,   5*60,    5*60,       0,          0};   

/* Motor H�ſ���IO *******************************************************/
#define MOTOR_DRV_nSLEEP(n)   	P17 = (n)
#define MOTOR_DRV_AIN1(n)    	P54 = (n)
#define MOTOR_DRV_AIN2(n)    	P55 = (n)
#define ENCODER_PWD_nOPEN(n)   	P40 = (n)

/* �������IO��ʼ�� */
void vMotorCtrlGpioConfig(void)
{
	/* ������� */
	P1_OUT_PP_Set(GPIO_Px7);//DRV8848_nSLEEP
	P5_OUT_PP_Set(GPIO_Px4);//DRV8848_AIN1
	P5_OUT_PP_Set(GPIO_Px5);//DRV8848_AIN2

	P4_OUT_OD_Set(GPIO_Px0);//LIGH_PWD
}
/* �����ת���� */
static void vMotorRunPos(void)
{
	ENCODER_PWD_nOPEN(0);

	MOTOR_DRV_nSLEEP(1);
	MOTOR_DRV_AIN1(1);
	MOTOR_DRV_AIN2(0);
}
/* �����ת���� */
static void vMotorRunNeg(void)
{
	ENCODER_PWD_nOPEN(0);

	MOTOR_DRV_nSLEEP(1);
	MOTOR_DRV_AIN1(0);
	MOTOR_DRV_AIN2(1);
}
/* ���ֹͣת������ */
static void vMotorStop(void)
{
	MOTOR_DRV_nSLEEP(0);
	MOTOR_DRV_AIN1(0);
	MOTOR_DRV_AIN2(0);

	//�������Ƿ��ڴ˹رգ���
	ENCODER_PWD_nOPEN(1);
}


/* Motor ������������ *******************************************************/
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

/* ��������������������������usPulseBeat������󴥷��ж� */
static void vMotorPulseCounterStart(u16 usPulseBeat)
{
	xMotorCtrlPara.usPulseNum = 0xFFFF - usPulseBeat + 1;//��(65536UL - usPulseBeat)
	xMotorCtrlPara.ucPulseInterval = 0;
	vDebugLog("start:",xMotorCtrlPara.usPulseNum,"\n");
	Timer0_Load(xMotorCtrlPara.usPulseNum);
	Timer0_IRQ_Enable();	
	Timer0_Run();
}

/* ������������ֹͣ���� */
static void vMotorPulseCounterStop(void)
{
	Timer0_IRQ_Disable();	
	Timer0_Stop();
}

/* �����ת��⺯������������10ms */
void vMotorLockRotorMonitorPolling_10ms(void)
{
	/* 
	1.δʹ��PWM����£����ȫ��ת������Լ85msһ��������� 
	2.�������ж�ת��Ԥ��3�����壬��255ms���������δ�仯�ж϶�ת
	3.����ֱ����255ms������ֱ�ӵ��ü�⺯�������磺���ڽ��е�250ms����ʱ����������Ǿ���5ms�ͻ����������ֵ���������м�����ֵδ�仯
	*/
	u16 Counter0_TC = 0;
	if (xMotorCtrlPara.ucMotorRunSta != MOTOR_STOP)
	{
		if ((++xMotorCtrlPara.ucPulseInterval)>=(PULSE_PERIOD_MS*3/10))//260ms,˵����3������ʱ�䣬����10��Ϊ��������Ϊ10ms
		{
			xMotorCtrlPara.ucPulseInterval = 0;
			/* ͨ����������ֵ�Ƿ�仯���жϵ����ת */
			Counter0_TC = Timer0_Read_TC();
			if (Counter0_TC==xMotorCtrlPara.usPulseNum)
			{//���ת��Ϊ�㣬����ԭ��1.��ת��2.���/�����
				vValveStop();
				/* ״̬���� */
				if (xMotorCtrlPara.ucCurPos == (VALVE_POS_ZERO | VALVE_GOTO_SET_POS))
				{//������λ�����ֶ�ת����
					xMotorCtrlPara.ucCurPos = VALVE_POS_ZERO;
				}
				else
				{//����
					xMotorCtrlPara.ucMotorRunErr = TRUE;
					/* ���Ʒ�������λ�ƶ�,���Ⱦ�����תλ */
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


/* Timer0�жϺ��� */
void MotorPulseCounter_IrqHandler(void) interrupt TIMER0_VECTOR
{//����Ŀ��λ��
	vValveStop();
	xMotorCtrlPara.ucCurPos &= ~VALVE_GOTO_SET_POS;
#if 0
	vDebugLog("counter stop",Timer0_Read_TC(),"\n");
#endif
}

/* ��������˶�����һ��λ��(usSrcPos)�л�����һ��λ��(usDesPos) */
static void vValveRun(u8 usSrcPos, u8 usDesPos)
{
	u16 pulseBeat = 0;
	if (xMotorCtrlPara.ucMotorRunSta!=MOTOR_STOP)
	{/* ����˶���,�����л�����λ�� */
		return;
	}
	
	xMotorCtrlPara.ucCurPos = usDesPos | VALVE_GOTO_SET_POS;			//��Ŀ��λ���ƶ�״̬
	xMotorCtrlPara.usPosHoldTime = usWaterWayDuration[usDesPos];		//Ŀ��λ�ñ���ʱ��
	if (usWaterWayPlus[usDesPos] < usWaterWayPlus[usSrcPos])
	{
		pulseBeat = usWaterWayPlus[usSrcPos] - usWaterWayPlus[usDesPos];	//λ���л�����λ��������
		vMotorPulseCounterStart(pulseBeat);
		vMotorRunNeg();
		xMotorCtrlPara.ucMotorRunSta = MOTOR_RUN_NEG;						//�����ת״̬
		vDebugLog("Step:",pulseBeat,"\n");
	}
	else if (usWaterWayPlus[usDesPos] > usWaterWayPlus[usSrcPos])
	{
		pulseBeat = usWaterWayPlus[usDesPos] - usWaterWayPlus[usSrcPos];
		vMotorPulseCounterStart(pulseBeat);
		vMotorRunPos();
		xMotorCtrlPara.ucMotorRunSta = MOTOR_RUN_POS;						//�����ת״̬
		vDebugLog("Step:",pulseBeat,"\n");
	}
	else
	{//usDesPos == usSrcPos
		
	}
}

/* �������ֹͣ���� */
static void vValveStop(void)
{
	vMotorPulseCounterStop();
	vMotorStop();
	xMotorCtrlPara.ucMotorRunSta = MOTOR_STOP;
}

/* ���屣��λ��ʱ��,ʱ��Ϊ���л�λ�� */
static u16 usValvePosHoldTimePolling_1s(void)
{
	if (xMotorCtrlPara.usPosHoldTime!=0)
	{
		xMotorCtrlPara.usPosHoldTime--;
	}

	return xMotorCtrlPara.usPosHoldTime;
}

/* ������Ʋ�����ʼ�� */
void vValveCtrlParmsInit(void)
{
	xMotorCtrlPara.ucCurPos = VALVE_POS_UNKOWN;
	xMotorCtrlPara.ucDesPos = VALVE_POS_WORK;//���ڴ�e2prom��ȡ
}

/* �����ƶ�������λ�� */
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

/* �����ƶ�����һ��λ�� */
void vValveMoveToNextPos(u8 ucTargePos)
{
	if (!(xMotorCtrlPara.ucCurPos&VALVE_GOTO_SET_POS))
	{
		xMotorCtrlPara.ucDesPos = ucTargePos;
		vDebugLog("M",ucTargePos,"\n");
	}
}

/* �����޸�λ�ñ���ʱ�� */
void vValveChgPosHoldTime(u16 ucHoldTime)
{
	xMotorCtrlPara.usPosHoldTime = ucHoldTime;
	// vDebugLog("M",ucTargePos,"\n");
}

/* ���÷���λ�ñ���ʱ�� */
void vSetValvePosHoldTime(u8 ucPos, u16 usSec)
{
	if ((ucPos>=VALVE_POS_REFILL)&&(ucPos<=VALVE_POS_RINSE))
	{
		usWaterWayDuration[ucPos] = usSec;
	}
}

/* ����λ���л���ѵ��������1�� */
void vValvePosSwitchProcPolling_1s(void)
{
	// vDebugLog("valve:",xMotorCtrlPara.ucCurPos,"\0");
	// vDebugLog("-",xMotorCtrlPara.ucDesPos,"\0");
	// vDebugLog(":",xMotorCtrlPara.usPosHoldTime,"\n");
	switch (xMotorCtrlPara.ucCurPos)
	{
	case VALVE_POS_ZERO://��ת
		{
			if (xMotorCtrlPara.ucDesPos!=VALVE_POS_UNKOWN)
			{
				vValveRun(VALVE_POS_ZERO, xMotorCtrlPara.ucDesPos);
			}		
		}
		break;
	case VALVE_POS_WORK://����
		{
			if ((xMotorCtrlPara.ucDesPos==VALVE_POS_FULL_REGEN)
				||(xMotorCtrlPara.ucDesPos==VALVE_POS_PART_REGEN))
			{/* �������� */
				vValveRun(VALVE_POS_WORK, xMotorCtrlPara.ucDesPos);
			}				
		}
		break;
	/* ��������������������� */
	case VALVE_POS_REFILL://עˮ
		{
			if ((usValvePosHoldTimePolling_1s()==0)
				||(xMotorCtrlPara.ucDesPos == VALVE_POS_DISSOLVE))
			{
				xMotorCtrlPara.ucDesPos = VALVE_POS_DISSOLVE;				
				vValveRun(VALVE_POS_REFILL, xMotorCtrlPara.ucDesPos);
			}
		}
		break;
	case VALVE_POS_DISSOLVE://����
		{
			if ((usValvePosHoldTimePolling_1s()==0)
				||(xMotorCtrlPara.ucDesPos == VALVE_POS_BRINE))
			{
				xMotorCtrlPara.ucDesPos = VALVE_POS_BRINE;
				vValveRun(VALVE_POS_DISSOLVE, xMotorCtrlPara.ucDesPos);
			}
		}
		break;
	case VALVE_POS_BRINE://����
		{
			if ((usValvePosHoldTimePolling_1s()==0)
				||(xMotorCtrlPara.ucDesPos == VALVE_POS_BACKWASH))
			{
				xMotorCtrlPara.ucDesPos = VALVE_POS_BACKWASH;
				vValveRun(VALVE_POS_BRINE, xMotorCtrlPara.ucDesPos);
			}
		}
		break;
	case VALVE_POS_BACKWASH://��ϴ
		{
			if ((usValvePosHoldTimePolling_1s()==0)
				||(xMotorCtrlPara.ucDesPos == VALVE_POS_RINSE))
			{
				xMotorCtrlPara.ucDesPos = VALVE_POS_RINSE;
				vValveRun(VALVE_POS_BACKWASH, xMotorCtrlPara.ucDesPos);
			}
		}
		break;
	case VALVE_POS_RINSE://��ϴ
		{
			if ((usValvePosHoldTimePolling_1s()==0)
				||(xMotorCtrlPara.ucDesPos == VALVE_POS_WORK))
			{
				xMotorCtrlPara.ucDesPos = VALVE_POS_WORK;
				vRegenConditionClear();
				vValveRun(VALVE_POS_RINSE, VALVE_POS_ZERO);//ͨ����ת�ع���
			}
		}
		break;
	case VALVE_POS_BYPASS://��ͨ
		{
			if (xMotorCtrlPara.ucDesPos == VALVE_POS_WORK)
			{
				xMotorCtrlPara.ucDesPos = VALVE_POS_WORK;
				vValveRun(VALVE_POS_BYPASS, VALVE_POS_ZERO);//ͨ����ת�ع���
			}			
		}
		break;
	case VALVE_POS_UNKOWN://δ֪
		{//���Ʒ��ϵ磬λ��δ֪�����Ȼع���λ
			vValveRun(VALVE_POS_UNKOWN,VALVE_POS_ZERO);	
		}
		break;
	default:
		{
			/* ������... */
			if (xMotorCtrlPara.ucCurPos&VALVE_GOTO_SET_POS)
			{/* �Ƿ���Ҫ�ӳ�ʱ���� */
				/* code */
			}
		}
		break;
	}
}

