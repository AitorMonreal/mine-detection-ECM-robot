/* 
 * File:   infraRed.h
 * Author: ik1616
 *
 * Created on 30 November 2018, 13:29
 */

#ifndef INFRARED_H
#define	INFRARED_H

#pragma config OSC = IRCIO, MCLRE = OFF, LVP = OFF

void initCap();
void moveAlbert(int *pcheck);
void returnAlbert();

#endif	/* INFRARED_H */

