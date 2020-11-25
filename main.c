#include <xc.h>
#include <string.h>
#include <stdio.h>
#include "lcd.h"
#include "dc_motor.h"
#include "RFID.h"
#include "infraRed.h"
#pragma config OSC = IRCIO, WDTEN = OFF //internal oscillator
#define _XTAL_FREQ 8000000 //define the XTAL Frequency (8MHz)

int check=0; //global variable to check whether the interrupt has been triggered and the RFID code has been read

void main(){
    
    OSCCON = 0b11110010; // internal oscillator, 8MHz
    while(!OSCCONbits.IOFS); //Wait for OSC to become stable
    
    LATC=0; //set the output data latch levels to 0 on all C pins
    LATD=0; //set the output data latch levels to 0 on all D pins
    TRISC=0b10000000; //set the data direction registers to input for RC7 (RFID)
    TRISD=0; //set all data direction registers to output for D pins
    
    //Enable interrupts
    INTCONbits.GIEH = 1; //Global Interrupt Enable bit
    INTCONbits.PEIE = 1; //Peripheral Interrupt Enable bit
    PIE1bits.RCIE = 1; //EUSART Receive Interrupt Enable bit
    IPR1bits.RC1IP = 1; //EUSART Receive Interrupt Priority bit (High Priority)
    
    
    //Initialise all components    
    LCD_Init(); //initialise the LCD
    initCap(); //initialise CAPCON pins
    initPWM(); //initialise PWM signal for motor
    initRFID(); //initialise the RFID
    setMotorValues(); //initialise MotorL and MotorR
   
     
    while(1){

        moveAlbert(&check); //executes the moveAlbert() function to find the bomb (exits once an RFID signal is received based on the check variable)
              
        stop(&motorL, &motorR); 
        __delay_ms(1000); //stops for 1 second to ensure that there is enough time to read the code
        
        returnAlbert(); //executes the returnAlbert() function to bring robot back to starting position  
    }
    
}


void __interrupt(high_priority) my_hp_isr(void){
    
    if(PIR1bits.RCIF){ //checks if EUSART receive interrupt flag has been set
        
        check = 1; //set check to 1 to indicate that bomb was found, this variable is used to exit the moveAlbert() function
        getCode(); //executes the getCode() function to read and display the code on the LCD
        
        PIR1bits.RCIF = 0; //clear the interrupt flag
    }
    
}

