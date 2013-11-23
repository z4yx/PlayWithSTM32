#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"

void USART1_Config(void);
int putchar(int ch);
int getchar();
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);

#endif /* __USART1_H */
