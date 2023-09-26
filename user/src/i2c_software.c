#include "i2c_software.h"

#if (I2C_SOFTWARE_ENABLE == 1)

/* 起始信号 */
static u8 ucSw_IIC_Start(void)
{ /* 当SCL为高电平，SDA从高电平到低电平跳变，作为起始信号 */
	/* 时序：
	SCL  _____
			  |___
	SDA  ___
			|_____
	 */
	/* 配置SDA为开漏输出 */
	SDA_Mode_Output();
	/* 配置SCK为开漏输出 */
	SCK_Mode_Output();

	/* 拉高i2c总线 */
	SDA_Set_Level(1);
	SCK_Set_Level(1);
	IIC_Delay();
	// 查看此时SDA是否就绪（高电平）
	if (SDA_Read_Level() == 0)
	{ /* SDA线为低电平，总线忙，退出 */
		return SW_I2C_RET_FALSE;
	}
	/* 制造下降沿，下降沿是开始的标志 */
	SDA_Set_Level(0); // SDA跳变至低电平
	IIC_Delay();
	// 查看此时SDA是否变为低电平
	if (SDA_Read_Level() == 1)
	{ /* SDA线为高电平，总线出错，退出 */
		return SW_I2C_RET_FALSE;
	}
	SCK_Set_Level(0);//钳住I2C总线，准备发送或接收数据

	return SW_I2C_RET_TRUE;
}

/* 停止信号 */
static void ucSw_IIC_Stop(void)
{ /* 当SCL为高电平，SDA从低电平到高电平跳变，作为终止信号 */
	/* 时序：
			 ______
	SCL  ___|
				___
	SDA  ______|
	 */
	/* 配置SDA为推挽输出 */
	SDA_Mode_Output();
	/* 拉低i2c总线 */
	SCK_Set_Level(0);
	SDA_Set_Level(0);
	IIC_Delay();
	/* 制造上升沿，上升沿是结束的标志 */
	SCK_Set_Level(1); // SCK高电平
	IIC_Delay();
	SDA_Set_Level(1); // SDA跳变至高电平
	IIC_Delay();
}

/* 应答信号 */
static void ucSw_IIC_Ack(void)
{ 
	/* 时序：
				___
	SCL  ______|   |______
		 ____         ____
	SDA      |_______|
	 */
	/* 配置SDA为推挽输出 */
	SDA_Mode_Output();
	/* 拉低i2c总线 */
	SCK_Set_Level(0);
	SDA_Set_Level(0);
	IIC_Delay();

	SCK_Set_Level(1);
	IIC_Delay();
	SCK_Set_Level(0);
}

/* 非应答信号 */
static void ucSw_IIC_NoAck(void)
{ 
	/* 时序：
				___
	SCL  ______|   |______
			  _______
	SDA  ____|       |____
	 */
	/* 配置SDA为推挽输出 */
	SDA_Mode_Output();
	/* 拉低i2c总线 */
	SCK_Set_Level(0);
	IIC_Delay();

	SDA_Set_Level(1);
	IIC_Delay();
	SCK_Set_Level(1);
	IIC_Delay();
	SCK_Set_Level(0);
}

/* 等待应答信号 */
static u8 ucSw_IIC_WaitAck(void)
{ 
	u8 time = 0;
	/* 配置SDA为上拉输入 */
	SDA_Mode_Input();

	/* 释放i2c总线 */
	SDA_Set_Level(1);
	IIC_Delay();
	SCK_Set_Level(1);
	IIC_Delay();
	/* 查看从机是否应答 */
	while (SDA_Read_Level() == 1) // 从机若应答，会拉低SDA
	{
		if (time++ > 10)
		{//10个delay时间
			return SW_I2C_RET_FALSE;
		}
		IIC_Delay();
	}
	SCK_Set_Level(0);
	return SW_I2C_RET_TRUE;
}

/* 写一个字节数据 */
static u8 ucSw_IIC_WriteByte(void,u8 dat)
{
	u8 cnt;
	/* 配置SDA为推挽输出 */
	SDA_Mode_Output();

	/* SCL低(SCL低时，变化SDA) */
	SCK_Set_Level(0);
	IIC_Delay();

	for (cnt = 0; cnt < 8; cnt++)
	{
		/* 从最高位开始写起 */
		if (dat & 0x80)
		{
			SDA_Set_Level(1); // SDA高
		}
		else
		{
			SDA_Set_Level(0); // SDA低
		}
		dat <<= 1;
		IIC_Delay();
		SCK_Set_Level(1); // SCL高(发送数据)
		IIC_Delay();
		SCK_Set_Level(0); // SCL低(等待应答信号)
		IIC_Delay();
	}
	
	if (ucSw_IIC_WaitAck()==SW_I2C_RET_FALSE)
	{
		ucSw_IIC_Stop();
		return SW_I2C_RET_FALSE;
	}
	return SW_I2C_RET_TRUE;
}

/* 读一个字节数据 */
static u8 ucSw_IIC_ReadByte(void,u8 ack)
{
	u8 cnt = 0;
	u8 dat = 0;
	/* 配置SDA为上拉输入 */
	SDA_Mode_Input();

	for (cnt = 0; cnt < 8; cnt++)
	{
		SCK_Set_Level(0);// SCL低
		IIC_Delay();
		SCK_Set_Level(1);// SCL高(读取数据)
		dat <<= 1;
		if (SDA_Read_Level() == 1)
		{
			dat |= 0x01; // SDA高(数据有效)
		}
		IIC_Delay();
	}
	SCK_Set_Level(0);// SCL低
	IIC_Delay();
	// 发送应答信号，低代表应答，高代表非应答
	if (ack == 0)
	{
		ucSw_IIC_NoAck();
	}
	else
	{
		ucSw_IIC_Ack();
	}
	return dat; // 返回数据
}

/* 软件模拟I2C发送操作 */
u8 ucSw_IIC_SendOps(u8 ucDevAddr,u16 usRegAddr,IIC_RegAddrType_EN ucRegType,u8 *ucpDat,u16 usDatLen)
{
	u16 i;
	/* 1.开始 */
	if (ucSw_IIC_Start()==SW_I2C_RET_FALSE)
	{
		ucSw_IIC_Stop();
		return SW_I2C_RET_FALSE;
	}
	/* 2.设备地址+写标志 */
	if (ucSw_IIC_WriteByte(((ucDevAddr<<1) & 0xFE))==SW_I2C_RET_FALSE)
	{
		ucSw_IIC_Stop();
		return SW_I2C_RET_FALSE;
	}
	/* 3.数据地址 */
	/* 高8位 */
	if (ucRegType==I2C_REG_ADDR_U16)
	{
		if (ucSw_IIC_WriteByte((u8)(usRegAddr>>8))==SW_I2C_RET_FALSE)
		{
			ucSw_IIC_Stop();
			return SW_I2C_RET_FALSE;
		}
	}
	/* 低8位 */
	if (ucSw_IIC_WriteByte((u8)usRegAddr)==SW_I2C_RET_FALSE)
	{
		ucSw_IIC_Stop();
		return SW_I2C_RET_FALSE;
	}	
	/* 4.数据 */
	if ((ucpDat!=NULL)&&(usDatLen!=0))
	{
		for (i = 0; i < usDatLen; i++)
		{
			if (ucSw_IIC_WriteByte(ucpDat[i])==SW_I2C_RET_FALSE)
			{
				ucSw_IIC_Stop();
				return SW_I2C_RET_FALSE;
			}
		}
	}
	/* 5.停止 */
  	ucSw_IIC_Stop();

	return SW_I2C_RET_TRUE;
}

/* 软件模拟I2C读取操作 */
u8 ucSw_IIC_RecvOps(u8 ucDevAddr,u8 *ucpDat,u16 usDatLen)
{
	u16 i;
	u8 ack = 1;
	/* 传参断言 */
	if ((ucpDat==NULL)||(usDatLen==0))
	{
		return SW_I2C_RET_FALSE;
	}
	
	/* 1.重新开始 */
	if (ucSw_IIC_Start()==SW_I2C_RET_FALSE)
	{
		ucSw_IIC_Stop();
		return SW_I2C_RET_FALSE;
	}
	/* 2.设备地址+读标志 */
	if (ucSw_IIC_WriteByte(((ucDevAddr<<1) | 0x01))==SW_I2C_RET_FALSE)
	{
		ucSw_IIC_Stop();
		return SW_I2C_RET_FALSE;
	}
	/* 3.数据 */
	for (i = 0; i < usDatLen; i++)
	{
		if (i==(usDatLen-1))
		{
			ack = 0;
		}
		else
		{
			ack = 1;
		}		
		ucpDat[i] = ucSw_IIC_ReadByte(ack);
	}
	
	/* 4.停止 */
  	ucSw_IIC_Stop();
	return SW_I2C_RET_TRUE;
}

#endif 
