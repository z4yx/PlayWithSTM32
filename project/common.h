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

#define LOG_ERR(M, ...) USART1_printf(USART1, "[ERROR] (%s:%d) " M "\r\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_WARN(M, ...) USART1_printf(USART1, "[WARN] (%s:%d) " M "\r\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(M, ...) USART1_printf(USART1, "[INFO] (%s:%d) " M "\r\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_DBG(M, ...) USART1_printf(USART1, "[DBG] (%s:%d) " M "\r\n", __FILE__, __LINE__, ##__VA_ARGS__)
 
void RCC_GPIOClockCmd(GPIO_TypeDef* GPIOx, FunctionalState state);
void RCC_USARTClockCmd(USART_TypeDef* USARTx, FunctionalState state);
void Timer_16bit_Calc(int freq, uint16_t *period, uint16_t *prescaler);

#endif /* __COMMON__H__ */