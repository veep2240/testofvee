
//=========== Lab21_Serial.C ===========
#include <reg52.h>         
#include <stdio.h> 
#include <ctype.h>
 char LCD_Buff[16];


#include"Inc_LCD.C"


#define PortO_Seg P0 
bit	LeftRightDisp;

sbit SW0 = P1^0;
sbit SW1 = P1^1;
sbit SW2 = P1^2;
sbit SW3 = P1^3; 
sbit SW36 = P3^6;
sbit SW37 = P3^7;

sbit CS_Seg0	= P1^4;
sbit CS_Seg1	= P1^5;
//set time
 int i;
 int a1=0;
 int a2=0;
 int a3=0;
 int a4=0;
//
unsigned int time2=30;
unsigned int time1=30;
int b[4]={68,69,71,72};
int num[4]={0,0,0,0};
 int Count=-1;
 int final=0;
 int Count1;
 int Count2;
unsigned int mode=2;  
int p=1;
code unsigned char CodeSeg[] = { 0x77,0x11,0x6D,0x5D,0x1B,0x5E,0x7E,0x15,		   
								 0x7F,0x5F };
code unsigned char CodeSeg1[] = { 0x08};
void lcd10();
//====================================================
void lcd10(void)
{	
				 
    			LCD_Goto(0);
				sprintf(LCD_Buff,"  cross word ");
				LCD_Print();
			
				LCD_Goto(64);
				sprintf(LCD_Buff,"1=%d%d:%d%d",Count1/60/10%10,Count1/60%10,Count1%60/10,Count1%60%10);
				LCD_Print();

				LCD_Goto(73);
				sprintf(LCD_Buff,"2=%d%d:%d%d",Count2/60/10%10,Count2/60%10,Count2%60/10,Count2%60%10);							         			   
				LCD_Print();	
}

//====================================================
void Delay(unsigned char msec)
	{	unsigned char i,j;

		while(msec)
			{	for(i=1;i<=250;i++) 		//not :
					for(j=1;j<=113;j++); 	//with ;
				msec--;
			} 
	}

//====================================================
 
//====================================================
 	    void int1_isr(void) interrupt 2 
	{	
		EX1 = 0;
		if(p!=1)
		{	// Disable Interrupt INT1
		 Count2=Count;
		 Count = Count1;
		}
		
		p=1;
  		CS_Seg0 = 0;	
		CS_Seg1 = 0; 
		EX1 = 1;	// Enablr Interrupt INT1 
		IE1 = 0;
	}
//====================================================
	  void int0_isr(void) interrupt 0 
	{	
		EX0 = 0;	// Disable Interrupt INT0
		if(p!=2)
		{
		Count1=Count;
		Count = Count2;
		}
		p=2;
  		CS_Seg0 = 0;	
		CS_Seg1 = 0; 
		EX0 = 1;	// Enablr Interrupt INT0 
		IE0 = 0;
	}
//====================================================
void main (void) 
	{
	
		LCD_Initial();

		SCON  	= 0x50;		// SCON: mode 1, 8-bit UART, enable rcvr      
    	RCAP2H 	= 0xFF; 	// Config Timer2
		RCAP2L	= 0xC4; 	// 9600 FFC4@18.432MHz 
  		TI    	= 1;		// Set TI to send first char of UART     
		

		ET0 = 1;    	// Enable External 0 Interrupt

		IT0 = 1;    // Adge Trigger Interrupt
		EX0 = 1;    // Enable External 0 Interrupt
		IT1 = 1;    // Adge Trigger Interrupt
		EX1 = 1;    // Enable External 1 Interrupt
		
		EA  = 1;    	// Globol Interrupt Enable 
		
		Count1=Count;
		Count2=Count;
		LCD_Initial();
		LCD_Goto(0);
		sprintf(LCD_Buff,"  SELECT  MODE");
		LCD_Print();
		LCD_Goto(64);
		sprintf(LCD_Buff," A=UP    B=DOWN");
		LCD_Print();
		
		Delay(5);
		while(1)//step 1
		{
			  if(SW3==0)//mode2 time down
			  {
			  	mode=2;
				break;
			  }
			  if(SW2==0)//mode1 time up
			  {			 	
			    mode=1;
				break;
			  }
		}
		LCD_Initial();
		LCD_Goto(0);
		sprintf(LCD_Buff,"   TIME SET");
		LCD_Print();
		LCD_Goto(64);
		sprintf(LCD_Buff,"    00:00");
		LCD_Print();
		LCD_Goto(68);
		i=0;
		
		while(1)//step 2
		{		
			  	if(SW0==0)
				{  Count=(num[0]*10+num[1])*60 +(num[2]*10)+num[3];
				   if(Count==-1||Count==0)
				   { LCD_Goto(64);	
					 sprintf(LCD_Buff,"   Insert Time");
					 LCD_Print();
					 Delay(10);
					 LCD_Initial();
					 LCD_Goto(0);
					 sprintf(LCD_Buff,"   TIME SET");
					 LCD_Print();
					 LCD_Goto(68);
					 sprintf(LCD_Buff,"%d%d:%d%d",num[0],num[1],num[2],num[3]);
					 LCD_Print();
					 LCD_Goto(68);
				   }
				   else
				   {
					break;
				   }

				   SW0=1;
				   Delay(4);
				}

				if(SW1==0)
				{  		if(i>2)
						{
						  i=0;
						}
						else{
							i++;
							}

						LCD_Goto(b[i]);	
				   SW1=1;
				   Delay(4);	
				}
				if(SW2==0)
				{	 LCD_Goto(68);
					 num[i]=(++num[i])%10;
					 num[2]=num[2]%7;
					 sprintf(LCD_Buff,"%d%d:%d%d",num[0],num[1],num[2],num[3]);
					 LCD_Print();
					 LCD_Goto(b[i]);
				   SW2=1;
				   Delay(4);
				}
				if(SW3==0)
				{
					 LCD_Goto(68);
					 num[i]=(--num[i])%10;
					 num[2]=num[2]%7;
					 if(num[i]<0)
					 {
					 	num[i]=9;
					 }
					 sprintf(LCD_Buff,"%d%d:%d%d",num[0],num[1],num[2],num[3]);
					 LCD_Print();
					 LCD_Goto(b[i]);
				     SW3=1;
				     Delay(4);	
				}
		}
		LCD_Initial();
		if(mode==1)
		{
			final=Count-1;
			Count1=0;
			Count2=0;
			Count=0;
		}
		if(mode==2)
		{
			final=0;
			Count1=Count;
			Count2=Count;
		}
		SW37=0;
		SW36=1;

		LCD_Goto(0);
		sprintf(LCD_Buff," SELECT PLAYER:");
		LCD_Print();
		LCD_Goto(15);
		sprintf(LCD_Buff,"%d",p);
		LCD_Print();	
		LCD_Goto(64);
		sprintf(LCD_Buff,"     START");
		LCD_Print();	
		LCD_Goto(15);	

		while(1)
		{
			 if(SW3==0)
			 {	SW37=1;
				SW36=0;
			 	p=2;
				sprintf(LCD_Buff,"%d",p);
				LCD_Print();
				LCD_Goto(15);
			 }
			 if(SW2==0)
			 {	SW37=0;
				SW36=1;
			 	p=1;
				sprintf(LCD_Buff,"%d",p);
				LCD_Print();
				LCD_Goto(15);
			 }
			 if(SW0==0)
			 {
			 	break;
			 }
		}
		LCD_Initial();
		if(mode==1)//time up
		{
		while (1) 		
			{	
	  					
				Delay(10);
				Count++;
				
				if((Count1>final)&&(Count2>final))
				{
									EX1 = 1;
									EX0 = 1;
								   break;
				}
				else{
				if(p==1)	
				{
					
					if(Count1>final)
					{	Count = Count2;
						p=2;
					}
					else
					{
					 Count1=Count;
					}
				}
				else if(p==2)
				{
					
					if(Count2>final)
					{	Count = Count1;
						p=1;
					}
					else
					{
					Count2=Count;
					}
				}

				}
				lcd10();
			}
		}

		else if(mode==2)//time down
		{
		while (1) 		
			{	
	  					
				Delay(10);
				Count--;
				
				if((Count1<=final)&&(Count2<=final))
				{
									EX1 = 1;
									EX0 = 1;
								   break;
				}
				else{
				if(p==1)	
				{
					
					if(Count1<=final)
					{	Count = Count2;
						p=2;
						SW37=0;
						SW36=1;
					}
					else
					{
					 Count1=Count;
					}
				}
				else if(p==2)
				{
					
					if(Count2<=final)
					{	Count = Count1;
						p=1;
						SW37=1;
						SW36=0;
					}
					else
					{
					Count2=Count;
					}
				}

				}
				lcd10();
			}
		}

			LCD_Goto(0);
			sprintf(LCD_Buff,"     End Game ");
			LCD_Print();

			LCD_Goto(0);
			while(1)
			{
				  if(SW1==0)
				  {
				  SW0=0;
				  LCD_Initial();
				  LCD_Goto(0);
				  sprintf(LCD_Buff,"                ");
			      LCD_Print();
				  LCD_Goto(64);
				  sprintf(LCD_Buff,"                ");
			      LCD_Print();
				  break;
				  }
			}
		
		  
 }


