/*--------------------------------------------------------------------------
I2C.H

Prototypes for standard I/O functions.
Copyright (c) 1988-2002 Keil Elektronik GmbH and Keil Software, Inc.
All rights reserved.
--------------------------------------------------------------------------*/

#ifndef __I2C_H__
#define __I2C_H__

#ifndef EOF
 #define EOF -1
#endif


#pragma SAVE
#pragma REGPARMS
extern int Read_Write_I2C (bit ,char [],int ,unsigned char ); // ������ �������� ���� 1-�����,0-�����,������ �������� ������ ��� ������,����� ���������� ������ � �������,������ ���������� � �������� ���������� ���������, ���������� 1 ���� ��� ������ ������!
extern void delay (long int ); // ��������
#pragma RESTORE

#endif

