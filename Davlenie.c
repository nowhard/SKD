#include "ADUC847.H"
#include <stdio.H>
#include <stdlib.H>
#include <string.H>
#include <math.h>
#include <intrins.h>
#include "flash.h"

#define DEBUG  0

#define kof_dav 1000.0
#define kol_izmer 3
#define kol_izmer_yar 1				
#define delay_led 2
#define tochka_max 500.999 //300.999 - поставить если датчик на 600, 500.999 - поставить если датчик на 1000 

unsigned char idata str[20]={0};	  
volatile float adc_float=0;	// long
volatile float adc_float1=0;
float xdata interval=0;
unsigned char adc_gotov=0;
float xdata verhniy_pridel=1.200;
float xdata nijniy_pridel=0.0;
float xdata koef_usileniya_dav=0;
float xdata davlenie=0;
const float ves_raz=0.00000015258790881489620386316822328189;// 0.320 - 0.000000038146977203724050965792055820472;  // 0.640 - 0.000000076293954407448101931584111640943; // 1.28 - 0.00000015258790881489620386316822328189
//const float ves_raz_adc2=0.00000014901162082026128889687591176485;
volatile long adc=0;
volatile long adc2=0;
unsigned int data kol_byte=0; // количество выводимых символов
long nagruzka=0;
unsigned char  yar=0;
bit lcd_out=1;
bit verh=0;
bit niz=0;
char yar_gotov=0;

void delay(int length);
//---------------------------------
/*float float_ceil_floor(float fl)
{
float rezult=0;

rezult=fl-(long)fl;

if(rezult>0.5)
 {
 fl=(long)(fl)+1;
 return fl;
 }
else
 {
 fl=(long)fl;
 return fl;
 }
} */
//---------------------------------
unsigned char strlench(unsigned char *s)
{
unsigned char i=0;
for(i=0;i<256;i++)
{
if(s[i]==0x00)
return i;
}
}
//---------------------------------
void delay(int length)  
{
while(length >=0)
length--;
}
//---------------------------------
void perevod_FloatToString(float adc,unsigned char *s,int p)
{
unsigned char i1=0;
unsigned char y1=0;
unsigned char tochka=5;	 // знаков перед точкой 000. если есть минус то на один знак больше дл€ минуса
unsigned char p1=(char)p;
unsigned long t=0;
bit znak=0;
bit zameni=1;
char r=0;
if(adc<0)
{
adc=adc*(-1.0);
//tochka=5;
znak=1;
r++;
}
t=(long)(adc*ceil(pow(10,p)));
 for(i1=0;i1<=(int)(p+tochka);i1++)		// 4 значит сколько знаков перед точкой вместе с ней т.е. 000.=4, p это	сколько знаков после
  {							    // точки. Ётот цикл переводит переменную adc в символы но не в ASCII и записывает эти симловы
  if(i1<(int)(p+tochka))				// в массив s, вместе с точкой котора€ €вл€етс€ 4-ым символом, наоборот, и в конец вставл€ет 0х00.
  {
  if(i1==p)
  s[i1]=0x2E;
  else
  {
  s[i1]=(char)((t%10)|0x30);
  t=t/10;
  }
  }
  else
  s[i1]=0x00;
  }
 for(i1=0;i1<(int)((int)(p+tochka+r)/2);i1++)	// переворачиваем строчку и удал€ем лишние нули(0х30), вставл€€ вместо них 0х00,                                        
  {									// до первой значещей цифры перед зап€той
   y1=s[i1];
   if(s[(int)(p+tochka)-1-i1]==0x30&&(int)(p+tochka)>p1+2&&zameni==1)
    {
	if(znak)
	 {
     s[(int)(p+tochka)-1-i1]=0x2D;  // вставл€ем знак минус если отрицательное число
	  if(p!=p1)
	   {
	   s[(int)(p+tochka)-1-i1+1]=0x00;
	   //i=-1;
       //p=p-1;
	   }
	 }
	else
	  s[(int)(p+tochka)-1-i1]=0x00;
	  i1=-1;
      p=p-1;
	}
   else
    {
	if(znak)
	{
	zameni=0;
    s[i1]=s[(int)(p+tochka)-i1];
	s[(int)(p+tochka)-i1]=y1;
	}
	else
	{
	zameni=0;
	s[i1]=s[(int)(p+tochka)-1-i1];
    s[(int)(p+tochka)-1-i1]=y1;
	}
	}
  }
}
//------------------------------------
void _ADC_ (void) interrupt 6
{
if(ADC0CON2==0x0D)
{
 RDY0=0;
 adc=ADC0H;
 adc=(((adc<<8)|ADC0M)<<8)|ADC0L;
 adc=adc-0x800000;

  if(adc_gotov<kol_izmer)
    {
	//if(lcd_out==1)
    // {
     adc_float+=adc;
	 adc_gotov++;

	 if(adc_gotov==kol_izmer)
        {
	    ADC0CON1=0x07;	// +/- 1.280 mV
	    ADC0CON2=0x0C;	// AIN5 -> AIN6
        } 

	}
}
else
if(ADC0CON2==0x0C)
{
RDY0=0;
yar_gotov++;
adc2=ADC0H;
adc2=(((adc2<<8)|ADC0M)<<8)|ADC0L;
yar=adc2/1118481;  // получение 1/15 части дл€ €ркости

if(yar_gotov==kol_izmer_yar)
 {
 ADC0CON1=0x07;	 // 0x06 - +/- 0.640 0x7 - +/- 1.28 mV
 ADC0CON2=0x0D;	 //  AIN7 -> AIN8
 //ADCMODE=0x73;
 } 
} 
}
//------------------------------
void LED_OUT(unsigned char *out,char kol,bit initch,unsigned char *inith)  // 1-массив данных 2-размер массива данных 3- бит режима вывода:0 - вывод информации 1- инициализаци€ устройства 4- массив инициализации микросхемы вывода ћах7219
{
unsigned char init[2]={0};	 // init[0] - адрес регистра init[1] - данные
bit tocka=0;				 // признак точки 
unsigned char propusk=0;	 // не используетс€
char kol_pered=0;			 // перебор знакомест
unsigned char kol_byt=0;	 // перебор двух байтных данных
unsigned char byte=0;		 // дл€ разбора байта данных

unsigned char bit_count=0;

if(initch==0)
{
for(kol_pered=5;kol_pered>0;kol_pered--)  // выводим п€ть знакомест
  {
   init[0]=kol_pered;
   delay(delay_led);
   CS=0;
   if(kol<=0)
   {
   init[1]=0x0F;
   kol=1;
   }
   else
   {
    if(out[kol-1]==0x2E)   // обнаружение точки 
     {
     tocka=1;
     kol_pered++;	// позуцию знакоместа не мен€ем
	 kol--;			// сдвигаемс€ на следующий символ
     continue;
     }
    if(tocka==1)
     {
     init[1]=(out[kol-1]&0x0F);//|0x80; - вывод с точкой
     //tocka=0;	 - вывод с точкой
     }
    else
     init[1]=0x0F;//out[kol-1]&0x0F; - вывод чисел после точки
   }
  for(kol_byt=0;kol_byt<2;kol_byt++)  	  // вывод двух байтных данных
   {
    byte=init[kol_byt];
	 for(bit_count=0;bit_count<8;bit_count++) // побитный вывод байта
     {
     byte=byte<<1;
     DIN=CY;	 
	 delay(delay_led);//  задержка
     CLK=1;
     delay(delay_led);
     CLK=0;
	 delay(delay_led);
     }
   }
CS=1;
kol--;	 
  }
}
else
{
CS=0;
init[0]=inith[0];
init[1]=inith[1];

for(kol_byt=0;kol_byt<2;kol_byt++)  // включение питани€
   {
    byte=init[kol_byt];
	 for(bit_count=0;bit_count<8;bit_count++)
     {
     byte=byte<<1;
     DIN=CY;	 
	 delay(delay_led);//  задержка	  1.67 мсек
     CLK=1;
     delay(delay_led);
     CLK=0;
	 delay(delay_led);
     }
	 delay(delay_led);
   }
CS=1;
}
}
//------------------------------
void main (void)
{
unsigned int w=15;
//unsigned char sbros=0;
char len=0;

#if DEBUG==1
int it0=0;
int it1=0;
#endif

unsigned char  yars[2]=0; // €ркость
PLLCON=0x02; //0x48; 12.58 MGh

CS=1;	 // выбор кристала 
CLK=0;

flash_read (&verhniy_pridel,sizeof(verhniy_pridel),0x00000000);
flash_read (&nijniy_pridel,sizeof(nijniy_pridel),0x00000004);

if(_chkfloat_(verhniy_pridel)==4)
verhniy_pridel=0.2602;
if(_chkfloat_(nijniy_pridel)==4)
nijniy_pridel=-0.0022;	 // -0.0022

interval=verhniy_pridel-nijniy_pridel;
koef_usileniya_dav=kof_dav/interval;

#if DEBUG==1
SCON=0x52;	  // –ежим 1 - 8 бит, прием запрещен
T3CON=0x81;	//UART = 57600 Baud rate
T3FD=0x2D;
#endif

LED_OUT(NULL,NULL,1,"\x09\xFF"); // инициализаци€ decode-on
LED_OUT(NULL,NULL,1,"\x0B\x04"); // сканирование 5


ADC0CON1=0x07;	   // 0x06 - +/- 640 mV  // 0x07 - +/- 1.28
ADC0CON2=0x0D;	  //  AIN7 -> AIN8	   опора 1.25
//ADC1CON=0x0C;	  // AIN5 -> AIN6
SF=0x45;
ADCMODE=0x73;//0x73; // 0x63 - REJ60 включен 1 ј÷ѕ


EADC=1;
EA=1; //разрешение всех прерываний

while(1)
 {
  if(yar_gotov==kol_izmer_yar)
    {
	yar_gotov=0;
    yars[0]=0x0A;
    yars[1]=yar;
    LED_OUT(NULL,NULL,1,yars); // €ркость
	} 
if(adc_gotov==kol_izmer)
 {    
  //lcd_out=0;
  adc_float1=(float)((float)(adc_float/(float)(kol_izmer))*ves_raz);	//adc_float1=adc_float;

  if(INT1==0)
   {
   interval=((adc_float1-nijniy_pridel)/tochka_max)*kof_dav;
   verhniy_pridel=interval+nijniy_pridel;
   //verhniy_pridel-=0.000000076293954407448101931584111640943;
   koef_usileniya_dav=kof_dav/interval;
   flash_write (&verhniy_pridel,sizeof(verhniy_pridel),0x00000000);
   }

  if(INT0==0)
   {
   nijniy_pridel=adc_float1;//+0.000000076293954407448101931584111640943;
   interval=verhniy_pridel-nijniy_pridel;	  // маштабирование
   koef_usileniya_dav=kof_dav/interval;
   flash_write (&nijniy_pridel,sizeof(nijniy_pridel),0x00000004);
   }  

  adc_float=0;
  adc_gotov=0;
 // lcd_out=1;


  davlenie=((interval-(verhniy_pridel-adc_float1))*koef_usileniya_dav);
  if(davlenie<0)
  davlenie=0;
  if(davlenie>=999) // поставить 600 если датчик на 600, или 999 если датчик на 1000
  davlenie=999;

  perevod_FloatToString(davlenie,str,1);

LED_OUT(NULL,NULL,1,"\x09\xFF"); // инициализаци€ decode-on
LED_OUT(NULL,NULL,1,"\x0B\x04"); // сканирование 5
LED_OUT(NULL,NULL,1,"\x0C\x01"); // включение питани€
len=strlench(str);
//yar=0x0F;
yars[0]=0x0A;
yars[1]=yar;
LED_OUT(NULL,NULL,1,yars); // €ркость
LED_OUT(str,len,0,NULL);

#if DEBUG==1
it0=INT0;
it1=INT1;

printf("\r%.1f  %d  %d   %d   %lx    ",davlenie,(int)yar,it0,it1,adc2);
//printf("%.3d",(int)davlenie);
#endif	 
  
  }	 
 }
}
