/* 
 * File:   RFID.h
 * Author: ik1616
 *
 * Created on 30 November 2018, 13:15
 */

#ifndef RFID_H
#define	RFID_H


#pragma config OSC = IRCIO, MCLRE = OFF, LVP = OFF

void initRFID(void);
char getCharSerial(void);
void getCode(void);


#endif	/* RFID_H */

