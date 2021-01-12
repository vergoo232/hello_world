#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "system.h" 
void uart_init(u32 bound);
void CheckBusy(void);
void USART_PutChar(USART_TypeDef* USARTx, uint8_t Data);
void USART_PutStr(USART_TypeDef* USARTx, uint8_t *str);
#endif
