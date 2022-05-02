/*****************************************************
This program was produced by the
CodeWizardAVR V2.03.4 Standard
Automatic Program Generator
� Copyright 1998-2008 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 2022/02/27
Author  : 
Company : 
Comments: 


Chip type           : ATmega16
Program type        : Application
Clock frequency     : 8.000000 MHz
Memory model        : Small
External RAM size   : 0
Data Stack size     : 256
*****************************************************/

#include <mega16.h>
#include <delay.h>
// I2C Bus functions
#asm
   .equ __i2c_port=0x18 ;PORTB
   .equ __sda_bit=1
   .equ __scl_bit=0
#endasm
#include <i2c.h>



int cmp,c;
  #define EEPROM_BUS_ADDRESS 0xc0   
 /*read a byte from the EEPROM*/ 
unsigned char compass_read(unsigned char address) 
{
unsigned char data;    
i2c_start();       
delay_us(100);
i2c_write(EEPROM_BUS_ADDRESS);
delay_us(100);
i2c_write(address);
delay_us(100);
i2c_start();  
delay_us(100);
i2c_write(EEPROM_BUS_ADDRESS | 1);
delay_us(100);
data=i2c_read(0);
delay_us(100);
i2c_stop(); 
return data;  

} 







// Alphanumeric LCD Module functions
#asm
   .equ __lcd_port=0x15 ;PORTC
#endasm
#include <lcd.h>

#include <delay.h>

#define ADC_VREF_TYPE 0x40

// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | (ADC_VREF_TYPE & 0xff);
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=0x40;
// Wait for the AD conversion to complete
while ((ADCSRA & 0x10)==0);
ADCSRA|=0x10;
return ADCW;
}
/////////////////////////////////////////////////////////////////

// Declare your global variables here
//////////////////////////////////////////////////sensor fun
struct SensorStruct{
    int l;
    int r;
    int b;
    int f;           
}   kaf, sharp;
    
eeprom int set_r,set_l,set_b,set_f;
int shb,shl,shr,d,v=150,kr,kl,kf,kb;
int i=0,min,number;
int j=0;
void sensor()
{
    min=1023;
    for(i = 0 ; i < 16 ; i++)
        {
        PORTB.7 = (i/8)%2;
        PORTB.6 = (i/4)%2;
        PORTB.5 = (i/2)%2;
        PORTB.4 = i%2;
        if(min>read_adc(0))
            {
            number = i;
            min = read_adc(0);
            }  
        }
    lcd_gotoxy(0,0);
    lcd_putchar((number/10)%10+'0');
    lcd_putchar((number/1)%10+'0'); 
    if(min<500)         lcd_putsf("T");
    else                lcd_putsf("F");
//    lcd_putchar((min/1000)%10+'0');
//    lcd_putchar((min/100)%10+'0');
//    lcd_putchar((min/10)%10+'0');
//    lcd_putchar((min/1)%10+'0');
    
    
    
    shb=read_adc(1);
    lcd_gotoxy(3,1);
    
    lcd_putsf("B");
    lcd_putchar((shb/100)%10+'0');
//    lcd_putchar((shb/10)%10+'0');
//    lcd_putchar((shb/1)%10+'0');
    
    /////////////////////////////////////
    shr=read_adc(2);
    lcd_gotoxy(6,1);
    
    lcd_putsf("R");
    lcd_putchar((shr/100)%10+'0');
//    lcd_putchar((shr/10)%10+'0');
//    lcd_putchar((shr/1)%10+'0');
    /////////////////////////////////////
    shl=read_adc(3);
    lcd_gotoxy(0,1);
    
    lcd_putsf("L");
    lcd_putchar((shl/100)%10+'0');
//    lcd_putchar((shl/10)%10+'0');
//    lcd_putchar((shl/1)%10+'0');
    
    d=shl-shr;
    if(PINC.3==1)
    {
    set_r=read_adc(4);
    set_l=read_adc(5);
    set_b=read_adc(6);
    set_f=read_adc(7);
    }    
    ////////////////////ldr caf right
    kr=read_adc(4)-set_r;
    lcd_gotoxy(9,0);
    
    lcd_putsf("R");
    lcd_putchar((kr/100)%10+'0');
    lcd_putchar((kr/10)%10+'0');
//    lcd_putchar((kr/1)%10+'0'); 
    /////////////////////////////ldr caf  left
    kl=read_adc(5)-set_l;
    lcd_gotoxy(9,1);
    
    lcd_putsf("L");
    lcd_putchar((kl/100)%10+'0');
    lcd_putchar((kl/10)%10+'0');
//    lcd_putchar((kr/1)%10+'0'); 
    ////////////////////////////ldr caf back
    kb=read_adc(6)-set_b;
    lcd_gotoxy(13,0);
    
    lcd_putsf("B");
    lcd_putchar((kb/100)%10+'0');
    lcd_putchar((kb/10)%10+'0');
//    lcd_putchar((kr/1)%10+'0'); 
    ///////////////////////////ldr  caf front
    kf=read_adc(7)-set_f;
    lcd_gotoxy(13,1);
    
    lcd_putsf("F");
    lcd_putchar((kf/100)%10+'0');
    lcd_putchar((kf/10)%10+'0');
//    lcd_putchar((kf/1)%10+'0');
                   
    
    ////////////////////////////////////////////////////////////////////////////
      cmp=compass_read(1)-c;
      
      if(cmp<0)         cmp=cmp+255;
      if(cmp>128)       cmp=cmp-255;
            
      if(cmp>=0)
      {
      lcd_gotoxy(4,0);
      lcd_putchar('+');
      lcd_putchar((cmp/100)%10+'0');
      lcd_putchar((cmp/10)%10+'0');
      lcd_putchar((cmp/1)%10+'0');        
      }       
      
      else
      {
      lcd_gotoxy(4,0);
      lcd_putchar('-');
      lcd_putchar((-cmp/100)%10+'0');
      lcd_putchar((-cmp/10)%10+'0');
      lcd_putchar((-cmp/1)%10+'0');      
      }
      
      if(-10<cmp<10) cmp=-cmp*2;
      else  cmp=-cmp;
         
    if(kr>40)   kaf.r = 1;
    else        kaf.r = 0;
    if(kl>40)   kaf.l = 1;
    else        kaf.l = 0;
    if(kb>20)   kaf.b = 1;
    else        kaf.b = 0;
    if(kf>40)   kaf.f = 1;
    else        kaf.f = 0;
    
    if(shr>280)   sharp.r = 1;
    else          sharp.r = 0;
    if(shl>300)   sharp.l = 1;
    else          sharp.l = 0;
    if(shb>300)   sharp.b = 1;
    else          sharp.b = 0;
    }
void motor(int L1, int L2, int R2,int R1 )
{
  L1 += cmp;
  L2 += cmp;
  R1 += cmp;
  R2 += cmp;
  
  if(R1>255)  R1=255;
  if(R2>255)  R2=255;
  if(L1>255)  L1=255;
  if(L2>255)  L2=255;
  
  if(R1<-255)  R1=-255;
  if(R2<-255)  R2=-255;
  if(L1<-255)  L1=-255;
  if(L2<-255)  L2=-255;
    ///////////////////L1
    if (L1>0)
    {      
        PORTD.3=0;
        OCR2=L1;
    } 
    else 
    {
        PORTD.3=1;
        OCR2=255+L1;
    }
    ///////////////////////////L2
    if (L2>0)
    {      
        PORTD.2=0;
        OCR1A=L2;
    } 
    else 
    {
        PORTD.2=1;
        OCR1A=255+ L2;
    }
    ///////////// ///////////////////R2
    if (R2>0)
    {      
        PORTD.1=0;
        OCR1B=R2;
    } 
    else 
    {
        PORTD.1=1;
        OCR1B=255+R2;
    }
    /////////////////////////R1
    if (R1>0)
    {      
        PORTD.0=0;
        OCR0=R1;
    } 
    else 
    {
        PORTD.0=1;
        OCR0=255+R1;
    }

}
void move(int direction)
    {
    if(direction == 0)      motor(v   , v   , -v  , -v   );
    if(direction == 1)      motor(v   , v/2 , -v  , -v/2 );
    if(direction == 2)      motor(v   , 0       , -v  , 0);
    if(direction == 3)      motor(v   , -v/2, -v  , v/2  );
    if(direction == 4)      motor(v   , -v  , -v  , v    );
    if(direction == 5)      motor(v/2 , -v  , -v/2, v    );
    if(direction == 6)      motor(0       , -v  , 0   , v);
    if(direction == 7)      motor(-v/2, -v  , v/2 , v    );    

    if(direction == 8)      motor(-v  , -v  , v   , v    );

    if(direction == 9)      motor(-v   , -v/2, v   , v/2 );
    if(direction == 10)     motor(-v   , 0       , v   ,0);
    if(direction == 11)     motor(-v   , v/2 , v   , -v/2);
    if(direction == 12)     motor(-v   , v   , v   , -v  );
    if(direction == 13)     motor(-v/2 , v   , v/2 , -v  );
    if(direction == 14)     motor(0        , v   , 0,-v  );
    if(direction == 15)     motor(v/2  , v   , -v/2, -v  );        
    } 
    
void moveForSeconds(int direction, int seconds)
    {
        int temp = v;
        v = 130;
        for(j=0; j<seconds*20; j++){
            sensor();
            move(direction);
        }
        v = temp;
    }
    
    void goal()
    {
//    if( (kaf.r && kaf.f)&& (kaf.r && kaf.f)&& (kaf.l && kaf.f)&& (kaf.l && kaf.b)&&(kaf.r && kaf.b) && (shr < 220)) move(number);
    }
    
    
    
void out()
    {
    ///////////////////////////outf & outb  
    if((shr > 270 || shl > 270) && ((kaf.l && kaf.f) || (kaf.r && kaf.f) || (kaf.l && kaf.b) || (kaf.r && kaf.b)))
        {
        if(number > 4 && number < 12)
            {
            moveForSeconds(0, 2);
            while(number < 4 && number > 12 && min > 500)
                {
                    sensor();
                    if(kaf.r || kaf.l || kaf.b || kaf.f) 
                        move(8);
                     
//                  else if(shr < 220 || shl > 220)     move(number);
                    else          
                        motor(0, 0, 0, 0);
                }
            }
        else
            {
            moveForSeconds(8, 2);
            while((number > 4 && number < 12) && min > 500)
                {
                    sensor();
                    if(kaf.r || kaf.l || kaf.b || kaf.f) 
                        move(0); 
                    else          
                        motor(0, 0, 0, 0);
                }
            }                     
        }
    ///////////////////////////outr  
    if((kaf.r && sharp.r) || (kaf.l && sharp.r) || (kaf.b && kaf.f && sharp.r))
        {  goal();
        moveForSeconds(12, 1);
        while(number < 8 && number > 0)
            {
                sensor();
                if(sharp.r || kaf.r || kaf.l || kaf.b || kaf.f) 
                              motor(-v/2,v/2,v/2,-v/2); 
                else          motor(0, 0, 0, 0);
            }                         
        }
    ///////////////////////////outl  
    if((kaf.l && sharp.l) || (kaf.r && sharp.l) || (kaf.b && kaf.f && sharp.l))
        {
        moveForSeconds(4, 1);
        while(number > 8)
            {
                sensor();
                if(sharp.r || kaf.r || kaf.l || kaf.b || kaf.f) 
                    motor(v/2,-v/2,-v/2,v/2); 
                else          
                    motor(0, 0, 0, 0);
            }                         
        }
    
    
    
}

/////////////////////////////////////////////////////////////////////

void main(void)
{
// Declare your local variables here

// Input/Output Ports initialization
// Port A initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTA=0x00;
DDRA=0x00;

// Port B initialization
// Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=In Func0=In 
// State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=T State0=T 
PORTB=0x00;
DDRB=0xFC;

// Port C initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTC=0x00;
DDRC=0x00;

// Port D initialization
// Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out 
// State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=0 State0=0 
PORTD=0x00;
DDRD=0xFF;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 31.250 kHz
// Mode: Fast PWM top=FFh
// OC0 output: Non-Inverted PWM
TCCR0=0x6C;
TCNT0=0x00;
OCR0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 31.250 kHz
// Mode: Fast PWM top=00FFh
// OC1A output: Non-Inv.
// OC1B output: Non-Inv.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer 1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=0xA1;
TCCR1B=0x0C;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: 31.250 kHz
// Mode: Fast PWM top=FFh
// OC2 output: Non-Inverted PWM
ASSR=0x00;
TCCR2=0x6E;
TCNT2=0x00;
OCR2=0x00;

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: Off
MCUCR=0x00;
MCUCSR=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x00;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

// ADC initialization
// ADC Clock frequency: 62.500 kHz
// ADC Voltage Reference: AVCC pin
// ADC Auto Trigger Source: None
ADMUX=ADC_VREF_TYPE & 0xff;
ADCSRA=0x87;

// I2C Bus initialization
i2c_init();

// LCD module initialization
lcd_init(16);
        delay_ms(1000);
        c=compass_read(1);


while (1)
       {        
    sensor();
    if(min<500)
        {
     out();
      
        if(number == 0 || number == 1 || number == 15)     move(number);
        else if(number < 4) move(number+2);
        else if(number < 8) move(number+3);
        else if(number >12) move(number-2);
        else                move(number-3);

        }  
    else
        {
     
        if(shb<200)    motor(-v/2+d,-v/2-d,v/2-d,v/2+d);
        else           motor(0,0,0,0);
        }    
    }
}
