#ifndef	__C51_LIB_H__
#define	__C51_LIB_H__

#include "pvtInt.h"

/* �����ַ�λ�� */
u8 *pFindAsc(u8 *pucInfo, u8 ucAsc);
/* �ڴ濽�� */
void vMemCopy(u8 *pucDes, u8 *pucSrc, u16 usLen);
/* �ڴ�Ƚ� */
u8 ucMemCompare(u8 *pucS1, u8 *pucS2, u16 usLen);

#endif
