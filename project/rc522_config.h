 /* 注： 通过此文件可以修改RFID的引脚连线    */

#define RC522_PORT GPIOB
#define RC522_PORT_RCC RCC_APB2Periph_GPIOB

#define RC522_SPI SPI2
#define RC522_SPI_RCC RCC_APB1Periph_SPI2

#define RC522_PIN_MISO GPIO_Pin_14
#define RC522_PIN_MOSI GPIO_Pin_15
#define RC522_PIN_CLK GPIO_Pin_13

#define RC522_PIN_RST GPIO_Pin_0
#define RC522_PIN_CS GPIO_Pin_1
