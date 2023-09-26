#ifndef __I2C_SOFTWARE_H__
#define __I2C_SOFTWARE_H__

/* 开启或关闭模拟I2C */
#define  I2C_SOFTWARE_ENABLE 1

#if (I2C_SOFTWARE_ENABLE==1)

#include "config.h"
/* 如果多个软件I2C,使用函数指针,使用宏主要是为了减少代码占用空间 */
#define SDA_Mode_Output()    P1_OUT_OD_Set(GPIO_Px5)
#define SDA_Mode_Input()     P1_IN_HighZ_Set(GPIO_Px5)
#define SDA_Set_Level(n)     P15 = (n);_nop_();_nop_()
#define SDA_Read_Level()     P15
#define SCK_Mode_Output()    P1_OUT_OD_Set(GPIO_Px6)
#define SCK_Set_Level(n)     P16 = (n);_nop_();_nop_()
#define IIC_Delay()          _nop_();_nop_();_nop_();_nop_()

/* 返回值 */
#define SW_I2C_RET_FALSE	    (FALSE)
#define SW_I2C_RET_TRUE			(TRUE)

/* 寄存器地址是8位或16位 */
typedef enum
{
    I2C_REG_ADDR_U8 = 0,
    I2C_REG_ADDR_U16
}IIC_RegAddrType_EN;

/* 软件模拟I2C发送操作 */
u8 ucSw_IIC_SendOps(u8 ucDevAddr,u16 usRegAddr,IIC_RegAddrType_EN ucRegType,u8 *ucpDat,u16 usDatLen);
/* 软件模拟I2C读取操作 */
u8 ucSw_IIC_RecvOps(u8 ucDevAddr,u8 *ucpDat,u16 usDatLen);

#endif

#endif/* I2C_SOFTWARE_H */
