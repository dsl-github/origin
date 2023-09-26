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
#define		Timer0_Load(n)		TH0 = (n) / 256,	TL0 = (n) % 256     //װ�ض�ʱ��0��ֵ
#define		Timer1_Load(n)		TH1 = (n) / 256,	TL1 = (n) % 256     //װ�ض�ʱ��1��ֵ
#define		Timer2_Load(n)		T2H = (n) / 256,	T2L = (n) % 256     //װ�ض�ʱ��2��ֵ

#define		Timer0_Read_TC()	((u16)TH0<<8)|TL0 //��ȡ��ʱ��0������
#define		Timer1_Read_TC()	((u16)TH1<<8)|TL1 //��ȡ��ʱ��1������
#define		Timer2_Read_TC()	((u16)T2H<<8)|T2L //��ȡ��ʱ��2������

//						  7      6      5      4      3      2     1     0        Reset Value
//sfr TCON = 0x88;		 TF1    TR1    TF0    TR0    IE1    IT1   IE0   IT0       0000,0000B  
#define		Timer1_Run()			            TR1 = 1             //����ʱ��1����
#define		Timer1_Stop()			            TR1 = 0             //��ֹ��ʱ��1����
#define		Timer0_Run()			            TR0 = 1             //����ʱ��0����
#define		Timer0_Stop()			            TR0 = 0             //��ֹ��ʱ��0����
#define     EXT1_Mode_Fall()                    IT1 = 1             //�ⲿ�ж�1�½��ش���
#define     EXT1_Mode_RiseFall()                IT1 = 0             //�ⲿ�ж�1�������½��ؾ�����
#define     EXT0_Mode_Fall()                    IT0 = 1             //�ⲿ�ж�0�½��ش���
#define     EXT0_Mode_RiseFall()                IT0 = 0             //�ⲿ�ж�0�������½��ؾ�����
/* IE1��IE0��TF1��TF0Ӳ�����ƣ���δ���� */

//						  7      6      5      4      3      2     1     0        Reset Value
//sfr TMOD = 0x89;		GATE    C/T     M1     M0   GATE    C/T    M1    M0       0000,0000B    
#define		Timer0_16bitAutoReload()	TMOD &= ~0x03					//16λ�Զ���װ
#define		Timer0_16bit()				TMOD  = (TMOD & ~0x03) | 0x01	//16λ
#define		Timer0_8bitAutoReload()		TMOD  = (TMOD & ~0x03) | 0x02	//8λ�Զ���װ
#define		Timer0_16bitAutoRL_NoMask()	TMOD |=  0x03		            //16λ�Զ���װ���������ж�
#define		Timer0_AsCounter()			TMOD |=  4		                //��ʱ��0����������
#define		Timer0_AsTimer()			TMOD &= ~4		                //��ʱ��0������ʱ��
#define		Timer0_ExtControl()			TMOD |=  (1<<3)		            //��ʱ��0���ⲿINT0�ߵ�ƽ����ʱ����
#define		Timer1_16bitAutoReload()	TMOD &= ~0x30					//16λ�Զ���װ
#define		Timer1_16bit()				TMOD  = (TMOD & ~0x30) | 0x10	//16λ
#define		Timer1_8bitAutoReload()		TMOD  = (TMOD & ~0x30) | 0x20	//8λ�Զ���װ
#define		Timer1_16bitAutoRL_NoMask()	TMOD |=  0x30		            //16λ�Զ���װ���������ж�
#define		Timer1_AsCounter()			TMOD |=  (1<<6)		            //��ʱ��1����������
#define		Timer1_AsTimer()			TMOD &= ~(1<<6)		            //��ʱ��1������ʱ��
#define		Timer1_ExtControl()			TMOD |=  (1<<7)		            //��ʱ��1���ⲿINT1�ߵ�ƽ����ʱ����


/* **************************************************    IO    ************************************************** */
#define	GPIO_Px0		0x01	//IO���� Px.0
#define	GPIO_Px1		0x02	//IO���� Px.1
#define	GPIO_Px2		0x04	//IO���� Px.2
#define	GPIO_Px3		0x08	//IO���� Px.3
#define	GPIO_Px4		0x10	//IO���� Px.4
#define	GPIO_Px5		0x20	//IO���� Px.5
#define	GPIO_Px6		0x40	//IO���� Px.6
#define	GPIO_Px7		0x80	//IO���� Px.7
#define	GPIO_All	    0xFF	//IO��������

/* PxM1.n,PxM0.n 	=00--->Standard,	01--->push-pull						
                    =10--->pure input,	11--->open drain          */
#define     P0_IO_PullUp_Set(pin)    P0M1 &= ~(pin),P0M0 &= ~(pin)//����׼˫���
#define     P0_IN_HighZ_Set(pin)     P0M1 |=  (pin),P0M0 &= ~(pin)//��������
#define     P0_OUT_OD_Set(pin)       P0M1 |=  (pin),P0M0 |=  (pin)//��©���
#define     P0_OUT_PP_Set(pin)       P0M1 &= ~(pin),P0M0 |=  (pin)//�������

#define     P1_IO_PullUp_Set(pin)    P1M1 &= ~(pin),P1M0 &= ~(pin)//����׼˫���
#define     P1_IN_HighZ_Set(pin)     P1M1 |=  (pin),P1M0 &= ~(pin)//��������
#define     P1_OUT_OD_Set(pin)       P1M1 |=  (pin),P1M0 |=  (pin)//��©���
#define     P1_OUT_PP_Set(pin)       P1M1 &= ~(pin),P1M0 |=  (pin)//�������

#define     P2_IO_PullUp_Set(pin)    P2M1 &= ~(pin),P2M0 &= ~(pin)//����׼˫���
#define     P2_IN_HighZ_Set(pin)     P2M1 |=  (pin),P2M0 &= ~(pin)//��������
#define     P2_OUT_OD_Set(pin)       P2M1 |=  (pin),P2M0 |=  (pin)//��©���
#define     P2_OUT_PP_Set(pin)       P2M1 &= ~(pin),P2M0 |=  (pin)//�������

#define     P3_IO_PullUp_Set(pin)    P3M1 &= ~(pin),P3M0 &= ~(pin)//����׼˫���
#define     P3_IN_HighZ_Set(pin)     P3M1 |=  (pin),P3M0 &= ~(pin)//��������
#define     P3_OUT_OD_Set(pin)       P3M1 |=  (pin),P3M0 |=  (pin)//��©���
#define     P3_OUT_PP_Set(pin)       P3M1 &= ~(pin),P3M0 |=  (pin)//�������

#define     P4_IO_PullUp_Set(pin)    P4M1 &= ~(pin),P4M0 &= ~(pin)//����׼˫���
#define     P4_IN_HighZ_Set(pin)     P4M1 |=  (pin),P4M0 &= ~(pin)//��������
#define     P4_OUT_OD_Set(pin)       P4M1 |=  (pin),P4M0 |=  (pin)//��©���
#define     P4_OUT_PP_Set(pin)       P4M1 &= ~(pin),P4M0 |=  (pin)//�������

#define     P5_IO_PullUp_Set(pin)    P5M1 &= ~(pin),P5M0 &= ~(pin)//����׼˫���
#define     P5_IN_HighZ_Set(pin)     P5M1 |=  (pin),P5M0 &= ~(pin)//��������
#define     P5_OUT_OD_Set(pin)       P5M1 |=  (pin),P5M0 |=  (pin)//��©���
#define     P5_OUT_PP_Set(pin)       P5M1 &= ~(pin),P5M0 |=  (pin)//�������

/* **************************************************   ADC    ************************************************** */
#define     ADC_Use_P1x(pin)		P1ASF = pin		//����P1.xΪADC����
//								7       6      5       4         3      2    1    0   Reset Value
//sfr ADC_CONTR = 0xBC;		ADC_POWER SPEED1 SPEED0 ADC_FLAG ADC_START CHS2 CHS1 CHS0 0000,0000	//AD ת�����ƼĴ��� 
#define     ADC_PWR_On()			ADC_CONTR |=  (1<<7)		//ADC��Դ����
#define     ADC_PWR_Off()			ADC_CONTR &= ~(1<<7)		//ADC��Դ�ر�
#define     ADC_Speed_90T()			ADC_CONTR |=  (3<<5)						//ADCת������Ϊ90��ʱ������
#define     ADC_Speed_180T()		ADC_CONTR = (ADC_CONTR & ~(3<<5)) | (2<<5)	//ADCת������Ϊ180��ʱ������
#define     ADC_Speed_360T()		ADC_CONTR = (ADC_CONTR & ~(3<<5)) | (1<<5)	//ADCת������Ϊ360��ʱ������
#define     ADC_Speed_540T()		ADC_CONTR &= ~(0<<5)						//ADCת������Ϊ540��ʱ������
#define		Get_ADC_FIF()			(ADC_CONTR&0x10)            //��ȡADCת������жϱ�־
#define		Clr_PCA_FIF()			ADC_CONTR &= ~0x10          //�������ADCת������жϱ�־
#define     ADC_Run()				ADC_CONTR |=  (1<<3)		//ADC����ת��
#define     ADC_Stop()				ADC_CONTR &= ~(1<<3)		//ADCֹͣת��
#define     ADC_Channel(chl)        ADC_CONTR = (ADC_CONTR & 0x07) | (chl)		//ADCѡ��ת��ͨ��

/* ADC���ת�� */
#define     Get_ADC_Result_H2L8()        (((u16)(ADC_RES & 3)<<8)|ADC_RESL)
#define     Get_ADC_Result_H8L2()        (((u16)ADC_RES<<2)|(ADC_RESL&3))

/* **************************************************   UART   ************************************************** */
//						  7      6      5      4      3      2     1     0        Reset Value
//sfr SCON = 0x98;		 SM0    SM1    SM2    REN    TB8    RB8    TI    RI       0000,0000B    //S1 Control
#define		UART1_8Bit()			    SCON  = (SCON & 0x3f) | 0x40
#define		UART1_9Bit()			    SCON  = (SCON & 0x3f) | 0xc0
#define		UART1_Rx_Enable()		    SCON |=  (1<<4)         //����1����
#define		UART1_Rx_Disable()		    SCON &= ~(1<<4)		    //��ֹ��1����
#define		Get_UART1_TxIF()			(SCON & 2)              //��ȡ��1�����жϱ�־λ
#define		Set_UART1_TxIF()			SCON |= 2               //������1�����ж�
#define		Clr_UART1_TxIF()			SCON &= ~2              //������㴮1�����жϱ�־λ
#define		Get_UART1_RxIF()			(SCON & 1)              //��ȡ��1�����жϱ�־λ
#define		Clr_UART1_RxIF()			SCON &= ~1              //������㴮1�����жϱ�־λ
/* SM2��TB8��RB8����ʹ�ã���δ���� */

#define     UART1_WrByte(dat)   SBUF = (dat)
#define     UART1_RdByte()      SBUF

//						  7      6      5      4      3      2     1     0        Reset Value
//sfr S2CON = 0x9A;		S2SM0    -    S2SM2  S2REN  S2TB8  S2RB8  S2TI  S2RI      0000,0000B    //S2 Control
#define		UART2_8Bit()			    S2CON &= ~(1<<7)		//����2ģʽ0��8λUART�������� = ��ʱ��2������� / 4
#define		UART2_9Bit()			    S2CON |=  (1<<7)		//����2ģʽ1��9λUART�������� = ��ʱ��2������� / 4
#define		UART2_Rx_Enable()		    S2CON |=  (1<<4)		//����2����
#define		UART2_Rx_Disable()		    S2CON &= ~(1<<4)		//��ֹ��2����
#define		Get_UART2_TxIF()			(S2CON & 2)             //��ȡ��2�����жϱ�־λ
#define		Set_UART2_TxIF()			S2CON |= 2              //������2�����ж�
#define		Clr_UART2_TxIF()			S2CON &= ~2             //������㴮2�����жϱ�־λ
#define		Get_UART2_RxIF()			(S2CON & 1)             //��ȡ��2�����жϱ�־λ
#define		Clr_UART2_RxIF()			S2CON &= ~1             //������㴮2�����жϱ�־λ
/* S2SM2��S2TB8��S2RB8����ʹ�ã���δ���� */

#define     UART2_WrByte(dat)   S2BUF = (dat)
#define     UART2_RdByte()      S2BUF

/* **************************************************   AUXR   ************************************************** */
//						   7     6       5      4     3      2      1      0    Reset Value
//sfr AUXR  = 0x8E;		T0x12 T1x12 UART_M0x6  T2R  T2_C/T T2x12 EXTRAM  S1ST2  0000,0000	//Auxiliary Register 
#define 	Timer0_CLK_1T()					AUXR |=  (1<<7)	//Timer0 clodk = fo
#define 	Timer0_CLK_12T()				AUXR &= ~(1<<7)	//Timer0 clodk = fo/12	12��Ƶ,	default
#define 	Timer1_CLK_1T()					AUXR |=  (1<<6)	//Timer1 clodk = fo
#define 	Timer1_CLK_12T()				AUXR &= ~(1<<6)	//Timer1 clodk = fo/12	12��Ƶ,	default
#define 	Timer2_Run()	 			    AUXR |=  (1<<4)	//����ʱ��2����
#define 	Timer2_Stop()	 			    AUXR &= ~(1<<4)	//��ֹ��ʱ��2����
#define		Timer2_AsCounter()		        AUXR |=  (1<<3)	//ʱ��2����������,��T2/P3.1�ⲿ�������
#define		Timer2_AsTimer()			    AUXR &= ~(1<<3)	//ʱ��2������ʱ��
#define 	Timer2_CLK_1T()					AUXR |=  (1<<2)	//Timer0 clodk = fo
#define 	Timer2_CLK_12T()				AUXR &= ~(1<<2)	//Timer0 clodk = fo/12	12��Ƶ,	default
#define 	ExternalRAM_Enable()		    AUXR |=  2		//�����ⲿXRAM����ֹʹ���ڲ�1024RAM
#define 	InternalRAM_Enable()		    AUXR &= ~2		//��ֹ�ⲿXRAM������ʹ���ڲ�1024RAM
#define 	UART1_BRT_UseTimer2()	        AUXR |=  1      //��ʱ��2��Ϊ��1�����ʷ�����
#define 	UART1_BRT_UseTimer1()	        AUXR &= ~1      //��ʱ��1��Ϊ��1�����ʷ�����
/* UART_M0x6����ʹ�ã���δ���� */


//					            7      6       5        4       3       2       1      0    Reset Value
//sfr AUXR1/P_SW1 = 0xA2;	  S1_S1  S1_S0  CCP_S1   CCP_S0  SPI_S1   SPI_S0    -     DPS   0100,0000	//Auxiliary Register 1
#define		UART1_Use_P30P31()	    P_SW1 &= ~0xc0					//UART1 ʹ��P30 P31��	Ĭ��
#define		UART1_Use_P36P37()		P_SW1 = (P_SW1 & ~0xc0) | 0x40	//UART1 ʹ��P36 P37��
#define		UART1_Use_P16P17()	    P_SW1 = (P_SW1 & ~0xc0) | 0x80	//UART1 ʹ��P16 P17��
#define		PCA_Use_P12P11P10P37()	AUXR1 &= ~0x30		            //��PCA/PWM�л���P12(ECI) P11(CCP0) P10(CCP1) P37(CCP2)(�ϵ�Ĭ��)
#define		PCA_Use_P34P35P36P37()	AUXR1 = (AUXR1 & ~0x30) | 0x10	//��PCA/PWM�л���P34(ECI) P35(CCP0) P36(CCP1) P37(CCP2)
#define		PCA_Use_P24P25P26P27()	AUXR1 = (AUXR1 & ~0x30) | 0x20	//��PCA/PWM�л���P24(ECI) P25(CCP0) P26(CCP1) P27(CCP2)
#define		SPI_USE_P12P13P14P15()	AUXR1 &= ~0x0c	                //��SPI�л���P12(SS) P13(MOSI) P14(MISO) P15(SCLK)(�ϵ�Ĭ��)
#define		SPI_USE_P24P23P22P21()	AUXR1 = (AUXR1 & ~0x0c) | 0x04	//��SPI�л���P24(SS) P23(MOSI) P22(MISO) P21(SCLK)
#define		SPI_USE_P54P40P41P43()	AUXR1 = (AUXR1 & ~0x0c) | 0x08	//��SPI�л���P54(SS) P40(MOSI) P41(MISO) P43(SCLK)
/* DPS��δʹ�ã���δ���� */


//                                  7   6    5    4   3     2        1       0         Reset Value
//sfr INT_CLKO/AUXR2 = 0x8F;	    -  EX4  EX3  EX2  -   T2CLKO   T1CLKO  T0CLKO      0000,0000B
#define		EXT4_IRQ_Enable()		        INT_CLKO |=  (1<<6)	//�����ⲿ�ж�4�ж�
#define		EXT4_IRQ_Disable()	            INT_CLKO &= ~(1<<6)	//��ֹ�ⲿ�ж�4�ж�
#define		EXT3_IRQ_Enable()		        INT_CLKO |=  (1<<5)	//�����ⲿ�ж�3�ж�
#define		EXT3_IRQ_Disable()	            INT_CLKO &= ~(1<<5)	//��ֹ�ⲿ�ж�3�ж�
#define		EXT2_IRQ_Enable()		        INT_CLKO |=  (1<<4)	//�����ⲿ�ж�2�ж�
#define		EXT2_IRQ_Disable()	            INT_CLKO &= ~(1<<4)	//��ֹ�ⲿ�ж�2�ж�
#define		Timer2_CLKOUT_P30_Enable()		INT_CLKO |=  (1<<2)   //����P3.0���óɶ�ʱ��2ʱ�������Fck2 = 1/2 T2 �����
#define		Timer2_CLKOUT_P30_Disable()		INT_CLKO &= ~(1<<2)   //��ֹP3.0���óɶ�ʱ��2ʱ�����
#define		Timer1_CLKOUT_P34_Enable()		INT_CLKO |=  (1<<1)   //����P3.4���óɶ�ʱ��1ʱ�������Fck1 = 1/2 T1 �����
#define		Timer1_CLKOUT_P34_Disable()		INT_CLKO &= ~(1<<1)   //��ֹP3.4���óɶ�ʱ��1ʱ�����
#define		Timer0_CLKOUT_P35_Enable()		INT_CLKO |=  (1<<0)   //����P3.5���óɶ�ʱ��0ʱ�������Fck0 = 1/2 T0 �����
#define		Timer0_CLKOUT_P35_Disable()		INT_CLKO &= ~(1<<0)   //��ֹP3.5���óɶ�ʱ��0ʱ�����


//						   7     6       5      4     3      2      1      0    Reset Value
//sfr P_SW2  = 0xBA;	EAXSFR DBLPWR  P31PU   P30PU  -    S4_S    S3_S   S2_S  0000,0000	//�˿����üĴ���
#define		UART2_Use_P10P11()		P_SW2 &= ~1		//UART2 ʹ��P1.0/RxD2,P1.1/TxD2��	Ĭ��
#define		UART2_Use_P46P47()		P_SW2 |=  1		//UART2 ʹ��P4.6/RxD2_2,P4.7/TxD2_2��
#define		UART3_Use_P00P01()		P_SW2 &= ~2		//UART3 ʹ��P0.0/RxD3,P0.1/TxD3��	Ĭ��
#define		UART3_Use_P50P51()		P_SW2 |=  2		//UART3 ʹ��P5.0/RxD3_2,P5.1/TxD3_2��
#define		UART4_Use_P02P03()		P_SW2 &= ~4		//UART4 ʹ��P0.2/RxD4,P0.3/TxD4��	Ĭ��
#define		UART4_Use_P52P53()		P_SW2 |=  4		//UART4 ʹ��P5.2/RxD4_2,P5.3/TxD4_2��
/* DBLPWR��P31PU��P30Pδ֪��EAXSFR��δʹ�ã���δ���� */


/* **************************************************   IE/IP  ************************************************** */
//						  7     6     5    4    3    2    1     0    Reset Value
//sfr IE  = 0xA8;		 EA   ELVD  EADC   ES  ET1  EX1  ET0   EX0   0000,0000B	//�ж�����Ĵ���(��λѰַ)  
#define		Set_IRQ_EA(n)		    EA   = (n)	//�ж������ܿ���λ
#define		LVD_IRQ_Enable()		ELVD =  1	//�����ѹ����ж�
#define		LVD_IRQ_Disable()		ELVD =  0	//��ֹ��ѹ����ж�
#define		ADC_IRQ_Enable()		EADC =  1	//����ADC�ж�
#define		ADC_IRQ_Disable()		EADC =  0	//��ֹADC�ж�
#define		UART1_IRQ_Enable()		ES   =  1	//������1�ж�
#define		UART1_IRQ_Disable()		ES   =  0	//��ֹ����1�ж�
#define		Timer1_IRQ_Enable()		ET1  =  1	//����ʱ��1�ж�
#define		Timer1_IRQ_Disable()	ET1  =  0	//��ֹ��ʱ��1�ж�
#define		EXT1_IRQ_Enable()		EX1  =  1	//�����ⲿ�ж�1�ж�
#define		EXT1_IRQ_Disable()	    EX1  =  0	//��ֹ�ⲿ�ж�1�ж�
#define		Timer0_IRQ_Enable()		ET0  =  1	//����ʱ��0�ж�
#define		Timer0_IRQ_Disable()	ET0  =  0	//��ֹ��ʱ��0�ж�
#define		EXT0_IRQ_Enable()		EX0  =  1	//�����ⲿ�ж�1�ж�
#define		EXT0_IRQ_Disable()	    EX0  =  0	//��ֹ�ⲿ�ж�1�ж�


//						  7     6     5    4    3    2    1     0    Reset Value
//sfr IE2  = 0xAF;		  -    ET4   ET3  ES4  ES3  ET2  ESPI  ES2   0000,0000B	//�ж�����Ĵ���(����λѰַ)  
#define		Timer4_IRQ_Enable()		IE2 |=  (1<<6)	//����ʱ��4�ж�
#define		Timer4_IRQ_Disable()	IE2 &= ~(1<<6)	//��ֹ��ʱ��4�ж�
#define		Timer3_IRQ_Enable()		IE2 |=  (1<<5)	//����ʱ��3�ж�
#define		Timer3_IRQ_Disable()	IE2 &= ~(1<<5)	//��ֹ��ʱ��3�ж�
#define		UART4_IRQ_Enable()		IE2 |=  (1<<4)	//������4�ж�
#define		UART4_IRQ_Disable()		IE2 &= ~(1<<4)	//��ֹ����4�ж�
#define		UART3_IRQ_Enable()		IE2 |=  (1<<3)	//������3�ж�
#define		UART3_IRQ_Disable()		IE2 &= ~(1<<3)	//��ֹ����3�ж�
#define		Timer2_IRQ_Enable()		IE2 |=  (1<<2)	//����ʱ��2�ж�
#define		Timer2_IRQ_Disable()	IE2 &= ~(1<<2)	//��ֹ��ʱ��2�ж�
#define		SPI_IRQ_Enable()		IE2 |=  2	//����SPI�ж�
#define		SPI_IRQ_Disable()		IE2 &= ~2	//��ֹSPI�ж�
#define		UART2_IRQ_Enable()		IE2 |=  1	//������2�ж�
#define		UART2_IRQ_Disable()		IE2 &= ~1	//��ֹ����2�ж�


//                                          7     6     5    4    3    2    1    0    Reset Value
//sfr IP      = 0xB8; //�ж����ȼ���λ      PPCA  PLVD  PADC  PS   PT1  PX1  PT0  PX0   0000,0000
#define     PCA_SetHighPolity()         PPCA = 1     //�����ȼ��ж�
#define     PCA_SetLowPolity()          PPCA = 0     //�����ȼ��ж�
#define     LVD_SetHighPolity()         PLVD = 1     //�����ȼ��ж�
#define     LVD_SetLowPolity()          PLVD = 0     //�����ȼ��ж�
#define     ADC_SetHighPolity()         PADC = 1     //�����ȼ��ж�
#define     ADC_SetLowPolity()          PADC = 0     //�����ȼ��ж�
#define     UART1_SetHighPolity()       PS = 1     //�����ȼ��ж�
#define     UART1_SetLowPolity()        PS = 0     //�����ȼ��ж�
#define     Timer1_SetHighPolity()      PT1 = 1     //�����ȼ��ж�
#define     Timer1_SetLowPolity()       PT1 = 0     //�����ȼ��ж�
#define     Ext1_SetHighPolity()        PX1 = 1     //�����ȼ��ж�
#define     Ext1_SetLowPolity()         PX1 = 0     //�����ȼ��ж�
#define     Timer0_SetHighPolity()      PT0 = 1     //�����ȼ��ж�
#define     Timer0_SetLowPolity()       PT0 = 0     //�����ȼ��ж�
#define     Ext0_SetHighPolity()        PX0 = 1     //�����ȼ��ж�
#define     Ext0_SetLowPolity()         PX0 = 0     //�����ȼ��ж�


//                                           7       6       5      4      3      2     1      0     Reset Value
//sfr IP2   = 0xB5; //                       -       -       -     PX4  PPWMFD   PPWM  PSPI   PS2    xxxx,xx00
#define     SPI_SetHighPolity()         IP2 |=  2     //�����ȼ��ж�
#define     SPI_SetLowPolity()          IP2 &= ~2     //�����ȼ��ж�
#define     UART2_SetHighPolity()       IP2 |=  1     //�����ȼ��ж�
#define     UART2_SetLowPolity()        IP2 &= ~1     //�����ȼ��ж�

/* **************************************************    PCA   ************************************************** */
//						7     6     5     4     3     2     1     0     Reset Value
//sfr CCON   = 0xD8;	CF    CR    -     -     -    CCF2  CCF1  CCF0   00xx,xx00	//PCA ���ƼĴ���
#define		Get_PCA0_CCIF()			CCF0            //��ȡPCA0ƥ��򲶻��жϱ�־λ
#define		Clr_PCA0_CCIF()			CCF0 = 0        //�������PCA0ƥ��򲶻��жϱ�־λ
#define		Get_PCA1_CCIF()			CCF1            //��ȡPCA0ƥ��򲶻��жϱ�־λ
#define		Clr_PCA1_CCIF()			CCF1 = 0        //�������PCA0ƥ��򲶻��жϱ�־λ
#define		Get_PCA2_CCIF()			CCF2            //��ȡPCA0ƥ��򲶻��жϱ�־λ
#define		Clr_PCA2_CCIF()			CCF2 = 0        //�������PCA0ƥ��򲶻��жϱ�־λ
#define		PCA_Timer_Stop()		CR = 0          //PCA��ʱ��/�������ر�
#define		PCA_Timer_Run()			CR = 1          //PCA��ʱ��/����������
#define		Get_PCA_Timer_CIF()		CF              //��ȡPCA��ʱ��/����������жϱ�־
#define		Clr_PCA_Timer_CIF()		CF = 0          //�������PCA��ʱ��/����������жϱ�־

//					 7     6     5     4     3     2     1     0    Reset Value
//sfr CMOD  = 0xD9;	CIDL   -     -     -   CPS2   CPS1  CPS0  ECF   0xxx,0000	//PCA ����ģʽ�Ĵ�����
#define PCA_Timer_IDLE_Off()	    CMOD |=  (1<<7)				//IDLE״̬PCAֹͣ������
#define PCA_Timer_IDLE_On()		    CMOD &= ~(1<<7)				//IDLE״̬PCA����������
#define PCA_Timer_CLK_12T()		    CMOD &= ~0x0E				//PCA��������ѡ���ⲿ����/12��	fosc/12
#define PCA_Timer_CLK_2T()		    CMOD = (CMOD & ~0x0E) + 2	//PCA��������ѡ���ⲿ����/2��	fosc/2
#define PCA_Timer_CLK_T0()		    CMOD = (CMOD & ~0x0E) + 4	//PCA��������ѡ��Timer0�жϣ�Timer0��ͨ��AUXR�Ĵ������óɹ�����12T��1Tģʽ��
#define PCA_Timer_CLK_ECI()		    CMOD = (CMOD & ~0x0E) + 6	//PCA��������ѡ���ECI/P3.4��������ⲿʱ�ӣ���� fosc/2��
#define PCA_Timer_CLK_1T()		    CMOD = (CMOD & ~0x0E) + 8	//PCA��������ѡ���ⲿ����		Fosc/1
#define PCA_Timer_CLK_4T()		    CMOD = (CMOD & ~0x0E) + 10	//PCA��������ѡ���ⲿ����/4��	Fosc/4
#define PCA_Timer_CLK_6T()		    CMOD = (CMOD & ~0x0E) + 12	//PCA��������ѡ���ⲿ����/6��	Fosc/6
#define PCA_Timer_CLK_8T()		    CMOD = (CMOD & ~0x0E) + 14	//PCA��������ѡ���ⲿ����/8��	Fosc/8
#define PCA_Timer_IRQ_Enable()	    CMOD |=  1					//PCA����������ж�����λ��1---����CF��CCON.7�������жϡ�
#define PCA_Timer_IRQ_Disable()	    CMOD &= ~1					//PCA����������жϽ�ֹ��


//									7     6      5      4     3     2     1     0     Reset Value
//sfr CCAPM0 = 0xDA;	PWM �Ĵ���  -   ECOM0  CAPP0  CAPN0  MAT0  TOG0  PWM0  ECCF0   x000,0000	//PCA ģ��0 
//sfr CCAPM1 = 0xDB;	PWM �Ĵ���  -   ECOM1  CAPP1  CAPN1  MAT1  TOG1  PWM1  ECCF1   x000,0000	//PCA ģ��1
//sfr CCAPM2 = 0xDC;	PWM �Ĵ���  -   ECOM2  CAPP2  CAPN2  MAT2  TOG2  PWM2  ECCF2   x000,0000	//PCA ģ��2
// ECOMn = 1:	����ȽϹ��ܡ�
// CAPPn = 1:	���������ش�����׽���ܡ�
// CAPNn = 1:	�����½��ش�����׽���ܡ�
// MATn  = 1:	��ƥ���������ʱ������CCON�е�CCFn��λ��
// TOGn  = 1:	��ƥ���������ʱ��CEXn����ת��(CEX0/PCA0/PWM0/P3.7,CEX1/PCA1/PWM1/P3.5)
// PWMn  = 1:	��CEXn����ΪPWM�����
// ECCFn = 1:	����CCON�е�CCFn�����жϡ�
// ECOMn CAPPn CAPNn MATn TOGn PWMn ECCFn
//   0     0     0    0    0    0     0		00H	δ�����κι��ܡ�
//   x     1     0    0    0    0     x	 	20H	16λCEXn�����ش�����׽���ܡ�
//   x     0     1    0    0    0     x	 	10H	16λCEXn�½��ش�����׽���ܡ�
//   x     1     1    0    0    0     x	 	30H	16λCEXn/PCAn���أ��ϡ����أ�������׽���ܡ�
//   1     0     0    1    0    0     x	 	48H	16λ�����ʱ����
//   1     0     0    1    1    0     x	 	4CH	16λ�������������
//   1     0     0    0    0    1     0		42H	8λPWM�����ж�
//   1     1     0    0    0    1     1		63H	8λPWM���ͱ�߿ɲ����ж�
//   1     0     1    0    0    1     1		53H	8λPWM���߱�Ϳɲ����ж�
//   1     1     1    0    0    1     1		73H	8λPWM���ͱ�߻�߱�;��ɲ����ж�
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

/* ��ʱ����ֵ */
#define     Set_PCA_TimerVal(val)	CL = (u8)val,CH = (u8)(val >> 8)
/* ����ռ�ձ� */
#define     Set_PCA0_PWM_DutyRatio(pwmBit,ratio)   CCAP0H = (1UL<<pwmBit)*(ratio)/100
#define     Set_PCA1_PWM_DutyRatio(pwmBit,ratio)   CCAP1H = (1UL<<pwmBit)*(ratio)/100
#define     Set_PCA2_PWM_DutyRatio(pwmBit,ratio)   CCAP2H = (1UL<<pwmBit)*(ratio)/100

/* ���ñȽ�ֵ */
#define     Set_PCA0_CaptureVal(val)   CCAP0L = (u8)val,CCAP0H = (u8)(val >> 8)//��дCCAP0L,��дCCAP0H
#define     Set_PCA1_CaptureVal(val)   CCAP1L = (u8)val,CCAP1H = (u8)(val >> 8)//��дCCAP1L,��дCCAP1H
#define     Set_PCA2_CaptureVal(val)   CCAP2L = (u8)val,CCAP2H = (u8)(val >> 8)//��дCCAP2L,��дCCAP2H

/* ��ȡ�Ƚ�ֵ */
#define     Get_PCA0_CaptureVal() ((u16)CCAP0H<<8)|CCAP0L
#define     Get_PCA1_CaptureVal() ((u16)CCAP1H<<8)|CCAP1L
#define     Get_PCA2_CaptureVal() ((u16)CCAP2H<<8)|CCAP2L

//                7       6     5   4   3   2     1       0
// PCA_PWMn:	EBSn_1	EBSn_0	-	-	-	-	EPCnH	EPCnL
// B1(EPCnH):	��PWMģʽ�£���CCAPnH���9λ����
// B0(EPCnL):	��PWMģʽ�£���CCAPnL���9λ����
// PWMģʽ:1.(0,CL[n:0])>=(EPCnL,CCPnL[n:0])���1;
//         2.(0,CL[n:0])<(EPCnL,CCPnL[n:0])���0;
//         3.CL���ʱ(EPCnH,CCPnH[n:0])������(EPCnL,CCPnL[n:0])

#define		PWM0_8Bit()	        PCA_PWM0 = (PCA_PWM0 & 0x0c) | (0<<6)
#define		PWM0_7Bit()	        PCA_PWM0 = (PCA_PWM0 & 0x0c) | (1<<6)
#define		PWM0_6Bit()	        PCA_PWM0 = (PCA_PWM0 & 0x0c) | (2<<6)
#define		PWM0_OUT_NORMAL()	PCA_PWM0 &= ~3				//PWM0�������(Ĭ��)
#define		PWM0_OUT_0()	    PCA_PWM0 |=  3				//PWM0һֱ���0
#define		PWM0_OUT_1()	    PCA_PWM0 &= ~3, CCAP0H = 0	//PWM0һֱ���1

#define		PWM1_8Bit()	        PCA_PWM1 = (PCA_PWM1 & 0x0c) | (0<<6)
#define		PWM1_7Bit()	        PCA_PWM1 = (PCA_PWM1 & 0x0c) | (1<<6)
#define		PWM1_6Bit()	        PCA_PWM1 = (PCA_PWM1 & 0x0c) | (2<<6)
#define		PWM1_OUT_NORMAL()	PCA_PWM1 &= ~3				//PWM1�������(Ĭ��)
#define		PWM1_OUT_0()	    PCA_PWM1 |=  3				//PWM1һֱ���0
#define		PWM1_OUT_1()	    PCA_PWM1 &= ~3, CCAP1H = 0	//PWM1һֱ���1

#define		PWM2_8Bit()	        PCA_PWM2 = (PCA_PWM2 & 0x0c) | (0<<6)
#define		PWM2_7Bit()	        PCA_PWM2 = (PCA_PWM2 & 0x0c) | (1<<6)
#define		PWM2_6Bit()	        PCA_PWM2 = (PCA_PWM2 & 0x0c) | (2<<6)
#define		PWM2_OUT_NORMAL()	PCA_PWM2 &= ~3				//PWM2�������(Ĭ��)
#define		PWM2_OUT_0()	    PCA_PWM2 |=  3				//PWM2һֱ���0
#define		PWM2_OUT_1()	    PCA_PWM2 &= ~3, CCAP2H = 0	//PWM2һֱ���1


/* **************************************************    SYS   ************************************************** */
//						  7     6      5    4     3      2    1     0     Reset Value
//sfr PCON   = 0x87;	SMOD  SMOD0  LVDF  POF   GF1    GF0   PD   IDL    0001,0000	 //Power Control 
#define		UART1_2_3_DoubleRate()		PCON |= 0x80        //����1��2��3�����ʼӱ�
#define     MCU_IsColdBoot()            ((PCON&0x10)!=0)    //MCU�������ж�
#define		MCU_IntoIdle()			    PCON |= 1	        //MCU����IDLEģʽ
#define		MCU_IntoSleep()	            PCON |= 2	        //MCU����˯��ģʽ


//						   7        6        5        4        3        2        1        0     Reset Value
//sfr PCON2   = 0x97;	MCKO_S1  MCKO_S0   ADRJ     Tx_Rx    MCLKO_2   CLKS2   CLKS1    CLKS0   00010,0000	 
#define 	ADC_Result_H2L8()		PCON2 |=  (1<<5)    //ADC_RES[1:0]��Ÿ�2λ�����ADC_RESL[7:0]��ŵ�8λ���
#define     ADC_Result_H8L2()		PCON2 &= ~(1<<5)	//ADC_RES[7:0]��Ÿ�8λ�����ADC_RESL[1:0]��ŵ�2λ���
#define		UART1_TxD_RxD_Connect()	PCON2 |=  (1<<4)	//��TXD��RXD�����м����
#define		UART1_TxD_RxD_Disconn()	PCON2 &= ~(1<<4)	//��TXD��RXD�����м̶Ͽ�	Ĭ��

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

#define	WDT_reset(scale)	WDT_CONTR = D_EN_WDT + D_CLR_WDT + D_IDLE_WDT + (scale)		//��ʼ��WDT��ι��

/* **************************************************    IAP   ************************************************** */
//sfr IAP_CMD   = 0xC5;
#define		IAP_Op_None()	    IAP_CMD = 0		//IAP���������ֹ��
#define		IAP_Op_Read()		IAP_CMD = 1		//IAP��������
#define		IAP_Op_Write()		IAP_CMD = 2		//IAPд������
#define		IAP_Op_Erase()		IAP_CMD = 3		//IAP��������

//sfr IAP_TRIG  = 0xC6;
#define 	IAP_Trig_Op()	        IAP_TRIG = 0x5A,IAP_TRIG = 0xA5,_nop_()		//IAP��������,����ʱ
#define 	IAP_Not_Trig()	    	IAP_TRIG = 0x00		                        //��ֹISP/IAP�����󴥷�

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

#define     IAP_Enable()                IAP_CONTR = (IAP_EN + IAP_WAIT_FREQUENCY)   //���õȴ�ʱ�䣬����ISP/IAP��������һ�ξ͹�
#define     IAP_Disable()               IAP_CONTR = 0   
#define     MCU_SoftwareReset()         IAP_CONTR = IAP_SWRST   //MCU�����λ,���û�Ӧ�ó�������

//IAP_ADDRH,IAP_ADDRL,IAP_DATA
#define     IAP_WrAddr(addr)        IAP_ADDRH = (addr) / 256,IAP_ADDRL = (addr) % 256
#define     IAP_WrByte(dat)         IAP_DATA = (dat)
#define     IAP_RdByte()            IAP_DATA 

#endif
