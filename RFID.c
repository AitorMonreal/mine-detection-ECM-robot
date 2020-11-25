#include <xc.h>
#include "lcd.h"
#include <string.h>
#include <stdio.h>
#include "RFID.h"
#pragma config OSC = IRCIO, WDTEN = OFF //internal oscillator

void initRFID(void) {
    
    //set data direction registers

    SPBRG = 207; //set baud rate to 9600
    BAUDCONbits.BRG16 = 1; //set baud rate scaling to 8 bit asynchronous mode
    TXSTAbits.BRGH = 1; //high baud rate select bit
    RCSTAbits.CREN = 1; //continuous receive mode
    RCSTAbits.SPEN = 1; //enable serial port, other settings default
    TXSTAbits.TXEN = 0; //disable transmitter, other settings default

}

char getCharSerial(void) {

    while (!PIR1bits.RCIF); //wait for the data to arrive
    return RCREG; //return byte in RCREG
    
}

void getCode(void) {

    //initialise variables and buffers
    int i = 0; //counter for the 10 data characters
    int k = 0; //counter for checksum 
    int checksum = 0; //stores the checksum value
    int sequence = 0; //stores the xor value of the 10 data characters
    int buf1[10]; //buffer to store 10 data characters
    int buf2[2]; //buffer to store checksum characters
    int buf[20]; //buffer to display 'Checksum Failed'

    SetLine(1); //clear screen
    getCharSerial(); //get the start control character

    if (RCREG == 0x02){ //check if the start control character is 0x02
        getCharSerial(); //get first character of the code
        
        while (i < 10){ //displays 10 characters, not including the checksum, and converts the ASCII characters to hex characters
      
            SendLCD(RCREG, 1); //send character to the LCD
            
            //convert ASCII character to hex character
            if (RCREG < 58) { //for ASCII characters 0-9
                buf1[i] = RCREG - 48; //subtract 48 to get corresponding hex character between 0 and 9, store converted value in buf1
            } 
            else if (RCREG > 64) { //for ASCII characters A-F
                buf1[i] = RCREG - 55; //subtract 55 to get corresponding hex character between A and F, store converted value in buf1
            }
           
            if (i % 2) { //when i is odd, i.e. after every second character
                sequence = sequence ^ (buf1[i] | (buf1[i -1] << 4)); //combine character pairs to give 8 bit number and xor this value with the previous xor value. 
            }
            getCharSerial(); //get next character of the code
            i++; //increment i
        }

        while (k < 2) { //convert checksum values from ASCII characters to hex characters 
            if (RCREG < 58) { //for ASCII characters 0-9
                buf2[k] = RCREG - 48; //subtract 48 to get corresponding hex character between 0 and 9, store converted value in buf2
            } 
            else if (RCREG > 64) { //for ASCII characters A-F
                buf2[k] = RCREG - 55; //subtract 55 to get corresponding hex character between A and F, store converted value in buf2
            }
            
            getCharSerial(); //get next character of the checksum
            k++; //increment k
        }
         
        checksum = buf2[k-1] | (buf2[k - 2] << 4); //combine the checksum pair to give 8 bit number 
        
        if (sequence !=  checksum) { //compare checksum to the xor-ed value from the data character set
            SetLine(1); //clear screen
            
            sprintf(buf,"Checksum Failed"); 
            LCD_String(buf); //if checksum failed the code is removed from the screen and error message is displayed
        }
    }     
}




