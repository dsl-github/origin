#ifndef	__REG_DEF_H__
#define	__REG_DEF_H__

#include "STC15.H"
#include <intrins.h>

/*   interrupt vector */
#define		INT0_VECTOR		0
#define		TIMER0_VECTOR	1
#define		INT1_VECTOR		2
#define		TIMER1_VECTOR	3
#define		UART1_VECTOR	4
#define		ADC_VECTOR		5
#define		LVD_VECTOR		6
#define		PCA_VECTOR		7
#define		UART2_VECTOR	8
#define		SPI_VECTOR		9
#define		INT2_VECTOR		10
#define		INT3_VECTOR		11
#define		TIMER2_VECTOR	12
#define		INT4_VECTOR		16
#define		UART3_VECTOR	17
#define		UART4_VECTOR	18
#define		TIMER3_VECTOR	19
#define		TIMER4_VECTOR	20
#define		CMP_VECTOR		21

/* ************************************************** Timer0/1 ************************************************** */
/* THx/TLx */
#define		Timer0_Load(n)		TH0 = (n) / 256,	TL0 = (n) % 256     //装载定时器0初值
#define		Timer1_Load(n)		TH1 = (n) / 256,	TL1 = (n) % 256     //装载定时器1初值
#define		Timer2_Load(n)		T2H = (n) / 256,	T2L = (n) % 256     //装载定时器2初值

#define		Timer0_Read_TC()	((u16)TH0<<8)|TL0 //读取定时器0计数器
#define		Timer1_Read_TC()	((u16)TH1<<8)|TL1 //读取定时器1计数器
#define		Timer2_Read_TC()	((u16)T2H<<8)|T2L //读取定时器2计数器

//						  7      6      5      4      3      2     1     0        Reset Value
//sfr TCON = 0x88;		 TF1    TR1    TF0    TR0    IE1    IT1   IE0   IT0       0000,0000B  
#define		Timer1_Run()			            TR1 = 1             //允许定时器1计数
#define		Timer1_Stop()			            TR1 = 0             //禁止定时器1计数
#define		Timer0_Run()			            TR0 = 1             //允许定时器0计数
#define		Timer0_Stop()			            TR0 = 0             //禁止定时器0计数
#define     EXT1_Mode_Fall()                    IT1 = 1             //外部中断1下降沿触发
#define     EXT1_Mode_RiseFall()                IT1 = 0             //外部中断1上升或下降沿均触发
#define     EXT0_Mode_Fall()                    IT0 = 1             //外部中断0下降沿触发
#define     EXT0_Mode_RiseFall()                IT0 = 0             //外部中断0上升或下降沿均触发
/* IE1、IE0、TF1、TF0硬件控制，故未解析 */

//						  7      6      5      4      3      2     1     0        Reset Value
//sfr TMOD = 0x89;		GATE    C/T     M1     M0   GATE    C/T    M1    M0       0000,0000B    
#define		Timer0_16bitAutoReload()	TMOD &= ~0x03					//16位自动重装
#define		Timer0_16bit()				TMOD  = (TMOD & ~0x03) | 0x01	//16位
#define		Timer0_8bitAutoReload()		TMOD  = (TMOD & ~0x03) | 0x02	//8位自动重装
#define		Timer0_16bitAutoRL_NoMask()	TMOD |=  0x03		            //16位自动重装不可屏蔽中断
#define		Timer0_AsCounter()			TMOD |=  4		                //定时器0用做计数器
#define		Timer0_AsTimer()			TMOD &= ~4		                //定时器0用做定时器
#define		Timer0_ExtControl()			TMOD |=  (1<<3)		            //定时器0由外部INT0高电平允许定时计数
#define		Timer1_16bitAutoReload()	TMOD &= ~0x30					//16位自动重装
#define		Timer1_16bit()				TMOD  = (TMOD & ~0x30) | 0x10	//16位
#define		Timer1_8bitAutoReload()		TMOD  = (TMOD & ~0x30) | 0x20	//8位自动重装
#define		Timer1_16bitAutoRL_NoMask()	TMOD |=  0x30		            //16位自动重装不可屏蔽中断
#define		Timer1_AsCounter()			TMOD |=  (1<<6)		            //定时器1用做计数器
#define		Timer1_AsTimer()			TMOD &= ~(1<<6)		            //定时器1用做定时器
#define		Timer1_ExtControl()			TMOD |=  (1<<7)		            //定时器1由外部INT1高电平允许定时计数


/* **************************************************    IO    ************************************************** */
#define	GPIO_Px0		0x01	//IO引脚 Px.0
#define	GPIO_Px1		0x02	//IO引脚 Px.1
#define	GPIO_Px2		0x04	//IO引脚 Px.2
#define	GPIO_Px3		0x08	//IO引脚 Px.3
#define	GPIO_Px4		0x10	//IO引脚 Px.4
#define	GPIO_Px5		0x20	//IO引脚 Px.5
#define	GPIO_Px6		0x40	//IO引脚 Px.6
#define	GPIO_Px7		0x80	//IO引脚 Px.7
#define	GPIO_All	    0xFF	//IO所有引脚

/* PxM1.n,PxM0.n 	=00--->Standard,	01--->push-pull						
                    =10--->pure input,	11--->open drain          */
#define     P0_IO_PullUp_Set(pin)    P0M1 &= ~(pin),P0M0 &= ~(pin)//上拉准双向口
#define     P0_IN_HighZ_Set(pin)     P0M1 |=  (pin),P0M0 &= ~(pin)//浮空输入
#define     P0_OUT_OD_Set(pin)       P0M1 |=  (pin),P0M0 |=  (pin)//开漏输出
#define     P0_OUT_PP_Set(pin)       P0M1 &= ~(pin),P0M0 |=  (pin)//推挽输出

#define     P1_IO_PullUp_Set(pin)    P1M1 &= ~(pin),P1M0 &= ~(pin)//上拉准双向口
#define     P1_IN_HighZ_Set(pin)     P1M1 |=  (pin),P1M0 &= ~(pin)//浮空输入
#define     P1_OUT_OD_Set(pin)       P1M1 |=  (pin),P1M0 |=  (pin)//开漏输出
#define     P1_OUT_PP_Set(pin)       P1M1 &= ~(pin),P1M0 |=  (pin)//推挽输出

#define     P2_IO_PullUp_Set(pin)    P2M1 &= ~(pin),P2M0 &= ~(pin)//上拉准双向口
#define     P2_IN_HighZ_Set(pin)     P2M1 |=  (pin),P2M0 &= ~(pin)//浮空输入
#define     P2_OUT_OD_Set(pin)       P2M1 |=  (pin),P2M0 |=  (pin)//开漏输出
#define     P2_OUT_PP_Set(pin)       P2M1 &= ~(pin),P2M0 |=  (pin)//推挽输出

#define     P3_IO_PullUp_Set(pin)    P3M1 &= ~(pin),P3M0 &= ~(pin)//上拉准双向口
#define     P3_IN_HighZ_Set(pin)     P3M1 |=  (pin),P3M0 &= ~(pin)//浮空输入
#define     P3_OUT_OD_Set(pin)       P3M1 |=  (pin),P3M0 |=  (pin)//开漏输出
#define     P3_OUT_PP_Set(pin)       P3M1 &= ~(pin),P3M0 |=  (pin)//推挽输出

#define     P4_IO_PullUp_Set(pin)    P4M1 &= ~(pin),P4M0 &= ~(pin)//上拉准双向口
#define     P4_IN_HighZ_Set(pin)     P4M1 |=  (pin),P4M0 &= ~(pin)//浮空输入
#define     P4_OUT_OD_Set(pin)       P4M1 |=  (pin),P4M0 |=  (pin)//开漏输出
#define     P4_OUT_PP_Set(pin)       P4M1 &= ~(pin),P4M0 |=  (pin)//推挽输出

#define     P5_IO_PullUp_Set(pin)    P5M1 &= ~(pin),P5M0 &= ~(pin)//上拉准双向口
#define     P5_IN_HighZ_Set(pin)     P5M1 |=  (pin),P5M0 &= ~(pin)//浮空输入
#define     P5_OUT_OD_Set(pin)       P5M1 |=  (pin),P5M0 |=  (pin)//开漏输出
#define     P5_OUT_PP_Set(pin)       P5M1 &= ~(pin),P5M0 |=  (pin)//推挽输出

/* **************************************************   ADC    ************************************************** */
#define     ADC_Use_P1x(pin)		P1ASF = pin		//设置P1.x为ADC输入
//								7       6      5       4         3      2    1    0   Reset Value
//sfr ADC_CONTR = 0xBC;		ADC_POWER SPEED1 SPEED0 ADC_FLAG ADC_START CHS2 CHS1 CHS0 0000,0000	//AD 转换控制寄存器 
#define     ADC_PWR_On()			ADC_CONTR |=  (1<<7)		//ADC电源开启
#define     ADC_PWR_Off()			ADC_CONTR &= ~(1<<7)		//ADC电源关闭
#define     ADC_Speed_90T()			ADC_CONTR |=  (3<<5)						//ADC转换周期为90个时钟周期
#define     ADC_Speed_180T()		ADC_CONTR = (ADC_CONTR & ~(3<<5)) | (2<<5)	//ADC转换周期为180个时钟周期
#define     ADC_Speed_360T()		ADC_CONTR = (ADC_CONTR & ~(3<<5)) | (1<<5)	//ADC转换周期为360个时钟周期
#define     ADC_Speed_540T()		ADC_CONTR &= ~(0<<5)						//ADC转换周期为540个时钟周期
#define		Get_ADC_FIF()			(ADC_CONTR&0x10)            //获取ADC转换完成中断标志
#define		Clr_PCA_FIF()			ADC_CONTR &= ~0x10          //软件清零ADC转换完成中断标志
#define     ADC_Run()				ADC_CONTR |=  (1<<3)		//ADC启动转换
#define     ADC_Stop()				ADC_CONTR &= ~(1<<3)		//ADC停止转换
#define     ADC_Channel(chl)        ADC_CONTR = (ADC_CONTR & 0x07) | (chl)		//ADC选择转换通道

/* ADC结果转换 */
#define     Get_ADC_Result_H2L8()        (((u16)(ADC_RES & 3)<<8)|ADC_RESL)
#define     Get_ADC_Result_H8L2()        (((u16)ADC_RES<<2)|(ADC_RESL&3))

/* **************************************************   UART   ************************************************** */
//						  7      6      5      4      3      2     1     0        Reset Value
//sfr SCON = 0x98;		 SM0    SM1    SM2    REN    TB8    RB8    TI    RI       0000,0000B    //S1 Control
#define		UART1_8Bit()			    SCON  = (SCON & 0x3f) | 0x40
#define		UART1_9Bit()			    SCON  = (SCON & 0x3f) | 0xc0
#define		UART1_Rx_Enable()		    SCON |=  (1<<4)         //允许串1接收
#define		UART1_Rx_Disable()		    SCON &= ~(1<<4)		    //禁止串1接收
#define		Get_UART1_TxIF()			(SCON & 2)              //获取串1发送中断标志位
#define		Set_UART1_TxIF()			SCON |= 2               //触发串1发送中断
#define		Clr_UART1_TxIF()			SCON &= ~2              //软件清零串1发送中断标志位
#define		Get_UART1_RxIF()			(SCON & 1)              //获取串1接收中断标志位
#define		Clr_UART1_RxIF()			SCON &= ~1              //软件清零串1接收中断标志位
/* SM2、TB8、RB8很少使用，故未解析 */

#define     UART1_WrByte(dat)   SBUF = (dat)
#define     UART1_RdByte()      SBUF

//						  7      6      5      4      3      2     1     0        Reset Value
//sfr S2CON = 0x9A;		S2SM0    -    S2SM2  S2REN  S2TB8  S2RB8  S2TI  S2RI      0000,0000B    //S2 Control
#define		UART2_8Bit()			    S2CON &= ~(1<<7)		//串口2模式0，8位UART，波特率 = 定时器2的溢出率 / 4
#define		UART2_9Bit()			    S2CON |=  (1<<7)		//串口2模式1，9位UART，波特率 = 定时器2的溢出率 / 4
#define		UART2_Rx_Enable()		    S2CON |=  (1<<4)		//允许串2接收
#define		UART2_Rx_Disable()		    S2CON &= ~(1<<4)		//禁止串2接收
#define		Get_UART2_TxIF()			(S2CON & 2)             //获取串2发送中断标志位
#define		Set_UART2_TxIF()			S2CON |= 2              //触发串2发送中断
#define		Clr_UART2_TxIF()			S2CON &= ~2             //软件清零串2发送中断标志位
#define		Get_UART2_RxIF()			(S2CON & 1)             //获取串2接收中断标志位
#define		Clr_UART2_RxIF()			S2CON &= ~1             //软件清零串2接收中断标志位
/* S2SM2、S2TB8、S2RB8很少使用，故未解析 */

#define     UART2_WrByte(dat)   S2BUF = (dat)
#define     UART2_RdByte()      S2BUF

/* **************************************************   AUXR   ************************************************** */
//						   7     6       5      4     3      2      1      0    Reset Value
//sfr AUXR  = 0x8E;		T0x12 T1x12 UART_M0x6  T2R  T2_C/T T2x12 EXTRAM  S1ST2  0000,0000	//Auxiliary Register 
#define 	Timer0_CLK_1T()					AUXR |=  (1<<7)	//Timer0 clodk = fo
#define 	Timer0_CLK_12T()				AUXR &= ~(1<<7)	//Timer0 clodk = fo/12	12分频,	default
#define 	Timer1_CLK_1T()					AUXR |=  (1<<6)	//Timer1 clodk = fo
#define 	Timer1_CLK_12T()				AUXR &= ~(1<<6)	//Timer1 clodk = fo/12	12分频,	default
#define 	Timer2_Run()	 			    AUXR |=  (1<<4)	//允许定时器2计数
#define 	Timer2_Stop()	 			    AUXR &= ~(1<<4)	//禁止定时器2计数
#define		Timer2_AsCounter()		        AUXR |=  (1<<3)	//时器2用做计数器,对T2/P3.1外部脉冲计数
#define		Timer2_AsTimer()			    AUXR &= ~(1<<3)	//时器2用做定时器
#define 	Timer2_CLK_1T()					AUXR |=  (1<<2)	//Timer0 clodk = fo
#define 	Timer2_CLK_12T()				AUXR &= ~(1<<2)	//Timer0 clodk = fo/12	12分频,	default
#define 	ExternalRAM_Enable()		    AUXR |=  2		//允许外部XRAM，禁止使用内部1024RAM
#define 	InternalRAM_Enable()		    AUXR &= ~2		//禁止外部XRAM，允许使用内部1024RAM
#define 	UART1_BRT_UseTimer2()	        AUXR |=  1      //定时器2作为串1波特率发生器
#define 	UART1_BRT_UseTimer1()	        AUXR &= ~1      //定时器1作为串1波特率发生器
/* UART_M0x6很少使用，故未解析 */


//					            7      6       5        4       3       2       1      0    Reset Value
//sfr AUXR1/P_SW1 = 0xA2;	  S1_S1  S1_S0  CCP_S1   CCP_S0  SPI_S1   SPI_S0    -     DPS   0100,0000	//Auxiliary Register 1
#define		UART1_Use_P30P31()	    P_SW1 &= ~0xc0					//UART1 使用P30 P31口	默认
#define		UART1_Use_P36P37()		P_SW1 = (P_SW1 & ~0xc0) | 0x40	//UART1 使用P36 P37口
#define		UART1_Use_P16P17()	    P_SW1 = (P_SW1 & ~0xc0) | 0x80	//UART1 使用P16 P17口
#define		PCA_Use_P12P11P10P37()	AUXR1 &= ~0x30		            //将PCA/PWM切换到P12(ECI) P11(CCP0) P10(CCP1) P37(CCP2)(上电默认)
#define		PCA_Use_P34P35P36P37()	AUXR1 = (AUXR1 & ~0x30) | 0x10	//将PCA/PWM切换到P34(ECI) P35(CCP0) P36(CCP1) P37(CCP2)
#define		PCA_Use_P24P25P26P27()	AUXR1 = (AUXR1 & ~0x30) | 0x20	//将PCA/PWM切换到P24(ECI) P25(CCP0) P26(CCP1) P27(CCP2)
#define		SPI_USE_P12P13P14P15()	AUXR1 &= ~0x0c	                //将SPI切换到P12(SS) P13(MOSI) P14(MISO) P15(SCLK)(上电默认)
#define		SPI_USE_P24P23P22P21()	AUXR1 = (AUXR1 & ~0x0c) | 0x04	//将SPI切换到P24(SS) P23(MOSI) P22(MISO) P21(SCLK)
#define		SPI_USE_P54P40P41P43()	AUXR1 = (AUXR1 & ~0x0c) | 0x08	//将SPI切换到P54(SS) P40(MOSI) P41(MISO) P43(SCLK)
/* DPS暂未使用，故未解析 */


//                                  7   6    5    4   3     2        1       0         Reset Value
//sfr INT_CLKO/AUXR2 = 0x8F;	    -  EX4  EX3  EX2  -   T2CLKO   T1CLKO  T0CLKO      0000,0000B
#define		EXT4_IRQ_Enable()		        INT_CLKO |=  (1<<6)	//允许外部中断4中断
#define		EXT4_IRQ_Disable()	            INT_CLKO &= ~(1<<6)	//禁止外部中断4中断
#define		EXT3_IRQ_Enable()		        INT_CLKO |=  (1<<5)	//允许外部中断3中断
#define		EXT3_IRQ_Disable()	            INT_CLKO &= ~(1<<5)	//禁止外部中断3中断
#define		EXT2_IRQ_Enable()		        INT_CLKO |=  (1<<4)	//允许外部中断2中断
#define		EXT2_IRQ_Disable()	            INT_CLKO &= ~(1<<4)	//禁止外部中断2中断
#define		Timer2_CLKOUT_P30_Enable()		INT_CLKO |=  (1<<2)   //允许P3.0配置成定时器2时钟输出，Fck2 = 1/2 T2 溢出率
#define		Timer2_CLKOUT_P30_Disable()		INT_CLKO &= ~(1<<2)   //禁止P3.0配置成定时器2时钟输出
#define		Timer1_CLKOUT_P34_Enable()		INT_CLKO |=  (1<<1)   //允许P3.4配置成定时器1时钟输出，Fck1 = 1/2 T1 溢出率
#define		Timer1_CLKOUT_P34_Disable()		INT_CLKO &= ~(1<<1)   //禁止P3.4配置成定时器1时钟输出
#define		Timer0_CLKOUT_P35_Enable()		INT_CLKO |=  (1<<0)   //允许P3.5配置成定时器0时钟输出，Fck0 = 1/2 T0 溢出率
#define		Timer0_CLKOUT_P35_Disable()		INT_CLKO &= ~(1<<0)   //禁止P3.5配置成定时器0时钟输出


//						   7     6       5      4     3      2      1      0    Reset Value
//sfr P_SW2  = 0xBA;	EAXSFR DBLPWR  P31PU   P30PU  -    S4_S    S3_S   S2_S  0000,0000	//端口配置寄存器
#define		UART2_Use_P10P11()		P_SW2 &= ~1		//UART2 使用P1.0/RxD2,P1.1/TxD2口	默认
#define		UART2_Use_P46P47()		P_SW2 |=  1		//UART2 使用P4.6/RxD2_2,P4.7/TxD2_2口
#define		UART3_Use_P00P01()		P_SW2 &= ~2		//UART3 使用P0.0/RxD3,P0.1/TxD3口	默认
#define		UART3_Use_P50P51()		P_SW2 |=  2		//UART3 使用P5.0/RxD3_2,P5.1/TxD3_2口
#define		UART4_Use_P02P03()		P_SW2 &= ~4		//UART4 使用P0.2/RxD4,P0.3/TxD4口	默认
#define		UART4_Use_P52P53()		P_SW2 |=  4		//UART4 使用P5.2/RxD4_2,P5.3/TxD4_2口
/* DBLPWR、P31PU、P30P未知，EAXSFR暂未使用，故未解析 */


/* **************************************************   IE/IP  ************************************************** */
//						  7     6     5    4    3    2    1     0    Reset Value
//sfr IE  = 0xA8;		 EA   ELVD  EADC   ES  ET1  EX1  ET0   EX0   0000,0000B	//中断允许寄存器(可位寻址)  
#define		Set_IRQ_EA(n)		    EA   = (n)	//中断允许总控制位
#define		LVD_IRQ_Enable()		ELVD =  1	//允许低压监测中断
#define		LVD_IRQ_Disable()		ELVD =  0	//禁止低压监测中断
#define		ADC_IRQ_Enable()		EADC =  1	//允许ADC中断
#define		ADC_IRQ_Disable()		EADC =  0	//禁止ADC中断
#define		UART1_IRQ_Enable()		ES   =  1	//允许串口1中断
#define		UART1_IRQ_Disable()		ES   =  0	//禁止串口1中断
#define		Timer1_IRQ_Enable()		ET1  =  1	//允许定时器1中断
#define		Timer1_IRQ_Disable()	ET1  =  0	//禁止定时器1中断
#define		EXT1_IRQ_Enable()		EX1  =  1	//允许外部中断1中断
#define		EXT1_IRQ_Disable()	    EX1  =  0	//禁止外部中断1中断
#define		Timer0_IRQ_Enable()		ET0  =  1	//允许定时器0中断
#define		Timer0_IRQ_Disable()	ET0  =  0	//禁止定时器0中断
#define		EXT0_IRQ_Enable()		EX0  =  1	//允许外部中断1中断
#define		EXT0_IRQ_Disable()	    EX0  =  0	//禁止外部中断1中断


//						  7     6     5    4    3    2    1     0    Reset Value
//sfr IE2  = 0xAF;		  -    ET4   ET3  ES4  ES3  ET2  ESPI  ES2   0000,0000B	//中断允许寄存器(不可位寻址)  
#define		Timer4_IRQ_Enable()		IE2 |=  (1<<6)	//允许定时器4中断
#define		Timer4_IRQ_Disable()	IE2 &= ~(1<<6)	//禁止定时器4中断
#define		Timer3_IRQ_Enable()		IE2 |=  (1<<5)	//允许定时器3中断
#define		Timer3_IRQ_Disable()	IE2 &= ~(1<<5)	//禁止定时器3中断
#define		UART4_IRQ_Enable()		IE2 |=  (1<<4)	//允许串口4中断
#define		UART4_IRQ_Disable()		IE2 &= ~(1<<4)	//禁止串口4中断
#define		UART3_IRQ_Enable()		IE2 |=  (1<<3)	//允许串口3中断
#define		UART3_IRQ_Disable()		IE2 &= ~(1<<3)	//禁止串口3中断
#define		Timer2_IRQ_Enable()		IE2 |=  (1<<2)	//允许定时器2中断
#define		Timer2_IRQ_Disable()	IE2 &= ~(1<<2)	//禁止定时器2中断
#define		SPI_IRQ_Enable()		IE2 |=  2	//允许SPI中断
#define		SPI_IRQ_Disable()		IE2 &= ~2	//禁止SPI中断
#define		UART2_IRQ_Enable()		IE2 |=  1	//允许串口2中断
#define		UART2_IRQ_Disable()		IE2 &= ~1	//禁止串口2中断


//                                          7     6     5    4    3    2    1    0    Reset Value
//sfr IP      = 0xB8; //中断优先级低位      PPCA  PLVD  PADC  PS   PT1  PX1  PT0  PX0   0000,0000
#define     PCA_SetHighPolity()         PPCA = 1     //高优先级中断
#define     PCA_SetLowPolity()          PPCA = 0     //低优先级中断
#define     LVD_SetHighPolity()         PLVD = 1     //高优先级中断
#define     LVD_SetLowPolity()          PLVD = 0     //低优先级中断
#define     ADC_SetHighPolity()         PADC = 1     //高优先级中断
#define     ADC_SetLowPolity()          PADC = 0     //低优先级中断
#define     UART1_SetHighPolity()       PS = 1     //高优先级中断
#define     UART1_SetLowPolity()        PS = 0     //低优先级中断
#define     Timer1_SetHighPolity()      PT1 = 1     //高优先级中断
#define     Timer1_SetLowPolity()       PT1 = 0     //低优先级中断
#define     Ext1_SetHighPolity()        PX1 = 1     //高优先级中断
#define     Ext1_SetLowPolity()         PX1 = 0     //低优先级中断
#define     Timer0_SetHighPolity()      PT0 = 1     //高优先级中断
#define     Timer0_SetLowPolity()       PT0 = 0     //低优先级中断
#define     Ext0_SetHighPolity()        PX0 = 1     //高优先级中断
#define     Ext0_SetLowPolity()         PX0 = 0     //低优先级中断


//                                           7       6       5      4      3      2     1      0     Reset Value
//sfr IP2   = 0xB5; //                       -       -       -     PX4  PPWMFD   PPWM  PSPI   PS2    xxxx,xx00
#define     SPI_SetHighPolity()         IP2 |=  2     //高优先级中断
#define     SPI_SetLowPolity()          IP2 &= ~2     //低优先级中断
#define     UART2_SetHighPolity()       IP2 |=  1     //高优先级中断
#define     UART2_SetLowPolity()        IP2 &= ~1     //低优先级中断

/* **************************************************    PCA   ************************************************** */
//						7     6     5     4     3     2     1     0     Reset Value
//sfr CCON   = 0xD8;	CF    CR    -     -     -    CCF2  CCF1  CCF0   00xx,xx00	//PCA 控制寄存器
#define		Get_PCA0_CCIF()			CCF0            //获取PCA0匹配或捕获中断标志位
#define		Clr_PCA0_CCIF()			CCF0 = 0        //软件清零PCA0匹配或捕获中断标志位
#define		Get_PCA1_CCIF()			CCF1            //获取PCA0匹配或捕获中断标志位
#define		Clr_PCA1_CCIF()			CCF1 = 0        //软件清零PCA0匹配或捕获中断标志位
#define		Get_PCA2_CCIF()			CCF2            //获取PCA0匹配或捕获中断标志位
#define		Clr_PCA2_CCIF()			CCF2 = 0        //软件清零PCA0匹配或捕获中断标志位
#define		PCA_Timer_Stop()		CR = 0          //PCA定时器/计数器关闭
#define		PCA_Timer_Run()			CR = 1          //PCA定时器/计数器运行
#define		Get_PCA_Timer_CIF()		CF              //获取PCA定时器/计数器溢出中断标志
#define		Clr_PCA_Timer_CIF()		CF = 0          //软件清零PCA定时器/计数器溢出中断标志

//					 7     6     5     4     3     2     1     0    Reset Value
//sfr CMOD  = 0xD9;	CIDL   -     -     -   CPS2   CPS1  CPS0  ECF   0xxx,0000	//PCA 工作模式寄存器。
#define PCA_Timer_IDLE_Off()	    CMOD |=  (1<<7)				//IDLE状态PCA停止计数。
#define PCA_Timer_IDLE_On()		    CMOD &= ~(1<<7)				//IDLE状态PCA继续计数。
#define PCA_Timer_CLK_12T()		    CMOD &= ~0x0E				//PCA计数脉冲选择外部晶振/12。	fosc/12
#define PCA_Timer_CLK_2T()		    CMOD = (CMOD & ~0x0E) + 2	//PCA计数脉冲选择外部晶振/2。	fosc/2
#define PCA_Timer_CLK_T0()		    CMOD = (CMOD & ~0x0E) + 4	//PCA计数脉冲选择Timer0中断，Timer0可通过AUXR寄存器设置成工作在12T或1T模式。
#define PCA_Timer_CLK_ECI()		    CMOD = (CMOD & ~0x0E) + 6	//PCA计数脉冲选择从ECI/P3.4脚输入的外部时钟，最大 fosc/2。
#define PCA_Timer_CLK_1T()		    CMOD = (CMOD & ~0x0E) + 8	//PCA计数脉冲选择外部晶振。		Fosc/1
#define PCA_Timer_CLK_4T()		    CMOD = (CMOD & ~0x0E) + 10	//PCA计数脉冲选择外部晶振/4。	Fosc/4
#define PCA_Timer_CLK_6T()		    CMOD = (CMOD & ~0x0E) + 12	//PCA计数脉冲选择外部晶振/6。	Fosc/6
#define PCA_Timer_CLK_8T()		    CMOD = (CMOD & ~0x0E) + 14	//PCA计数脉冲选择外部晶振/8。	Fosc/8
#define PCA_Timer_IRQ_Enable()	    CMOD |=  1					//PCA计数器溢出中断允许位，1---允许CF（CCON.7）产生中断。
#define PCA_Timer_IRQ_Disable()	    CMOD &= ~1					//PCA计数器溢出中断禁止。


//									7     6      5      4     3     2     1     0     Reset Value
//sfr CCAPM0 = 0xDA;	PWM 寄存器  -   ECOM0  CAPP0  CAPN0  MAT0  TOG0  PWM0  ECCF0   x000,0000	//PCA 模块0 
//sfr CCAPM1 = 0xDB;	PWM 寄存器  -   ECOM1  CAPP1  CAPN1  MAT1  TOG1  PWM1  ECCF1   x000,0000	//PCA 模块1
//sfr CCAPM2 = 0xDC;	PWM 寄存器  -   ECOM2  CAPP2  CAPN2  MAT2  TOG2  PWM2  ECCF2   x000,0000	//PCA 模块2
// ECOMn = 1:	允许比较功能。
// CAPPn = 1:	允许上升沿触发捕捉功能。
// CAPNn = 1:	允许下降沿触发捕捉功能。
// MATn  = 1:	当匹配情况发生时，允许CCON中的CCFn置位。
// TOGn  = 1:	当匹配情况发生时，CEXn将翻转。(CEX0/PCA0/PWM0/P3.7,CEX1/PCA1/PWM1/P3.5)
// PWMn  = 1:	将CEXn设置为PWM输出。
// ECCFn = 1:	允许CCON中的CCFn触发中断。
// ECOMn CAPPn CAPNn MATn TOGn PWMn ECCFn
//   0     0     0    0    0    0     0		00H	未启用任何功能。
//   x     1     0    0    0    0     x	 	20H	16位CEXn上升沿触发捕捉功能。
//   x     0     1    0    0    0     x	 	10H	16位CEXn下降沿触发捕捉功能。
//   x     1     1    0    0    0     x	 	30H	16位CEXn/PCAn边沿（上、下沿）触发捕捉功能。
//   1     0     0    1    0    0     x	 	48H	16位软件定时器。
//   1     0     0    1    1    0     x	 	4CH	16位高速脉冲输出。
//   1     0     0    0    0    1     0		42H	8位PWM。无中断
//   1     1     0    0    0    1     1		63H	8位PWM。低变高可产生中断
//   1     0     1    0    0    1     1		53H	8位PWM。高变低可产生中断
//   1     1     1    0    0    1     1		73H	8位PWM。低变高或高变低均可产生中断
#define		PCA0_PWM_No_Int()	        CCAPM0 = 0x42
#define		PCA0_PWM_Rise_Int()         CCAPM0 = 0x63
#define		PCA0_PWM_Fall_Int()         CCAPM0 = 0x53
#define		PCA0_PWM_Edge_Int()         CCAPM0 = 0x73
#define		PCA0_Capture_Rise()		    CCAPM0 = (0x20 + 1)
#define		PCA0_IsCaptureRise()		(CCAPM0&0x20)
#define		PCA0_Capture_Fall()		    CCAPM0 = (0x10 + 1)
#define		PCA0_IsCaptureFall()		(CCAPM0&0x10)
#define		PCA0_Capture_Edge()		    CCAPM0 = (0x30 + 1)
#define		PCA0_IsCaptureEdge()		(CCAPM0&0x30)
#define		PCA0_16bit_Timer()		    CCAPM0 = (0x48 + 1)
#define		PCA0_High_Pulse()		    CCAPM0 = (0x4C + 1)

#define		PCA1_PWM_No_Int()	        CCAPM1 = 0x42
#define		PCA1_PWM_Rise_Int()         CCAPM1 = 0x63
#define		PCA1_PWM_Fall_Int()         CCAPM1 = 0x53
#define		PCA1_PWM_Edge_Int()         CCAPM1 = 0x73
#define		PCA1_Capture_Rise()		    CCAPM1 = (0x20 + 1)
#define		PCA1_Capture_Fall()		    CCAPM1 = (0x10 + 1)
#define		PCA1_Capture_Edge()		    CCAPM1 = (0x30 + 1)
#define		PCA1_16bit_Timer()		    CCAPM1 = (0x48 + 1)
#define		PCA1_High_Pulse()		    CCAPM1 = (0x4C + 1)

#define		PCA2_PWM_No_Int()	        CCAPM2 = 0x42
#define		PCA2_PWM_Rise_Int()         CCAPM2 = 0x63
#define		PCA2_PWM_Fall_Int()         CCAPM2 = 0x53
#define		PCA2_PWM_Edge_Int()         CCAPM2 = 0x73
#define		PCA2_Capture_Rise()		    CCAPM2 = (0x20 + 1)
#define		PCA2_Capture_Fall()		    CCAPM2 = (0x10 + 1)
#define		PCA2_Capture_Edge()		    CCAPM2 = (0x30 + 1)
#define		PCA2_16bit_Timer()		    CCAPM2 = (0x48 + 1)
#define		PCA2_High_Pulse()		    CCAPM2 = (0x4C + 1)

/* 定时器初值 */
#define     Set_PCA_TimerVal(val)	CL = (u8)val,CH = (u8)(val >> 8)
/* 设置占空比 */
#define     Set_PCA0_PWM_DutyRatio(pwmBit,ratio)   CCAP0H = (1UL<<pwmBit)*(ratio)/100
#define     Set_PCA1_PWM_DutyRatio(pwmBit,ratio)   CCAP1H = (1UL<<pwmBit)*(ratio)/100
#define     Set_PCA2_PWM_DutyRatio(pwmBit,ratio)   CCAP2H = (1UL<<pwmBit)*(ratio)/100

/* 设置比较值 */
#define     Set_PCA0_CaptureVal(val)   CCAP0L = (u8)val,CCAP0H = (u8)(val >> 8)//先写CCAP0L,后写CCAP0H
#define     Set_PCA1_CaptureVal(val)   CCAP1L = (u8)val,CCAP1H = (u8)(val >> 8)//先写CCAP1L,后写CCAP1H
#define     Set_PCA2_CaptureVal(val)   CCAP2L = (u8)val,CCAP2H = (u8)(val >> 8)//先写CCAP2L,后写CCAP2H

/* 获取比较值 */
#define     Get_PCA0_CaptureVal() ((u16)CCAP0H<<8)|CCAP0L
#define     Get_PCA1_CaptureVal() ((u16)CCAP1H<<8)|CCAP1L
#define     Get_PCA2_CaptureVal() ((u16)CCAP2H<<8)|CCAP2L

//                7       6     5   4   3   2     1       0
// PCA_PWMn:	EBSn_1	EBSn_0	-	-	-	-	EPCnH	EPCnL
// B1(EPCnH):	在PWM模式下，与CCAPnH组成9位数。
// B0(EPCnL):	在PWM模式下，与CCAPnL组成9位数。
// PWM模式:1.(0,CL[n:0])>=(EPCnL,CCPnL[n:0])输出1;
//         2.(0,CL[n:0])<(EPCnL,CCPnL[n:0])输出0;
//         3.CL溢出时(EPCnH,CCPnH[n:0])加载至(EPCnL,CCPnL[n:0])

#define		PWM0_8Bit()	        PCA_PWM0 = (PCA_PWM0 & 0x0c) | (0<<6)
#define		PWM0_7Bit()	        PCA_PWM0 = (PCA_PWM0 & 0x0c) | (1<<6)
#define		PWM0_6Bit()	        PCA_PWM0 = (PCA_PWM0 & 0x0c) | (2<<6)
#define		PWM0_OUT_NORMAL()	PCA_PWM0 &= ~3				//PWM0正常输出(默认)
#define		PWM0_OUT_0()	    PCA_PWM0 |=  3				//PWM0一直输出0
#define		PWM0_OUT_1()	    PCA_PWM0 &= ~3, CCAP0H = 0	//PWM0一直输出1

#define		PWM1_8Bit()	        PCA_PWM1 = (PCA_PWM1 & 0x0c) | (0<<6)
#define		PWM1_7Bit()	        PCA_PWM1 = (PCA_PWM1 & 0x0c) | (1<<6)
#define		PWM1_6Bit()	        PCA_PWM1 = (PCA_PWM1 & 0x0c) | (2<<6)
#define		PWM1_OUT_NORMAL()	PCA_PWM1 &= ~3				//PWM1正常输出(默认)
#define		PWM1_OUT_0()	    PCA_PWM1 |=  3				//PWM1一直输出0
#define		PWM1_OUT_1()	    PCA_PWM1 &= ~3, CCAP1H = 0	//PWM1一直输出1

#define		PWM2_8Bit()	        PCA_PWM2 = (PCA_PWM2 & 0x0c) | (0<<6)
#define		PWM2_7Bit()	        PCA_PWM2 = (PCA_PWM2 & 0x0c) | (1<<6)
#define		PWM2_6Bit()	        PCA_PWM2 = (PCA_PWM2 & 0x0c) | (2<<6)
#define		PWM2_OUT_NORMAL()	PCA_PWM2 &= ~3				//PWM2正常输出(默认)
#define		PWM2_OUT_0()	    PCA_PWM2 |=  3				//PWM2一直输出0
#define		PWM2_OUT_1()	    PCA_PWM2 &= ~3, CCAP2H = 0	//PWM2一直输出1


/* **************************************************    SYS   ************************************************** */
//						  7     6      5    4     3      2    1     0     Reset Value
//sfr PCON   = 0x87;	SMOD  SMOD0  LVDF  POF   GF1    GF0   PD   IDL    0001,0000	 //Power Control 
#define		UART1_2_3_DoubleRate()		PCON |= 0x80        //串口1、2、3波特率加倍
#define     MCU_IsColdBoot()            ((PCON&0x10)!=0)    //MCU冷启动判断
#define		MCU_IntoIdle()			    PCON |= 1	        //MCU进入IDLE模式
#define		MCU_IntoSleep()	            PCON |= 2	        //MCU进入睡眠模式


//						   7        6        5        4        3        2        1        0     Reset Value
//sfr PCON2   = 0x97;	MCKO_S1  MCKO_S0   ADRJ     Tx_Rx    MCLKO_2   CLKS2   CLKS1    CLKS0   00010,0000	 
#define 	ADC_Result_H2L8()		PCON2 |=  (1<<5)    //ADC_RES[1:0]存放高2位结果，ADC_RESL[7:0]存放低8位结果
#define     ADC_Result_H8L2()		PCON2 &= ~(1<<5)	//ADC_RES[7:0]存放高8位结果，ADC_RESL[1:0]存放低2位结果
#define		UART1_TxD_RxD_Connect()	PCON2 |=  (1<<4)	//将TXD与RXD连接中继输出
#define		UART1_TxD_RxD_Disconn()	PCON2 &= ~(1<<4)	//将TXD与RXD连接中继断开	默认

//                                      7     6     5      4       3      2   1   0     Reset Value
//sfr WDT_CONTR = 0xC1;             WDT_FLAG  -  EN_WDT CLR_WDT IDLE_WDT PS2 PS1 PS0    xx00,0000
#define D_WDT_FLAG			(1<<7)
#define D_EN_WDT			(1<<5)
#define D_CLR_WDT			(1<<4)	//auto clear
#define D_IDLE_WDT			(1<<3)	//WDT counter when Idle
#define D_WDT_SCALE_2		0
#define D_WDT_SCALE_4		1
#define D_WDT_SCALE_8		2		//T=393216*N/fo
#define D_WDT_SCALE_16		3
#define D_WDT_SCALE_32		4
#define D_WDT_SCALE_64		5
#define D_WDT_SCALE_128		6
#define D_WDT_SCALE_256		7

#define	WDT_reset(scale)	WDT_CONTR = D_EN_WDT + D_CLR_WDT + D_IDLE_WDT + (scale)		//初始化WDT，喂狗

/* **************************************************    IAP   ************************************************** */
//sfr IAP_CMD   = 0xC5;
#define		IAP_Op_None()	    IAP_CMD = 0		//IAP空闲命令（禁止）
#define		IAP_Op_Read()		IAP_CMD = 1		//IAP读出命令
#define		IAP_Op_Write()		IAP_CMD = 2		//IAP写入命令
#define		IAP_Op_Erase()		IAP_CMD = 3		//IAP擦除命令

//sfr IAP_TRIG  = 0xC6;
#define 	IAP_Trig_Op()	        IAP_TRIG = 0x5A,IAP_TRIG = 0xA5,_nop_()		//IAP触发命令,需延时
#define 	IAP_Not_Trig()	    	IAP_TRIG = 0x00		                        //防止ISP/IAP命令误触发

//							  7    6    5      4    3    2    1     0    Reset Value
//sfr IAP_CONTR = 0xC7;		IAPEN SWBS SWRST CFAIL  -   WT2  WT1   WT0   0000,x000	//IAP Control Register
#define IAP_EN			(1<<7)
#define IAP_SWBS		(1<<6)
#define IAP_SWRST		(1<<5)
#define IAP_CMD_FAIL	(1<<4)
#define IAP_WAIT_1MHZ	7
#define IAP_WAIT_2MHZ	6
#define IAP_WAIT_3MHZ	5
#define IAP_WAIT_6MHZ	4
#define IAP_WAIT_12MHZ	3
#define IAP_WAIT_20MHZ	2
#define IAP_WAIT_24MHZ	1
#define IAP_WAIT_30MHZ	0

#if (MAIN_FOSC >= 24000000L)
	#define		IAP_WAIT_FREQUENCY	IAP_WAIT_30MHZ
#elif (MAIN_FOSC >= 20000000L)
	#define		IAP_WAIT_FREQUENCY	IAP_WAIT_24MHZ
#elif (MAIN_FOSC >= 12000000L)
	#define		IAP_WAIT_FREQUENCY	IAP_WAIT_20MHZ
#elif (MAIN_FOSC >= 6000000L)
	#define		IAP_WAIT_FREQUENCY	IAP_WAIT_12MHZ
#elif (MAIN_FOSC >= 3000000L)
	#define		IAP_WAIT_FREQUENCY	IAP_WAIT_6MHZ
#elif (MAIN_FOSC >= 2000000L)
	#define		IAP_WAIT_FREQUENCY	IAP_WAIT_3MHZ
#elif (MAIN_FOSC >= 1000000L)
	#define		IAP_WAIT_FREQUENCY	IAP_WAIT_2MHZ
#else
	#define		IAP_WAIT_FREQUENCY	IAP_WAIT_1MHZ
#endif

#define     IAP_Enable()                IAP_CONTR = (IAP_EN + IAP_WAIT_FREQUENCY)   //设置等待时间，允许ISP/IAP操作，送一次就够
#define     IAP_Disable()               IAP_CONTR = 0   
#define     MCU_SoftwareReset()         IAP_CONTR = IAP_SWRST   //MCU软件复位,从用户应用程序启动

//IAP_ADDRH,IAP_ADDRL,IAP_DATA
#define     IAP_WrAddr(addr)        IAP_ADDRH = (addr) / 256,IAP_ADDRL = (addr) % 256
#define     IAP_WrByte(dat)         IAP_DATA = (dat)
#define     IAP_RdByte()            IAP_DATA 

#endif
