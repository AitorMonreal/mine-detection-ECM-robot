#include <xc.h>
#include "dc_motor.h"

#define _XTAL_FREQ 8000000 //define the XTAL Frequency (8MHz)

void initPWM(void) {
    PTCON0 = 0b00000000; // free running mode, 1:64 prescaler = 32 us
    PTCON1 = 0b10000000; // enable PWM timer
    PWMCON0 = 0b01101111; // PWM1/3 enabled enabled
    PWMCON1 = 0x00; // special features, all 0 (default)

    PTPERL = 199; // base PWM period low byte 8,000,000*1.5/1000
    PTPERH = 199>>8; // base PWM period high byte
    PDC0L = 0b00000000; // PDC1L bits 7:2 are on period bits 5:0
    PDC0H = 0b00000000; // PDC1H bits 5:0 are on period bits 11:6
    PDC1L = 0b00000000; // PDC3L bits 7:2 are on period bits 5:0
    PDC1H = 0b00000000; // PDC3H bits 5:0 are on period bits 11:6
}

void setMotorPWM(struct DC_motor *m) 
{
    int PWMduty; //tmp variable to store PWM duty cycle
    if (m->direction) { //if forward
        // low time increases with power
        PWMduty = m->PWMperiod - ((int) (m->power)*(m->PWMperiod)) / 100;
    } 
    else { //if reverse
        // high time increases with power
        PWMduty = ((int) (m->power)*(m->PWMperiod)) / 100;
    }
    
    PWMduty = (PWMduty << 2); // two LSBs are reserved for other things
    *(m->dutyLowByte) = PWMduty & 0xFF; //set low duty cycle byte
    *(m->dutyHighByte) = (PWMduty >> 8) & 0x3F; //set high duty cycle byte

    if (m->direction) { // if direction is high,
        LATB = LATB | (1 << (m->dir_pin)); // set dir_pin bit in LATB to high
    } 
    else { // if direction is low,
        LATB = LATB & (~(1 << (m->dir_pin))); // set dir_pin bit in LATB to low
    }
} 

void setMotorValues(void){
    TRISB = 0; //set all data direction registers to output for B pins
    LATB=0; //set the output data latch levels to 0 on all B pins
    
    motorL.power = 0; //zero power to start
    motorL.direction = 0; //set default motor direction
    motorL.dutyLowByte = (unsigned char *) (&PDC0L); //store address of PWM duty low byte
    motorL.dutyHighByte = (unsigned char *) (&PDC0H); //store address of PWM duty high byte
    motorL.dir_pin = 0; //pin RB0/PWM0 controls direction
    motorL.PWMperiod = 199; //store PWMperiod for motor

    motorR.power = 0; //zero power to start
    motorR.direction = 0; //set default motor direction
    motorR.dutyLowByte = (unsigned char *) (&PDC1L); //store address of PWM duty low byte
    motorR.dutyHighByte = (unsigned char *) (&PDC1H); //store address of PWM duty high byte
    motorR.dir_pin = 2; //pin RB2/PWM2 controls direction
    motorR.PWMperiod = 199; //store PW period for motor
   
}

void setMotorFullSpeed(struct DC_motor *m_L, struct DC_motor *m_R) //set full speed for forward movement 
{
    for (m_L->power; (m_L->power) <= 87; (m_L->power)++) { //increase motor power of left motor until 87
        m_R->power = m_L -> power; //set motor power of right motor to that of the left motor
        setMotorPWM(m_L); //set PWM high time
        setMotorPWM(m_R); //set PWM high time
        __delay_ms(1); //delay of 1 ms 
    }
}

void setMotorFullSpeedR(struct DC_motor *m_L, struct DC_motor *m_R) //set full speed for reverse movement
{
    for (m_R->power; (m_R->power) <= 90; (m_R->power)++) { //increase motor power until 90, this is higher than when moving forward since motor isn't as powerful when moving backwards
        m_L->power = (m_R -> power) -9.1; //different power for left and right motor due to drifting when moving backwards
        setMotorPWM(m_L); //set PWM high time
        setMotorPWM(m_R); //set PWM high time
        __delay_ms(1); //delay of 1 ms 
    }
}

void stop(struct DC_motor *m_L, struct DC_motor *m_R) 
 {
    for (m_L->power; (m_L->power) > 0; (m_L->power)--) { //decrease motor power of left motor until 0
        m_R->power = m_L -> power; //set motor power of right motor to that of the left motor
        setMotorPWM(m_L); //set PWM high time
        setMotorPWM(m_R); //set PWM high time
        __delay_ms(1); //delay of 1 ms
    }
}

void fullSpeedAhead(struct DC_motor *m_L, struct DC_motor *m_R){
    m_L->direction = 1; //set direction to forward
    m_R->direction = 1; //set direction to forward
    setMotorFullSpeed(m_L, m_R); // both motors forward and up to full power
} 

void fullSpeedReverse(struct DC_motor *m_L, struct DC_motor *m_R){
    m_L->direction = 0; //set direction to reverse
    m_R->direction = 0; //set direction to reverse
    setMotorFullSpeedR(m_L, m_R); // both motors reverse and up to full power
} 

void turnRight(struct DC_motor *m_L, struct DC_motor *m_R) { // turn robot base right on the spot
    stop(m_L, m_R); 
    m_R->direction = 1; //set right motor to forward
    m_L->direction = 0; //set left motor to reverse
    setMotorFullSpeed(m_L, m_R); //set motors to full power
}

void turnLeft(struct DC_motor *m_L, struct DC_motor *m_R){ // turn robot base left on the spot
    stop(m_L, m_R);
    m_R->direction = 0; //set right motor to reverse
    m_L->direction = 1; //set left motor to forward
    setMotorFullSpeed(m_L, m_R); //set motors to full power
} 
