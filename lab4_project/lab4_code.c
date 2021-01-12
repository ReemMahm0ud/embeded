#include <reg52.h>

//frequency XTAL = 22.1184 MHZ
sbit outputLed = P2^0;
sbit Led = P2^1;
sbit  HZ38K = P2^2;
sbit detectObstacle = P3^3;
sbit output = P3^7;
unsigned char flag = 0;

void finalOutput(void)
{

    output = HZ38K & Led ;

}
void Timer0_ISR (void) interrupt 1
{

    if(flag == 0)
    {
        //generate 600us on
        Led = 1;
        TL0 = 0xAF;
        TH0 = 0xFB;
        TR0 = 1;
        flag=1;
    }
    else{
        //generate 20ms off
        Led = 0;
        TL0 = 0x00;
        TH0 = 0x70;
        TR0 = 1;
        flag = 0;
    }

}
void Timer1_ISR (void) interrupt 3
{
    TH1 = 0xE8;
    TR1 = 1;
    HZ38K = ~HZ38K; //generate frequency 38KHZ
    finalOutput();
}

void main()
{
    TMOD = 0x29	;  // 00101001
    //IE = 0x8A;
    ET0 = 1;
    EA = 1;
    ET1 = 1;

    TL0 = 0xAF;
    TH0 = 0xFB;
    TF0 = 0;
    TR0 = 1;
    Led = 0;

    TH1 = 0xE8;
    TF1 = 0;
    TR1 = 1;
    HZ38K = 0;

    while(1)
    {
        if (detectObstacle == 1)
            outputLed = 1;
        else
            outputLed = 0;
    }
}
