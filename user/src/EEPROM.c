#include "config.h"

static void vEepromDisable(void)
{
	IAP_Disable();		// 禁止ISP/IAP功能
	IAP_Op_None();		// 去除ISP/IAP命令
	IAP_Not_Trig();		// 防止ISP/IAP命令误触发
	IAP_WrAddr(0xFFFF); // 指向非EEPROM区，防止误操作
}

void vEepromSectorErase(u16 usAddr)
{
	Set_IRQ_EA(0); // 禁止中断
	IAP_Enable();  // 设置等待时间，允许ISP/IAP操作，送一次就够
	// 只有扇区擦除，没有字节擦除，512字节/扇区。
	// 扇区中任意一个字节地址都是扇区地址。
	IAP_WrAddr(usAddr); // 送地址(地址需要改变时才需重新送地址)
	IAP_Op_Erase();		// 送扇区擦除命令(命令不需改变时，不需重新送命令)
	IAP_Trig_Op();		// ISP/IAP触发寄存器
	vEepromDisable();
	Set_IRQ_EA(1); // 重新允许中断
}

void vEepromReadBytes(u16 usAddr, u8 *pucDat, u16 usLen)
{
	Set_IRQ_EA(0); // 禁止中断
	IAP_Enable();  // 设置等待时间，允许ISP/IAP操作，送一次就够
	IAP_Op_Read(); // 送字节读命令，命令不需改变时，不需重新送命令
	do
	{
		IAP_WrAddr(usAddr);		// 送地址（地址需要改变时才需重新送地址）
		IAP_Trig_Op();			// ISP/IAP触发寄存器
		*pucDat = IAP_RdByte(); // 读出的数据送往
		usAddr++;
		pucDat++;
	} while (--usLen);

	vEepromDisable();
	Set_IRQ_EA(1); // 重新允许中断
}

void vEepromWriteBytes(u16 usAddr, u8 *pucDat, u16 usLen)
{
	Set_IRQ_EA(0);	// 禁止中断
	IAP_Enable();	// 设置等待时间，允许ISP/IAP操作，送一次就够
	IAP_Op_Write(); // 送字节写命令，命令不需改变时，不需重新送命令
	do
	{
		IAP_WrAddr(usAddr);	 // 送地址（地址需要改变时才需重新送地址）
		IAP_Trig_Op();		 // ISP/IAP触发寄存器
		IAP_WrByte(*pucDat); // 送数据到ISP_DATA，只有数据改变时才需重新送
		IAP_Trig_Op();		 // ISP/IAP触发寄存器
		usAddr++;
		pucDat++;
	} while (--usLen);

	vEepromDisable();
	Set_IRQ_EA(1); // 重新允许中断
}
