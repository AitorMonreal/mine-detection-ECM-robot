#include "lcd.h"
#include <xc.h>
#include <string.h>
#include <stdio.h>

void E_TOG(void) {
    //don?t forget to put a delay between the on and off
    //commands! 5us will be plenty.
    LCD_E = 1;
    __delay_us(5); // 5us delay ? remember to define _XTAL_FREQ
    LCD_E = 0;
}

void LCDout(unsigned char number) {
    LCD_DB7 = (number & 0b1000)>>3;
    LCD_DB6 = (number & 0b0100)>>2;
    LCD_DB5 = (number & 0b0010)>>1;
    LCD_DB4 = number & 0b0001; //set data pins using the four bits from number
    E_TOG();
    __delay_us(5); // 5us delay
}

void SendLCD(unsigned char Byte, char type) {
    LCD_RS = type; // set RS pin whether it is a Command (0) or Data/Char (1)
    // using type as the argument
    LCDout((Byte & 0b11110000)>>4); // send high bits of Byte using LCDout function
    __delay_us(10); // 10us delay
    LCDout(Byte & 0b00001111); // send low bits of Byte using LCDout function
}

void LCD_Init(void) {
    LCD_DB7 = 0;
    LCD_DB6 = 0;
    LCD_DB5 = 0;
    LCD_DB4 = 0;
    LCD_E = 0;
    LCD_RS = 0; // set initial LAT output values (they start up in a random state)

    
    TRISD = 0;
    TRISAbits.RA6 = 0; // set LCD pins as output (TRIS registers)
    // Initialisation sequence code - see the data sheet
    __delay_ms(15); //delay 15mS
    LCDout(0b0011); // using LCDout
    __delay_ms(5); //delay 5ms
    LCDout(0b0011); //send 0b0011 using LCDout
    __delay_us(200); //delay 200us
    LCDout(0b0011); //send 0b0011 using LCDout
    __delay_us(50); //delay 50us
    LCDout(0b0010); //send 0b0010 using LCDout set to four bit mode
    __delay_us(50);
    
    SendLCD(0b00101000,0); //function set
    __delay_us(50);
    SendLCD(0b00001000,0); //display off
    __delay_us(50);
    SendLCD(0b00000001,0); //display clear
    __delay_ms(5);
    SendLCD(0b00000110,0);// entry mode set
    __delay_us(50);
    SendLCD(0b00001110,0); //display on
    __delay_us(50);
}

void SetLine(char line) {
    if (line ==1){
    SendLCD(0x80,0);//Send 0x80 to set line to 1 (0x00 ddram address)
    }
    else{
    SendLCD(0xC0,0);//Send 0xC0 to set line to 2 (0x40 ddram address)
    }
    __delay_us(50); // 50us delay
}

void LCD_String(char *string){
 //While the data pointed to isn?t a 0x00 do below
while(*string != 0){
 //Send out the current byte pointed to
 // and increment the pointer
SendLCD(*string++,1);
 __delay_us(50); //so we can see each character
 //being printed in turn (remove delay if you want
 //your message to appear almost instantly)
 }
}

