#include "i2c_software.h"

#if (I2C_SOFTWARE_ENABLE == 1)

/* ��ʼ�ź� */
static u8 ucSw_IIC_Start(void)
{ /* ��SCLΪ�ߵ�ƽ��SDA�Ӹߵ�ƽ���͵�ƽ���䣬��Ϊ��ʼ�ź� */
	/* ʱ��
	SCL  _____
			  |___
	SDA  ___
			|_____
	 */
	/* ����SDAΪ��©��� */
	SDA_Mode_Output();
	/* ����SCKΪ��©��� */
	SCK_Mode_Output();

	/* ����i2c���� */
	SDA_Set_Level(1);
	SCK_Set_Level(1);
	IIC_Delay();
	// �鿴��ʱSDA�Ƿ�������ߵ�ƽ��
	if (SDA_Read_Level() == 0)
	{ /* SDA��Ϊ�͵�ƽ������æ���˳� */
		return SW_I2C_RET_FALSE;
	}
	/* �����½��أ��½����ǿ�ʼ�ı�־ */
	SDA_Set_Level(0); // SDA�������͵�ƽ
	IIC_Delay();
	// �鿴��ʱSDA�Ƿ��Ϊ�͵�ƽ
	if (SDA_Read_Level() == 1)
	{ /* SDA��Ϊ�ߵ�ƽ�����߳����˳� */
		return SW_I2C_RET_FALSE;
	}
	SCK_Set_Level(0);//ǯסI2C���ߣ�׼�����ͻ��������

	return SW_I2C_RET_TRUE;
}

/* ֹͣ�ź� */
static void ucSw_IIC_Stop(void)
{ /* ��SCLΪ�ߵ�ƽ��SDA�ӵ͵�ƽ���ߵ�ƽ���䣬��Ϊ��ֹ�ź� */
	/* ʱ��
			 ______
	SCL  ___|
				___
	SDA  ______|
	 */
	/* ����SDAΪ������� */
	SDA_Mode_Output();
	/* ����i2c���� */
	SCK_Set_Level(0);
	SDA_Set_Level(0);
	IIC_Delay();
	/* ���������أ��������ǽ����ı�־ */
	SCK_Set_Level(1); // SCK�ߵ�ƽ
	IIC_Delay();
	SDA_Set_Level(1); // SDA�������ߵ�ƽ
	IIC_Delay();
}

/* Ӧ���ź� */
static void ucSw_IIC_Ack(void)
{ 
	/* ʱ��
				___
	SCL  ______|   |______
		 ____         ____
	SDA      |_______|
	 */
	/* ����SDAΪ������� */
	SDA_Mode_Output();
	/* ����i2c���� */
	SCK_Set_Level(0);
	SDA_Set_Level(0);
	IIC_Delay();

	SCK_Set_Level(1);
	IIC_Delay();
	SCK_Set_Level(0);
}

/* ��Ӧ���ź� */
static void ucSw_IIC_NoAck(void)
{ 
	/* ʱ��
				___
	SCL  ______|   |______
			  _______
	SDA  ____|       |____
	 */
	/* ����SDAΪ������� */
	SDA_Mode_Output();
	/* ����i2c���� */
	SCK_Set_Level(0);
	IIC_Delay();

	SDA_Set_Level(1);
	IIC_Delay();
	SCK_Set_Level(1);
	IIC_Delay();
	SCK_Set_Level(0);
}

/* �ȴ�Ӧ���ź� */
static u8 ucSw_IIC_WaitAck(void)
{ 
	u8 time = 0;
	/* ����SDAΪ�������� */
	SDA_Mode_Input();

	/* �ͷ�i2c���� */
	SDA_Set_Level(1);
	IIC_Delay();
	SCK_Set_Level(1);
	IIC_Delay();
	/* �鿴�ӻ��Ƿ�Ӧ�� */
	while (SDA_Read_Level() == 1) // �ӻ���Ӧ�𣬻�����SDA
	{
		if (time++ > 10)
		{//10��delayʱ��
			return SW_I2C_RET_FALSE;
		}
		IIC_Delay();
	}
	SCK_Set_Level(0);
	return SW_I2C_RET_TRUE;
}

/* дһ���ֽ����� */
static u8 ucSw_IIC_WriteByte(void,u8 dat)
{
	u8 cnt;
	/* ����SDAΪ������� */
	SDA_Mode_Output();

	/* SCL��(SCL��ʱ���仯SDA) */
	SCK_Set_Level(0);
	IIC_Delay();

	for (cnt = 0; cnt < 8; cnt++)
	{
		/* �����λ��ʼд�� */
		if (dat & 0x80)
		{
			SDA_Set_Level(1); // SDA��
		}
		else
		{
			SDA_Set_Level(0); // SDA��
		}
		dat <<= 1;
		IIC_Delay();
		SCK_Set_Level(1); // SCL��(��������)
		IIC_Delay();
		SCK_Set_Level(0); // SCL��(�ȴ�Ӧ���ź�)
		IIC_Delay();
	}
	
	if (ucSw_IIC_WaitAck()==SW_I2C_RET_FALSE)
	{
		ucSw_IIC_Stop();
		return SW_I2C_RET_FALSE;
	}
	return SW_I2C_RET_TRUE;
}

/* ��һ���ֽ����� */
static u8 ucSw_IIC_ReadByte(void,u8 ack)
{
	u8 cnt = 0;
	u8 dat = 0;
	/* ����SDAΪ�������� */
	SDA_Mode_Input();

	for (cnt = 0; cnt < 8; cnt++)
	{
		SCK_Set_Level(0);// SCL��
		IIC_Delay();
		SCK_Set_Level(1);// SCL��(��ȡ����)
		dat <<= 1;
		if (SDA_Read_Level() == 1)
		{
			dat |= 0x01; // SDA��(������Ч)
		}
		IIC_Delay();
	}
	SCK_Set_Level(0);// SCL��
	IIC_Delay();
	// ����Ӧ���źţ��ʹ���Ӧ�𣬸ߴ����Ӧ��
	if (ack == 0)
	{
		ucSw_IIC_NoAck();
	}
	else
	{
		ucSw_IIC_Ack();
	}
	return dat; // ��������
}

/* ���ģ��I2C���Ͳ��� */
u8 ucSw_IIC_SendOps(u8 ucDevAddr,u16 usRegAddr,IIC_RegAddrType_EN ucRegType,u8 *ucpDat,u16 usDatLen)
{
	u16 i;
	/* 1.��ʼ */
	if (ucSw_IIC_Start()==SW_I2C_RET_FALSE)
	{
		ucSw_IIC_Stop();
		return SW_I2C_RET_FALSE;
	}
	/* 2.�豸��ַ+д��־ */
	if (ucSw_IIC_WriteByte(((ucDevAddr<<1) & 0xFE))==SW_I2C_RET_FALSE)
	{
		ucSw_IIC_Stop();
		return SW_I2C_RET_FALSE;
	}
	/* 3.���ݵ�ַ */
	/* ��8λ */
	if (ucRegType==I2C_REG_ADDR_U16)
	{
		if (ucSw_IIC_WriteByte((u8)(usRegAddr>>8))==SW_I2C_RET_FALSE)
		{
			ucSw_IIC_Stop();
			return SW_I2C_RET_FALSE;
		}
	}
	/* ��8λ */
	if (ucSw_IIC_WriteByte((u8)usRegAddr)==SW_I2C_RET_FALSE)
	{
		ucSw_IIC_Stop();
		return SW_I2C_RET_FALSE;
	}	
	/* 4.���� */
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
	/* 5.ֹͣ */
  	ucSw_IIC_Stop();

	return SW_I2C_RET_TRUE;
}

/* ���ģ��I2C��ȡ���� */
u8 ucSw_IIC_RecvOps(u8 ucDevAddr,u8 *ucpDat,u16 usDatLen)
{
	u16 i;
	u8 ack = 1;
	/* ���ζ��� */
	if ((ucpDat==NULL)||(usDatLen==0))
	{
		return SW_I2C_RET_FALSE;
	}
	
	/* 1.���¿�ʼ */
	if (ucSw_IIC_Start()==SW_I2C_RET_FALSE)
	{
		ucSw_IIC_Stop();
		return SW_I2C_RET_FALSE;
	}
	/* 2.�豸��ַ+����־ */
	if (ucSw_IIC_WriteByte(((ucDevAddr<<1) | 0x01))==SW_I2C_RET_FALSE)
	{
		ucSw_IIC_Stop();
		return SW_I2C_RET_FALSE;
	}
	/* 3.���� */
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
	
	/* 4.ֹͣ */
  	ucSw_IIC_Stop();
	return SW_I2C_RET_TRUE;
}

#endif 
