#ifndef	__EEPROM_H__
#define	__EEPROM_H__

#include "pvtInt.h"

/************************** ISP/IAP ***********************************
     �ͺ�        ��С   ������  ��ʼ��ַ  ������ַ   
STC15F/L2K08S2   53K   106����  0x0000  ~  0xD3FF        
STC15F/L2K16S2   45K    90����  0x0000  ~  0xB3FF        
STC15F/L2K24S2   37K    74����  0x0000  ~  0x93FF        
STC15F/L2K32S2   29K    58����  0x0000  ~  0x73FF        
STC15F/L2K40S2   21K    42����  0x0000  ~  0x53FF        
STC15F/L2K48S2   13K    26����  0x0000  ~  0x33FF        
STC15F/L2K56S2   5K     10����  0x0000  ~  0x13FF        
STC15F/L2K60S2   1K      2����  0x0000  ~  0x03FF        

STC15F/L2K61S2   ��EPROM, ����122������FLASH�����Բ�д ��ַ 0x0000~0xF3ff.
IAPϵ�� ������Ӧ�ó����޸�Ӧ�ó���
***********************************************************************/

void vEepromSectorErase(u16 usAddr);
void vEepromReadBytes(u16 usAddr, u8 *pucDat, u16 usLen);
void vEepromWriteBytes(u16 usAddr, u8 *pucDat, u16 usLen);


#endif