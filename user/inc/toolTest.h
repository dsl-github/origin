#ifndef	__TOOL_TEST_H__
#define	__TOOL_TEST_H__

#include "pvtInt.h"

#define TOOL_TEST //工具测试功能

/* 工具测试信息上传 */
void vToolTestInfoUploadPolling_10ms(void);
/* 工具测试信息解析 */
void vToolTestSetInfoParse(u8 *pucInfo,u16 usLen);

#endif
