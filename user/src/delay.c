#include "config.h"

/* ms��ʱ��������֧��1~255ms */
void delay_ms(u8 ms)
{
	u16 i;
	do
	{
		i = MAIN_FOSC / 13000;
		while (--i); // 14T per loop
	} while (--ms);
}
