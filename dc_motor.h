/* 
 * File:   dc_motor.h
 * Author: ik1616
 *
 * Created on 13 November 2018, 11:46
 */

#ifndef DC_MOTOR_H
#define	DC_MOTOR_H
#define _XTAL_FREQ 8000000
struct DC_motor {
 char power; //motor power, out of 100
 char direction; //motor direction, forward(1), reverse(0)
 unsigned char *dutyLowByte; //PWM duty low byte address
 unsigned char *dutyHighByte; //PWM duty high byte address
 char dir_pin; // pin that controls direction on PORTB
 int PWMperiod; //base period of PWM cycle
};

struct DC_motor motorL, motorR;

void initPWM(void);
void setMotorPWM(struct DC_motor *m);
void setMotorValues(void);
void setMotorFullSpeed(struct DC_motor *m_L, struct DC_motor *m_R);
void setMotorFullSpeedR(struct DC_motor *m_L, struct DC_motor *m_R);
void stop(struct DC_motor *m_L, struct DC_motor *m_R); 
void fullSpeedAhead(struct DC_motor *m_L, struct DC_motor *m_R);
void turnLeft(struct DC_motor *m_L, struct DC_motor *m_R);
void turnRight(struct DC_motor *m_L, struct DC_motor *m_R);
void fullSpeedReverse(struct DC_motor *m_L, struct DC_motor *m_R);


#endif	/* DC_MOTOR_H */

