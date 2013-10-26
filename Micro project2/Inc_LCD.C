//===== Inc_LCD.C ======
//===================================================
//       LCD Subroutine Program 
//===================================================
#define PortLCD P2 	// Using P2.4,P2.5,P2.6 and P2.7
sbit EN=P2^3;
sbit RS=P2^2;
void LCD_WrDelay(char i)
{	unsigned int j;
	do
	{	for(j=0;j<500;j++) {}
	  	i--;
	}
	while(i>0);
}

void LCD_WrCmd(unsigned char i)
{	PortLCD = i | 0x0f;
	RS = 0;
	EN = 1;
	LCD_WrDelay(2);
	EN = 0;
	LCD_WrDelay(2);

	PortLCD = (i<<4)| 0x0f;
	RS = 0;
	EN = 1;
	LCD_WrDelay(2);
	EN = 0;
	LCD_WrDelay(2);
}

void LCD_WrData(unsigned char i)
{	PortLCD = i | 0x0f;
	RS = 1;
	EN = 1;
	LCD_WrDelay(2);
	EN = 0;
	LCD_WrDelay(2);

	PortLCD = (i<<4)| 0x0f;
	RS = 1;
	EN = 1;
	LCD_WrDelay(2);
	EN = 0;
	LCD_WrDelay(2);
}

void LCD_Initial(void)
{	LCD_WrCmd(0x33);
	LCD_WrCmd(0x32);
	LCD_WrCmd(0x28);
	LCD_WrCmd(0x0f);
	LCD_WrCmd(0x06);
	LCD_WrCmd(0x01);
}

void LCD_Goto(unsigned char i)
{ 	i |=0x80;
	LCD_WrCmd(i);
}

void LCD_Print(void)
{	char i=0;
	do
	{	LCD_WrData(LCD_Buff[i]);
		i++;
	}
	while(LCD_Buff[i] != '\0');
}
