#ifndef	__C51_LIB_H__
#define	__C51_LIB_H__

#include "pvtInt.h"

/* 查找字符位置 */
u8 *pFindAsc(u8 *pucInfo, u8 ucAsc);
/* 内存拷贝 */
void vMemCopy(u8 *pucDes, u8 *pucSrc, u16 usLen);
/* 内存比较 */
u8 ucMemCompare(u8 *pucS1, u8 *pucS2, u16 usLen);

#endif
