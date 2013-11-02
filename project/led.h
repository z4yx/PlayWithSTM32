
#include "stm32f10x.h"

#define LED_ON  1
#define LED_OFF 0

#define LED_Board(a) \
do {\
  if (a) \
    GPIO_SetBits(GPIOD,GPIO_Pin_2); \
  else \
    GPIO_ResetBits(GPIOD,GPIO_Pin_2); \
}while(0)


void LED_Config();
