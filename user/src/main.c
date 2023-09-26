
#include "boardInit.h"
#include "delay.h"
#include "motorCtrl.h"
#include "rtcTime.h"
#include "flowCalc.h"
#include "regenProc.h"
#include "toolTest.h"

u16 idata usPeriod10msCnt = 0;

void vTimeMinEventHandle(void)
{
	vFlowRateCalc();				//���ټ���
}
void vTimeHourEventHandle(void)
{
	vLastHourWaterUseCalc();		//ǰһСʱ��ˮ��ͳ��
}
void vTimeDayEventHandle(void)
{
	vYesterdayWaterUseCalc();		//������ˮ��ͳ��
	vAvgDailyWaterUseCalc();		//��ƽ����ˮ������
	vVacationDaysStatistics();		//��������ͳ��
	vPeriodRegenDayCount();			//������������
}

void vToolTestInfo(void)
{
	vDebugLog("I1[",0,NULL);
}

void main(void)
{
	/* ���Դ��� */
    vDebugUartConfig(vToolTestSetInfoParse);
	/* ������ʼ�� */
	vSysParmInit();
	/* ϵͳʱ�� */
	vPCA_Init();
	/* ������������������� */
	vFlowCounterConfig();
	/* ������Ʋ�����ʼ�� */
	vValveCtrlParmsInit();
	/* �������IO */
	vMotorCtrlGpioConfig();	
	/* ������������������ */
	vMotorPulseCounterConfig();
	/* �ⲿ�ж� */
	vExt0Confit();
	/* RTCʱ���ʼ�� */
 	ucTimeInit();
	vTimeEventCallbackRegister(TIME_EVENT_MIN,vTimeMinEventHandle);
	vTimeEventCallbackRegister(TIME_EVENT_HOUR,vTimeHourEventHandle);
	vTimeEventCallbackRegister(TIME_EVENT_DAY,vTimeDayEventHandle);
	/* �ж��ܿ��� */
	Set_IRQ_EA(1);
	
	// vUart1PrintDigit(test);
	vDebugLog("system ok\n",0,NULL);
	
	delay_ms(250);
	delay_ms(250);
	delay_ms(250);
	delay_ms(250);
	delay_ms(250);
	delay_ms(250);
	delay_ms(250);
	
	while(1)
	{
		if (ucGetSysTick10msFlg())
		{
			vSetSysTick10msFlg(FALSE);
			/* ����10ms*************************************************************** */
			/* �����ת��� */
			vMotorLockRotorMonitorPolling_10ms();
			/* ���Դ��ڽ��ճ�ʱ���� */
			vDebugUartRxTimeout();
			/* ���߲�����Ϣ�ϴ� */
			vToolTestInfoUploadPolling_10ms();
			/* ����250ms************************************************************** */
			/* ����������� */
			if (!(usPeriod10msCnt%25))
			{//����250ms
				vRegenEventMonitorPolling_250ms();
			}
			/* ����1000ms************************************************************* */
			//���º���ִ�����ڶ���1��
			switch (usPeriod10msCnt++)
			{
			case 10://��ǰʱ��ˢ��
				{		
					vTimeReadPolling_1s();
				}									
				break;
			case 20://����λ���л�
				{
					vValvePosSwitchProcPolling_1s();
				}
				break;
			case 100:
				{
					usPeriod10msCnt = 0;
				}
				break;
			default:
				break;
			}			
		}
	}
}




