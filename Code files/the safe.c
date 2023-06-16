/*
 * the_safe.c
 *
 * Created: 8/22/2022 12:18:27 AM
 *  Author: HP.SXH08
 */ 


#include <avr/io.h>
#include "EEPROM_driver.h"
#include "LCD.h"
#include "LCD_config.h"
#include "keypad_driver.h"
#define  F_CPU 8000000UL
#include <util/delay.h>

int main(void)
{
	keypad_vInit();
	LCD_vInit();
	char pass[4];
	char passCheck[4];
    if(EEPROM_read(0x1f)==0xff)
	{
		LCD_vSend_string("Set Pass:");
		for (char i=0; i<4; i++)
		{
			while(keypad_u8check_press()==NOTPRESSED);
			EEPROM_write(0x20+i , keypad_u8check_press());
			LCD_vSend_char(keypad_u8check_press());
			_delay_ms(50);
			LCD_movecursor(1,10+i);
			LCD_vSend_char('*');
		}
		EEPROM_write(0x1f , 0x00);
		LCD_clearscreen();
		LCD_vSend_string("Pass saved!");
	}
	else
	{
		char k =3;
		while(k)
		{
			LCD_clearscreen();
			LCD_vSend_string("Check Pass:");
			for (char i=0; i<4; i++)
			{
				while(keypad_u8check_press()==NOTPRESSED);
				passCheck[i] = keypad_u8check_press();
				LCD_vSend_char(passCheck[i]);
				_delay_ms(50);
				LCD_movecursor(1,12+i);
				LCD_vSend_char('*');
			}
			for(int i=0; i<4; i++)
			{
				if(EEPROM_read(0x20+i) != passCheck[i])
				{
					LCD_clearscreen();
					LCD_vSend_string("Wrong Pass!");
					_delay_ms(50);
					k--;
					if(k==0)
					{
						LCD_clearscreen();
						LCD_vSend_string("No More Trailes..");
					}
					else
					{
						LCD_clearscreen();
						LCD_vSend_string("you have ");
						LCD_vSend_char(k+48);
						LCD_vSend_string(" more");
						LCD_movecursor(2,1);
						LCD_vSend_string("trails.");
						_delay_ms(50);
					}
					break;
				}
				if(i==3)
				{
					LCD_clearscreen();
					LCD_vSend_string("Safe Opened!");
					k=0;
				}
			}
		}
	}
}