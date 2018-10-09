#include "mbed.h"
#include "ads1256.h"
DigitalOut myled(PC_7);
DigitalOut B(PE_14);
DigitalOut G(PE_8);
DigitalOut R(PB_1);
AnalogIn   temp(ADC_TEMP); 
ADS1256 myADC(PF_9,PF_8,PF_7,PF_6,PF_4,PB_5,PE_5);
#define  NUM     16
double ADS1255_DataFormatting(uint32_t Data , double Vref ,uint8_t PGA)
{
    /*
            Vin = ( (2*Vr) / G ) * ( x / (2^23 -1))
    */
    double ReadVoltage;
    if(Data & 0x00800000)
    {
        Data = (~Data) & 0x00FFFFFF;
        ReadVoltage = -(((double)Data) / 8388607) * ((2*Vref) / ((double)PGA));
    }
    else
        ReadVoltage =  (((double)Data) / 8388607) * ((2*Vref) / ((double)PGA));
    return(ReadVoltage);
}
void swap(uint32_t *a, uint32_t *b)  
{  
    int     c;  
     c = *a;  
    *a = *b;  
    *b =  c;  
} 
uint32_t GetAverage(uint32_t *dat,uint16_t leng)
{ 
    int i,j;
   uint32_t Average;
   for (i = 0; i < leng; i++)  
    {  
        for (j = i + 1; j < leng; j++)  
        {  
            if (dat[i] > dat[j])  
            {  
                swap(&dat[i], &dat[j]);  
            }
        }
    }
        
        Average=dat[leng/2];
 
    return  Average;
}
  void ReadASingleData(uint8_t AINP,uint8_t AINN)
{
    double ReadVoltage;
    uint32_t Data[NUM];
    uint16_t i;
    myADC.write_reg(0x01,(AINP<<4) | AINN );
    myADC.write_reg(0x03,0x72);
    myADC.WriteControl(SELFCAL);
    wait_us(5);
    myADC.WriteControl(SYNC);
    wait_ms(20);
    myADC.WriteControl(WAKEUP);
    wait_us(5);
 
        for(i=0;i<NUM;i++)
        {          
            Data[i] = myADC.ReadData(); 
            
        }
        ReadVoltage =ADS1255_DataFormatting(GetAverage(Data,NUM),2.5,1); 
 //  printf("Voltage=%4.4f temp=%4.4f\n", ReadVoltage, (ReadVoltage-0.1)*350/2.3-50);    
    printf("Voltage=%4.4f Speed=%4.4f\n", ReadVoltage, (ReadVoltage-0.1)*30/2.3); 
} 
int main() {
    printf("modular-2 20mA Analog IO module test \n"); 
    printf("STM32F429 internal temperature=%4.4f\n", temp.read()*100);
  //  myADC.SetPGAgain(0x00);//X1
  //SlectChannel(0x01,0x08);

    while(1) {
        myled = !myled; // LED is ON
        wait(1.0); // 1 sec
       ReadASingleData(0x01,0x08);
    }
}
