#ifndef	__BOARD_INIT_H__
#define	__BOARD_INIT_H__

#include "config.h"



/* GPIO Init */



void vExt0Confit(void);




u8 ucGetSysTick10msFlg(void);
void vSetSysTick10msFlg(u8 sta);

void vPCA_Init(void);
void vFlowDebugLog(void);

#endif
