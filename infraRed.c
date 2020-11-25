#include <xc.h>
#include <string.h>
#include <stdio.h>
#include "dc_motor.h"
#include "RFID.h"

int j; //variable to store the executed moves in a buffer
char trace[400] = {0}; //buffer to store executed moves, 400 is max array size - too much memory if greater

void initCap(){
    
    ANSEL0 = 0; // set AN<0:7> as digital I/O
    ANSEL1 = 0; // set AN8 as digital I/O    
    
    // Set up right IR sensor     
    LATAbits.LATA2 = 0; // Set output data latch level to 0 on RA2  
    TRISAbits.RA2 = 1; // RA2 is an input     
    CAP1CON = 0b01000110; // //Pulse-Width Measurement mode, every rising to falling edge    
    CAP1BUFH = 0; // Clear higher bits     
    CAP1BUFL = 0; // Clear lower bits     
    
    // Set up left IR sensor     
    LATAbits.LATA4 = 0; // Set output data latch level to 0 on RA4   
    TRISAbits.RA4 = 1; // RA4 is an input     
    CAP3CON = 0b01000110; //Pulse-Width Measurement mode, every rising to falling edge   
    CAP3BUFH = 0; // Clear higher bits    
    CAP3BUFL = 0; // Clear lower bits     
      
    T5CON = 0b00011001; // set up Timer5
}

void moveAlbert(int *pcheck) { //function to move robot to the bomb

    int signal; // gives difference between IR sensor width values to determine motor movement
    j=1; //sets initial value of j to 1

    while(*pcheck!=1 && j<436){ //moves the robot while j has not reached the limit of the array and the interrupt has not been triggered
        int widthRight = CAP1BUFL | CAP1BUFH<<8; //combines low and high bytes to give PWM width
        int widthLeft = CAP3BUFL | CAP3BUFH<<8; //combines low and high bytes to give PWM width
    
        signal = widthRight-widthLeft; //turn right for negative signal, turn left for positive signal
        
        if(signal<-750){ //checks if the robot is moving too far left
            turnRight(&motorL, &motorR); 
            __delay_ms(20); //turns right for 20 ms
            stop(&motorL, &motorR);
            trace[j] = 'R'; //stores that a 'right' move was executed
        }
        else if (signal > 750){ //checks if the robot is moving too far right
            turnLeft(&motorL, &motorR); 
            __delay_ms(20); //turns left for 20 ms
            stop(&motorL, &motorR);
            trace[j]= 'L'; //stores that a 'left' move was executed
        }
        else{
            fullSpeedAhead(&motorL, &motorR); 
            __delay_ms(100); //moves forward for 100 ms
            trace[j]= 'F'; //stores that a 'forward' move was executed
        }
        j++; //increments j
    }
}

void returnAlbert(){ //function to return robot to the starting position

    j--; //decreases j
    
    while (j>0){
    
        if (trace[j]=='R'){ //if the robot executed a right move in the moveAlbert() function, a left move should be executed for the return
            turnLeft(&motorL, &motorR); 
            __delay_ms(20); //turns left for 20 ms
            stop(&motorL, &motorR);
        }
        else if (trace[j]=='L'){  //if the robot executed a left move in the moveAlbert() function, a right move should be executed for the return
            turnRight(&motorL, &motorR); 
            __delay_ms(20); //turns right for 20 ms
            stop(&motorL, &motorR);
        }
        else if (trace[j]=='F'){  //if the robot executed a forward move in the moveAlbert() function, a backwards move should be executed for the return
            fullSpeedReverse(&motorL, &motorR); 
            __delay_ms(100); //moves backward for 100 ms 
        }
        j--; //decreases j
        
    }
    stop(&motorL, &motorR); //stops once starting position is reached
    
}
