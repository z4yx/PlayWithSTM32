#ifndef _USB_COMMON_H__
#define _USB_COMMON_H__
#include "usb_type.h"

#define USB_DISCONNECT                      GPIOB  
#define USB_DISCONNECT_PIN                  GPIO_Pin_14
#define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOB

void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config (FunctionalState NewState);

void USBCommon_Init(void);
#endif
