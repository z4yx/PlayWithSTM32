#ifndef __COMMON__H__
#define __COMMON__H__

#include "stm32f10x.h"
#include <stdint.h>

#define NULL ((void*)0)
#ifndef bool
#define bool uint8_t
#define true 1
#define TRUE true
#define false 0
#define FALSE false
#endif

extern void USART1_printf(USART_TypeDef* USARTx, char *Data, ...);

#define DBG_MSG(format, ...) USART_printf(USART1, "[Debug]%s: " format "\r\n", __func__, __VA_ARGS__)
#define ERR_MSG(format, ...) USART_printf(USART1, "[Error]%s: " format "\r\n", __func__, __VA_ARGS__)
 
void RCC_GPIOClockCmd(GPIO_TypeDef* GPIOx, FunctionalState state);
void RCC_USARTClockCmd(USART_TypeDef* USARTx, FunctionalState state);
void Timer_16bit_Calc(int freq, uint16_t *period, uint16_t *prescaler);

#endif /* __COMMON__H__ */