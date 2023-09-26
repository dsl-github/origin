
特殊管脚
基本型：
PCA : P1.2/ECI,P1.1/CCP0,P1.0/CCP1,P3.7/CCP2
      P3.4/ECI,P3.5/CCP0,P3.6/CCP1,P3.7/CCP2
	P2.4/ECI,P2.5/CCP0,P2.6/CCP1,P2.7/CCP2

INT :P3.2/INT0    P3.3/INT1    P3.6/INT2    P3.7/INT3    P3.0/INT4    

TIM :P3.4/T0      P3.5/T1      P3.1/T2

S1  :P3.0/RXD,P3.1/TXD         P3.6/RXD,P3.7/TXD         P1.6/RXD,P1.7/TXD    BRT只能使用T1或T2
S2  :P1.0/RXD,P1.1/TXD         P4.6/RXD,P4.7/TXD                              BRT只能使用T2

ADC :P1.0-P1.7

增强型：
PCA只有PCA0和PCA1

TIM增加     P0.5/T3  P0.7/T4  

uart增加    S3:P0.0/RXD,P0.1/TXD       P5.0/RXD,P1.1/TXD
            S4:P0.2/RXD,P0.3/TXD       P5.2/RXD,P5.3/TXD






最优利用资源

TIM :P3.4/T0      P3.5/T1(流量计)    

S1  :P3.0/RXD,P3.1/TXD
S2  :P4.6/RXD,P4.7/TXD   

INT :P3.2/INT0    P3.3/INT1    P3.6/INT2    P3.7/INT3

PCA : P2.4/ECI,P2.5/CCP0(捕获),P2.6/CCP1(PWM),P2.7/CCP2(PWM)

ADC :P1.0-P1.7


光感检测最好放在INT，预留电阻可以连接到T0或T1

