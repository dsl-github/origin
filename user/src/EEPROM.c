#include "config.h"

static void vEepromDisable(void)
{
	IAP_Disable();		// ��ֹISP/IAP����
	IAP_Op_None();		// ȥ��ISP/IAP����
	IAP_Not_Trig();		// ��ֹISP/IAP�����󴥷�
	IAP_WrAddr(0xFFFF); // ָ���EEPROM������ֹ�����
}

void vEepromSectorErase(u16 usAddr)
{
	Set_IRQ_EA(0); // ��ֹ�ж�
	IAP_Enable();  // ���õȴ�ʱ�䣬����ISP/IAP��������һ�ξ͹�
	// ֻ������������û���ֽڲ�����512�ֽ�/������
	// ����������һ���ֽڵ�ַ����������ַ��
	IAP_WrAddr(usAddr); // �͵�ַ(��ַ��Ҫ�ı�ʱ���������͵�ַ)
	IAP_Op_Erase();		// ��������������(�����ı�ʱ����������������)
	IAP_Trig_Op();		// ISP/IAP�����Ĵ���
	vEepromDisable();
	Set_IRQ_EA(1); // ���������ж�
}

void vEepromReadBytes(u16 usAddr, u8 *pucDat, u16 usLen)
{
	Set_IRQ_EA(0); // ��ֹ�ж�
	IAP_Enable();  // ���õȴ�ʱ�䣬����ISP/IAP��������һ�ξ͹�
	IAP_Op_Read(); // ���ֽڶ���������ı�ʱ����������������
	do
	{
		IAP_WrAddr(usAddr);		// �͵�ַ����ַ��Ҫ�ı�ʱ���������͵�ַ��
		IAP_Trig_Op();			// ISP/IAP�����Ĵ���
		*pucDat = IAP_RdByte(); // ��������������
		usAddr++;
		pucDat++;
	} while (--usLen);

	vEepromDisable();
	Set_IRQ_EA(1); // ���������ж�
}

void vEepromWriteBytes(u16 usAddr, u8 *pucDat, u16 usLen)
{
	Set_IRQ_EA(0);	// ��ֹ�ж�
	IAP_Enable();	// ���õȴ�ʱ�䣬����ISP/IAP��������һ�ξ͹�
	IAP_Op_Write(); // ���ֽ�д��������ı�ʱ����������������
	do
	{
		IAP_WrAddr(usAddr);	 // �͵�ַ����ַ��Ҫ�ı�ʱ���������͵�ַ��
		IAP_Trig_Op();		 // ISP/IAP�����Ĵ���
		IAP_WrByte(*pucDat); // �����ݵ�ISP_DATA��ֻ�����ݸı�ʱ����������
		IAP_Trig_Op();		 // ISP/IAP�����Ĵ���
		usAddr++;
		pucDat++;
	} while (--usLen);

	vEepromDisable();
	Set_IRQ_EA(1); // ���������ж�
}
