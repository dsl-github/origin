#include "toolTest.h"
#include "rtcTime.h"
#include "motorCtrl.h"
#include "flowCalc.h"
#include "regenProc.h"
#include "debugLog.h"

#ifdef TOOL_TEST
extern Time_ST xdata xCurTime;
extern ValveCtrl_ST xdata xMotorCtrlPara;
extern OutletFlowInfo_ST xdata xOutletFlowInfo;
extern SystemParm_ST xdata xSysInfo;
extern u8 xdata ucSaltRet;
/* 上传时间数据至工具 */
void vCurrTimeUploadToTool(void)
{
	ucDebugPrintString("I1[");
	vDebugPrintByte(BCD2DEC(xCurTime.year));
	vDebugPrintByte(BCD2DEC(xCurTime.month));
	vDebugPrintByte(BCD2DEC(xCurTime.day));
	vDebugPrintByte(BCD2DEC(xCurTime.hour));
	vDebugPrintByte(BCD2DEC(xCurTime.min));
	vDebugPrintByte(BCD2DEC(xCurTime.sec));
	ucDebugPrintString("]\n");
}

/* 上传阀体位置数据至工具 */
void vCurrMotorUploadToTool(void)
{
	ucDebugPrintString("I2[");
	vDebugPrintByte(xMotorCtrlPara.ucCurPos&(~VALVE_GOTO_SET_POS));
	vDebugPrintByte(xMotorCtrlPara.usPosHoldTime>>8);
	vDebugPrintByte(xMotorCtrlPara.usPosHoldTime);
	ucDebugPrintString("]\n");
}

/* 上传流量数据至工具 */
void vCurrFlowUploadToTool(void)
{
	u32 surplusWaterUse = 0;
	if (xSysInfo.usNominalWaterYield>=xOutletFlowInfo.ulRegenWaterUse)
	{
		surplusWaterUse = xSysInfo.usNominalWaterYield-xOutletFlowInfo.ulRegenWaterUse;
	}	
	ucDebugPrintString("I3[");
	vDebugPrintByte(xOutletFlowInfo.usFlowRate>>8);
	vDebugPrintByte(xOutletFlowInfo.usFlowRate);
	vDebugPrintByte(surplusWaterUse>>24);
	vDebugPrintByte(surplusWaterUse>>16);
	vDebugPrintByte(surplusWaterUse>>8);
	vDebugPrintByte(surplusWaterUse);
	vDebugPrintByte(xOutletFlowInfo.usYesterdayWaterUse>>8);
	vDebugPrintByte(xOutletFlowInfo.usYesterdayWaterUse);
	vDebugPrintByte(xOutletFlowInfo.usAvgDailyWaterUse>>8);
	vDebugPrintByte(xOutletFlowInfo.usAvgDailyWaterUse);
	ucDebugPrintString("]\n");
}

/* 上传工作数据至工具 */
void vWorkParaUploadToTool(void)
{	
	/* 10,15,20,30,40 */
	u8 saltPer = 100;
	
	if (ucSaltRet<=10)
	{
		saltPer = 0;
	}	
	else if (ucSaltRet<=15)
	{
		saltPer = 25;
	}
	else if (ucSaltRet<=20)
	{
		saltPer = 50;
	}
	else if (ucSaltRet<=30)
	{
		saltPer = 75;
	}
	ucDebugPrintString("I4[");
	vDebugPrintByte(xWorkParm.ucRegenDayCnt);
	vDebugPrintByte(xWorkParm.ucVacationRegenDayCnt);
	vDebugPrintByte(saltPer);
	ucDebugPrintString("]\n");
}
#endif

/* 工具测试信息上传 */
void vToolTestInfoUploadPolling_10ms(void)
{
#ifdef TOOL_TEST
	static u16 testCnt = 0;
	static u16 testCnt_1s = 0;
	switch (testCnt++)
	{
	case 0:
		vCurrTimeUploadToTool();
		break;
	case 1:
		vCurrMotorUploadToTool();
		break;
	case 2:
		vWorkParaUploadToTool();
		break;
	case 9:
		{
			testCnt = 0;
			//秒操作
			testCnt_1s++;
			if ((testCnt_1s%3)==0)
			{
				vCurrFlowUploadToTool();
			}
		}
		break;
	default:
		break;
	}
#endif
}

/* 工具测试信息解析 */
void vToolTestSetInfoParse(u8 *pucInfo,u16 usLen)
{ 
	u16 idx = 0;
	while (idx < usLen)
	{
		if (pucInfo[idx]=='S')
		{
			switch (pucInfo[idx+1])
			{
			case '1'://S1[23,09,18,07,24,00,01]
				{
					if ((pucInfo[idx+2]=='[')&&(pucInfo[idx+10]==']'))
					{
						Time_ST tmpTime;
						tmpTime.year 	= pucInfo[idx+3];
						tmpTime.month 	= pucInfo[idx+4];
						tmpTime.day 	= pucInfo[idx+5];
						tmpTime.hour 	= pucInfo[idx+6];
						tmpTime.min 	= pucInfo[idx+7];
						tmpTime.sec 	= pucInfo[idx+8];
						tmpTime.week 	= pucInfo[idx+9];
						vTimeSet(&tmpTime);
					}	
				}
				break;
#ifdef TOOL_TEST
			case '2':
				{
					if ((pucInfo[idx+2]=='[')&&(pucInfo[idx+4]==']'))
					{
						u8 tmpPos = pucInfo[idx+3];
						vValveMoveToNextPos(tmpPos);
					}	
				}
				break;
			case '3':
				{
					if ((pucInfo[idx+2]=='[')&&(pucInfo[idx+5]==']'))
					{
						u16 tmpHoldTime = pucInfo[idx+3];
						tmpHoldTime = (tmpHoldTime<<8)+pucInfo[idx+4];
						vValveChgPosHoldTime(tmpHoldTime);
					}	
				}
				break;
			case '4':
				{
					if ((pucInfo[idx+2]=='[')&&(pucInfo[idx+5]==']'))
					{
						u16 tmpAdd = pucInfo[idx+3];
						tmpAdd = (tmpAdd<<8)+pucInfo[idx+4];
						vWaterUseInc(tmpAdd);
					}	
				}
				break;
#endif			
			default:
				break;
			}
		}
		idx++;
	}
}