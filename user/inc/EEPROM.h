#ifndef	__EEPROM_H__
#define	__EEPROM_H__

#include "pvtInt.h"

/************************** ISP/IAP ***********************************
     型号        大小   扇区数  开始地址  结束地址   
STC15F/L2K08S2   53K   106扇区  0x0000  ~  0xD3FF        
STC15F/L2K16S2   45K    90扇区  0x0000  ~  0xB3FF        
STC15F/L2K24S2   37K    74扇区  0x0000  ~  0x93FF        
STC15F/L2K32S2   29K    58扇区  0x0000  ~  0x73FF        
STC15F/L2K40S2   21K    42扇区  0x0000  ~  0x53FF        
STC15F/L2K48S2   13K    26扇区  0x0000  ~  0x33FF        
STC15F/L2K56S2   5K     10扇区  0x0000  ~  0x13FF        
STC15F/L2K60S2   1K      2扇区  0x0000  ~  0x03FF        

STC15F/L2K61S2   无EPROM, 整个122扇区的FLASH都可以擦写 地址 0x0000~0xF3ff.
IAP系列 可以在应用程序修改应用程序
***********************************************************************/

void vEepromSectorErase(u16 usAddr);
void vEepromReadBytes(u16 usAddr, u8 *pucDat, u16 usLen);
void vEepromWriteBytes(u16 usAddr, u8 *pucDat, u16 usLen);


#endif