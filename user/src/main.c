
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
	vFlowRateCalc();				//流速计算
}
void vTimeHourEventHandle(void)
{
	vLastHourWaterUseCalc();		//前一小时用水量统计
}
void vTimeDayEventHandle(void)
{
	vYesterdayWaterUseCalc();		//昨日用水量统计
	vAvgDailyWaterUseCalc();		//日平均用水量计算
	vVacationDaysStatistics();		//假日天数统计
	vPeriodRegenDayCount();			//再生天数计数
}

void vToolTestInfo(void)
{
	vDebugLog("I1[",0,NULL);
}

void main(void)
{
	/* 调试串口 */
    vDebugUartConfig(vToolTestSetInfoParse);
	/* 参数初始化 */
	vSysParmInit();
	/* 系统时间 */
	vPCA_Init();
	/* 流量计脉冲计数器配置 */
	vFlowCounterConfig();
	/* 阀体控制参数初始化 */
	vValveCtrlParmsInit();
	/* 电机控制IO */
	vMotorCtrlGpioConfig();	
	/* 电机编码器脉冲计数器 */
	vMotorPulseCounterConfig();
	/* 外部中断 */
	vExt0Confit();
	/* RTC时间初始化 */
 	ucTimeInit();
	vTimeEventCallbackRegister(TIME_EVENT_MIN,vTimeMinEventHandle);
	vTimeEventCallbackRegister(TIME_EVENT_HOUR,vTimeHourEventHandle);
	vTimeEventCallbackRegister(TIME_EVENT_DAY,vTimeDayEventHandle);
	/* 中断总开关 */
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
			/* 周期10ms*************************************************************** */
			/* 电机堵转检测 */
			vMotorLockRotorMonitorPolling_10ms();
			/* 调试串口接收超时处理 */
			vDebugUartRxTimeout();
			/* 工具测试信息上传 */
			vToolTestInfoUploadPolling_10ms();
			/* 周期250ms************************************************************** */
			/* 再生触发监测 */
			if (!(usPeriod10msCnt%25))
			{//周期250ms
				vRegenEventMonitorPolling_250ms();
			}
			/* 周期1000ms************************************************************* */
			//以下函数执行周期都是1秒
			switch (usPeriod10msCnt++)
			{
			case 10://当前时间刷新
				{		
					vTimeReadPolling_1s();
				}									
				break;
			case 20://阀体位置切换
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




