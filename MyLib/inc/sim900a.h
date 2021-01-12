#ifndef __SIM900A_H__
#define __SIM900A_H__	 
#include "system.h"
#include "string.h"
#include "delay.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	

u8* sim900a_check_cmd(u8 *str);
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8 sim900a_chr2hex(u8 chr);
u8 sim900a_hex2chr(u8 hex);
int sim900a_chr2dec(char s[]);
void sim900a_dec2chr(int d);
void sim900a_init(void);
#endif
