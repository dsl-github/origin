#ifndef	__RTC_TIME_H__
#define	__RTC_TIME_H__

#include "pvtInt.h"

/* ����ת�� */
#define BCD2DEC(n)   	((((n)>>4)&0x0F)*10+((n)&0x0F))
#define DEC2BCD(n)  	((((n)/10)<<4)+((n)%10))

/* ʱ��ṹ */
typedef struct
{//��Ҫ��PCF8563�ļĴ���02H-08Hλ��һ�£�����������д
    u8 sec;
    u8 min;
    u8 hour;
	u8 day;
	u8 week;
	u8 month;
	u8 year;
}Time_ST;

/* ʱ���¼� */
typedef enum
{
	TIME_EVENT_MIN = 0,
	TIME_EVENT_HOUR,
	TIME_EVENT_DAY,
	TIME_EVENT_MAX
}TimeEvent_EN;

/* ʱ���¼��ص����� */
typedef void (*pTimEventHandle)(void);

/* RTCʱ���ʼ�� */
u8 ucTimeInit(void);
/* ʱ�Ӳ������� */
void vTimeSet(Time_ST *pxTime);
/* ʱ���¼��ص�����ע�� */
void vTimeEventCallbackRegister(u8 ucTimeEvent, pTimEventHandle pCb);
/* RTCʱ�䶨ʱ��ȡ����������1�� */
void vTimeReadPolling_1s(void);
/* ��ȡ��ǰʱ��Сʱ��ֵ */
u8 ucGetCurrTimeHour(void);
/* ��ȡ��ǰʱ�������ֵ */
u8 ucGetCurrTimeMin(void);

#endif
