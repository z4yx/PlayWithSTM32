#include <stdint.h>
#include "stm32f10x.h"
#include "usart1.h"
#include "led.h"

#include "rc522.h"
#include "rc522_config.h"      //定义RC522的引脚


/*******************************************************************************
* 描  述  :  初始化RC522，开启天线并设置RC522的工作方式为 ISO14443_A
* 输  入  :  无
* 输  出  :  无
* 返  回  :  无
*******************************************************************************/
void InitRc522(void)
{
    PcdReset();
    PcdAntennaOff();
    PcdAntennaOn();
    M500PcdConfigISOType( 'A' );
}

/*******************************************************************************
* 描  述  :  初始化与RC522的接口
* 输  入  :  无
* 输  出  :  无
* 返  回  :  无
*******************************************************************************/
void InitSPI(void)
{
    SPI_InitTypeDef SPI_InitStructure;

    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RC522_PORT_RCC, ENABLE);

    //RFID引脚设置
    GPIO_InitStructure.GPIO_Pin = RC522_PIN_MISO;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(RC522_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = RC522_PIN_MOSI | RC522_PIN_CLK;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(RC522_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = RC522_PIN_RST | RC522_PIN_CS;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(RC522_PORT, &GPIO_InitStructure);

    GPIO_SetBits(RC522_PORT, RC522_PIN_RST);
    GPIO_SetBits(RC522_PORT, RC522_PIN_CS);

    RCC_APB1PeriphClockCmd(RC522_SPI_RCC, ENABLE);
    SPI_StructInit(&SPI_InitStructure);
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_BaudRatePrescaler =
        SPI_BaudRatePrescaler_128;
    SPI_Init(RC522_SPI, &SPI_InitStructure);
    SPI_Cmd(RC522_SPI, ENABLE);
}

void RFID_Config(void)
{
    InitSPI();
    InitRc522();
}

void RFID_Test(void)
{
    unsigned char RevBuffer[16], SelectedCard[4];
    unsigned char PassWd[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    int numBlock;
    int i;
    char status;

    PcdReset();
    status = PcdRequest(PICC_REQIDL, RevBuffer); //寻天线区内未进入休眠状态的卡，返回卡片类型 2字节
    if (status != MI_OK) 
        return;

    LED_Board(LED_OFF);

    status = PcdAnticoll(SelectedCard); //防冲撞，返回卡的序列号 4字节
    if (status != MI_OK)
        return;

    USART1_printf(USART1, "Card Found, Type: %d %d " "Selected: %d %d %d %d\r\n",

        RevBuffer[0],
        RevBuffer[1],

        SelectedCard[0],
        SelectedCard[1],
        SelectedCard[2],
        SelectedCard[3]
        );

    status = PcdSelect(SelectedCard); //选卡
    if (status != MI_OK)
        return;

    for(numBlock = 0; numBlock<64; numBlock++){
        USART1_printf(USART1, "Block %d: ", numBlock);

        status = PcdAuthState(PICC_AUTHENT1A, numBlock, PassWd, SelectedCard); //验证A密匙
        if (status != MI_OK)  {
            USART1_printf(USART1, "Auth Failed!\r\n");
            continue;
        }

        status = PcdRead(numBlock, RevBuffer);
        if (status != MI_OK)  
            return;

        for (i = 0; i < 16; i++)
        {
            USART1_printf(USART1, "%d ", RevBuffer[i]);
        }
        USART1_printf(USART1, "\r\n");
    }

    USART1_printf(USART1, "\r\n");

    PcdHalt();


    // else if(oprationcard==WRITECARD)//写卡
    // {
    //   oprationcard=0;
    //   status=PcdAuthState(PICC_AUTHENT1A,KuaiN,PassWd,MLastSelectedSnr);//
    //   if(status!=MI_OK)
    //   {
    //     return;
    //   }
    //   status=PcdWrite(KuaiN,&WriteData[0]);
    //   if(status!=MI_OK)
    //   {
    //     return;
    //   }
    //  PcdHalt();
    //  uart_count=0;
    //  CLR_BEEP;
    // }
    // else if(oprationcard==SENDID)//发送卡号
    // {
    //   oprationcard=0;
    //   for(i=0;i<4;i++)
    //   {
    //     cardID[i]=MLastSelectedSnr[i];
    //     UART1_SendByte(MLastSelectedSnr[i]);
    //   }
    // uart_count=0;
    // CLR_BEEP;
    // }

    LED_Board(LED_ON);
}
