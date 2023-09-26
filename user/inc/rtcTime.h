#ifndef	__RTC_TIME_H__
#define	__RTC_TIME_H__

#include "pvtInt.h"

/* 编码转换 */
#define BCD2DEC(n)   	((((n)>>4)&0x0F)*10+((n)&0x0F))
#define DEC2BCD(n)  	((((n)/10)<<4)+((n)%10))

/* 时间结构 */
typedef struct
{//需要与PCF8563的寄存器02H-08H位置一致，方便连续读写
    u8 sec;
    u8 min;
    u8 hour;
	u8 day;
	u8 week;
	u8 month;
	u8 year;
}Time_ST;

/* 时间事件 */
typedef enum
{
	TIME_EVENT_MIN = 0,
	TIME_EVENT_HOUR,
	TIME_EVENT_DAY,
	TIME_EVENT_MAX
}TimeEvent_EN;

/* 时间事件回调函数 */
typedef void (*pTimEventHandle)(void);

/* RTC时间初始化 */
u8 ucTimeInit(void);
/* 时钟参数设置 */
void vTimeSet(Time_ST *pxTime);
/* 时间事件回调函数注册 */
void vTimeEventCallbackRegister(u8 ucTimeEvent, pTimEventHandle pCb);
/* RTC时间定时读取，调用周期1秒 */
void vTimeReadPolling_1s(void);
/* 获取当前时间小时数值 */
u8 ucGetCurrTimeHour(void);
/* 获取当前时间分钟数值 */
u8 ucGetCurrTimeMin(void);

#endif
