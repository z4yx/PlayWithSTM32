 /* 注： 通过此文件可以修改RFID的引脚连线    */

#define RC522_PORT GPIOB
#define RC522_PORT_RCC RCC_APB2Periph_GPIOB

#define RC522_PIN_MISO GPIO_Pin_14
#define RC522_PIN_MOSI GPIO_Pin_15
#define RC522_PIN_CLK GPIO_Pin_13

#define RC522_PIN_RST GPIO_Pin_0
#define RC522_PIN_CS GPIO_Pin_1

#define TDIN_SET(a) \
do {\
  if (a) \
    GPIO_SetBits(RC522_PORT, RC522_PIN_MOSI); \
  else \
    GPIO_ResetBits(RC522_PORT, RC522_PIN_MOSI); \
}while(0)
#define TCLK_SET(a) \
do {\
  if (a) \
    GPIO_SetBits(RC522_PORT, RC522_PIN_CLK); \
  else \
    GPIO_ResetBits(RC522_PORT, RC522_PIN_CLK); \
}while(0)
#define TCS_SET(a) \
do {\
  if (a) \
    GPIO_SetBits(RC522_PORT, RC522_PIN_CS); \
  else \
    GPIO_ResetBits(RC522_PORT, RC522_PIN_CS); \
}while(0)

#define CLR_RC522RST GPIO_ResetBits(RC522_PORT, RC522_PIN_RST)
#define SET_RC522RST GPIO_SetBits(RC522_PORT, RC522_PIN_RST)

#define DOUT GPIO_ReadInputDataBit(RC522_PORT, RC522_PIN_MISO)
// #define RST  1 			   //PB1
		   
// #define  CLR_RC522RST  GPIOB->ODR=(GPIOB->ODR&~RST)|(0 ? RST:0)
// #define  SET_RC522RST  GPIOB->ODR=(GPIOB->ODR&~RST)|(1 ? RST:0)

// #define DOUT GPIOC->IDR&(1<<0X06)//PB6数据输入
			 
// #define TDIN  (1<<0X07)// PC7
// #define TCLK  (1<<0X08) // PC5
// #define TCS   (1<<1)  // PB2		    
// #define TDIN_SET(x) GPIOC->ODR=(GPIOC->ODR&~TDIN)|(x ? TDIN:0)
// #define TCLK_SET(x) GPIOC->ODR=(GPIOC->ODR&~TCLK)|(x ? TCLK:0)													    
// #define TCS_SET(x)  GPIOB->ODR=(GPIOB->ODR&~TCS)|(x ? TCS:0) 