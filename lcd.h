/* 
 * File:   lcd.h
 * Author: ik1616
 *
 * Created on 13 November 2018, 12:54
 */

#ifndef LCD_H
#define	LCD_H


//compiler directive to set clock to internal oscillator
#pragma config OSC = IRCIO, MCLRE = OFF, LVP = OFF
#define _XTAL_FREQ 8000000 //define _XTAL_FREQ so delay routines work
#define LCD_DB7 LATDbits.LATD1
#define LCD_DB6 LATDbits.LATD0
#define LCD_DB5 LATCbits.LATC2
#define LCD_DB4 LATCbits.LATC1
#define LCD_E LATCbits.LATC0
#define LCD_RS LATAbits.LATA6

void E_TOG(void);
void LCDout(unsigned char number);
void SendLCD(unsigned char Byte, char type);
void LCD_Init(void);
void SetLine(char line);
void LCD_String(char *string);


#endif	/* LCD_H */

