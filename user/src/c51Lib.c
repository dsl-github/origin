#include "c51Lib.h"

/* 查找字符位置 */
u8 *pFindAsc(u8 *pucInfo, u8 ucAsc)
{
	while (*pucInfo!='\0')
	{
		if (*pucInfo==ucAsc)
		{
			return pucInfo;
		}
		pucInfo++;
	}
	return NULL;
}

/* 内存拷贝 */
void vMemCopy(u8 *pucDes, u8 *pucSrc, u16 usLen)
{
	u16 i;
	for (i = 0; i < usLen; i++)
	{
		pucDes[i] = pucSrc[i];
	}
}

/* 内存比较 */
u8 ucMemCompare(u8 *pucS1, u8 *pucS2, u16 usLen)
{
	u16 i;
	for (i = 0; i < usLen; i++)
	{
		if (pucS1[i] != pucS2[i])
		{
			return FALSE;
		}
	}
	return TRUE;
}