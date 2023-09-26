#include "rtcTime.h"
#include "config.h"
#include "i2c_software.h"
#include "delay.h"

/* �Ĵ������� */
#define PCF8563_REG_STATUS1						0x00
// #define PCF8563_REG_STATUS2						0x01
#define PCF8563_REG_SECOND						0x02
#define PCF8563_REG_MINUTE						0x03
#define PCF8563_REG_HOURS						0x04
#define PCF8563_REG_DAY							0x05
#define PCF8563_REG_WEEK						0x06
#define PCF8563_REG_MONTH						0x07
#define PCF8563_REG_YEAR						0x08
// #define PCF8563_REG_MINUTE_ALARM				0x09
// #define PCF8563_REG_HOURS_ALARM					0x0A
// #define PCF8563_REG_DAY_ALARM					0x0B
// #define PCF8563_REG_WEEK_ALARM					0x0C
// #define PCF8563_REG_CLKOUT						0x0D
// #define PCF8563_REG_TIMER_CTR					0x0E
// #define PCF8563_REG_TIMER 						0x0F
/* 07H�Ĵ������ͱ�־λ */
#define PCF8563_CENTURY_FLG                     0x80
/* I2C�豸��ַ */
#define PCF8563_ADDR						    0x51/* ע�⣺�˵�ַ������ƫ�ƺ����������л��Զ����λ�ƶ�һλ */


/* ���� */
Time_ST xdata xCurTime = {0};
pTimEventHandle xdata pTimeEventCb[TIME_EVENT_MAX] = {NULL};
u8 xdata ucTimeInitFlg = FALSE;//���ԣ����ڿ�ɾ��

/* ���ģ��i2c���� */
static u8 ucSw_IIC_Send(u16 usRegAddr,u8 *ucpWrDat,u16 usWrDatLen)
{
	return ucSw_IIC_SendOps(PCF8563_ADDR,usRegAddr,I2C_REG_ADDR_U8,ucpWrDat,usWrDatLen);
}

/* ���ģ��i2��д��ַ������� */
static u8 ucSw_IIC_Recv(u16 usRegAddr,u8 *ucpRdDat,u16 usRdDatLen)
{
	if (ucSw_IIC_SendOps(PCF8563_ADDR,usRegAddr,I2C_REG_ADDR_U8,NULL,0)!=SW_I2C_RET_FALSE)
	{
		return ucSw_IIC_RecvOps(PCF8563_ADDR,ucpRdDat,usRdDatLen);
	}
	return SW_I2C_RET_FALSE;
}

/* PCF8563ʱ��ֵ���� */
static void vPCF8563_TimeValProc(Time_ST *pxTime)
{
	pxTime->sec    	&= 0x7F;
	pxTime->min    	&= 0x7F;
	pxTime->hour   	&= 0x3F;
	pxTime->day		&= 0x3F;	
	pxTime->week   	&= 0x07;
	pxTime->month 	&= 0x1F;//C=1
}

/* RTCʱ���ʼ�� */
u8 ucTimeInit(void)
{
	u8 ret = SW_I2C_RET_FALSE;
	u8 cnt = 0;

	while (cnt++<3)
	{
		ret = ucSw_IIC_Recv(PCF8563_REG_SECOND,(u8 *)&xCurTime, PCF8563_REG_YEAR-PCF8563_REG_SECOND+1);
		if (ret==SW_I2C_RET_TRUE)
		{
			if ((xCurTime.year>=0x23)&&(xCurTime.month&PCF8563_CENTURY_FLG))
			{//RTCʱ����ȷ
				vPCF8563_TimeValProc(&xCurTime);
			}
			else
			{//RTCʱ�����
				xCurTime.sec    = DEC2BCD(0);
				xCurTime.min    = DEC2BCD(30);
				xCurTime.hour   = DEC2BCD(7);
				xCurTime.day	= DEC2BCD(6);
				xCurTime.week   = DEC2BCD(3);//��ĩΪ0
				xCurTime.month 	= DEC2BCD(9)|PCF8563_CENTURY_FLG;
				xCurTime.year 	= DEC2BCD(23);

				ucSw_IIC_Send(PCF8563_REG_SECOND,(u8 *)&xCurTime, PCF8563_REG_YEAR-PCF8563_REG_SECOND+1);
				ucTimeInitFlg = TRUE;
			}
			break;
		}
		delay_ms(100);
	}

	return ret;
}

/* ʱ�Ӳ������� */
void vTimeSet(Time_ST *pxTime)
{//ע:����ֹͣʱ���޸ģ����������㿪ʼ
	Time_ST rtc;
	// u8 sta1 = 0x20;//RTCʱ��ֹͣ
	// ucSw_IIC_Send(PCF8563_REG_SECOND,(u8 *)&sta1, 1);
	rtc.year 	= DEC2BCD(pxTime->year);
	rtc.month 	= DEC2BCD(pxTime->month)|PCF8563_CENTURY_FLG;;
	rtc.day 	= DEC2BCD(pxTime->day);
	rtc.hour 	= DEC2BCD(pxTime->hour);
	rtc.min 	= DEC2BCD(pxTime->min);
	rtc.sec 	= DEC2BCD(pxTime->sec);
	rtc.week 	= DEC2BCD(pxTime->week);
	ucSw_IIC_Send(PCF8563_REG_SECOND,(u8 *)&rtc, PCF8563_REG_YEAR-PCF8563_REG_SECOND+1);
	// sta1 = 0x00;//RTCʱ������
	// ucSw_IIC_Send(PCF8563_REG_SECOND,(u8 *)&sta1, 1);
}

/* ʱ���¼��ص�����ע�� */
void vTimeEventCallbackRegister(u8 ucTimeEvent, pTimEventHandle pCb)
{
	if (ucTimeEvent<TIME_EVENT_MAX)
	{
		pTimeEventCb[ucTimeEvent] = pCb;
	}	
}

/* RTCʱ�䶨ʱ��ȡ����������1�� */
void vTimeReadPolling_1s(void)
{
	u8 min 	= xCurTime.min;
    u8 hour = xCurTime.hour;
	u8 day  = xCurTime.day;
	u8 ret = ucSw_IIC_Recv(PCF8563_REG_SECOND,(u8 *)&xCurTime, PCF8563_REG_YEAR-PCF8563_REG_SECOND+1);
	if (ret==SW_I2C_RET_TRUE)
	{
		vPCF8563_TimeValProc(&xCurTime);
		if ((min!=xCurTime.min)&&(pTimeEventCb[TIME_EVENT_MIN]!=NULL))
		{
			pTimeEventCb[TIME_EVENT_MIN]();
		}
		if ((hour!=xCurTime.hour)&&(pTimeEventCb[TIME_EVENT_HOUR]!=NULL))
		{
			pTimeEventCb[TIME_EVENT_HOUR]();
		}
		if ((day!=xCurTime.day)&&(pTimeEventCb[TIME_EVENT_DAY]!=NULL))
		{
			pTimeEventCb[TIME_EVENT_DAY]();
		}
	}
}

/* ��ȡ��ǰʱ��Сʱ��ֵ */
u8 ucGetCurrTimeHour(void)
{
	return BCD2DEC(xCurTime.hour);
}
/* ��ȡ��ǰʱ�������ֵ */
u8 ucGetCurrTimeMin(void)
{
	return BCD2DEC(xCurTime.min);
}

