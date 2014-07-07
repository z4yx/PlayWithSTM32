/**
    ******************************************************************************
    * @file    GPIO/IOToggle/main.c
    * @author  MCD Application Team
    * @version V3.5.0
    * @date    08-April-2011
    * @brief   Main program body.
    ******************************************************************************
    * @attention
    *
    * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
    * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
    * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
    * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
    * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
    * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
    *
    * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
    ******************************************************************************
    */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "led.h"
#include "usart1.h"
#include "systick.h"
#include "sdio.h"
#include "ff.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
    * @{
    */

/** @addtogroup GPIO_IOToggle
    * @{
    */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


SD_Error SDIO_Test(void)
{
    SD_Error Status = SD_OK;
    uint32_t DeviceSizeMul = 0, NumberOfBlocks = 0;
    uint32_t Mass_Block_Count = 0, Mass_Block_Size = 0;
    SD_CardInfo SDCardInfo;

    /* SD Init */
    Status = SD_Init();

    if(Status != SD_OK)
        return Status;

    /* Read CSD/CID MSD registers */
    Status = SD_GetCardInfo( &SDCardInfo );

    if(Status != SD_OK)
        return Status;

    DeviceSizeMul = (SDCardInfo.SD_csd.DeviceSizeMul + 2);

    if(SDCardInfo.CardType == SDIO_HIGH_CAPACITY_SD_CARD)
    {
        Mass_Block_Count = (SDCardInfo.SD_csd.DeviceSize + 1) * 1024;
    }
    else
    {
        NumberOfBlocks  = ((1 << (SDCardInfo.SD_csd.RdBlockLen)) / 512);
        Mass_Block_Count = ((SDCardInfo.SD_csd.DeviceSize + 1) * (1 << DeviceSizeMul) << (NumberOfBlocks/2));
    }
    Mass_Block_Size  = 512;
    
    USART1_printf(USART1, "CardCapacity: %dK, %d Blocks, Block Size %d \r\n",
        Mass_Block_Size*Mass_Block_Count/1024,
        Mass_Block_Count,
        Mass_Block_Size);

    USART1_printf(USART1, "SD Card Init OK!\r\n");


    return ( Status );
}

/*
注意：每一次调用f_read读取内容不能超过一个sector(512bytes)，否则fatfs会使用direct transfer方式，
直接使用传入的缓冲区，如果传入的缓冲区非4字节对齐，SDIO中的DMA就会出错。
f_read的官方说明：
The memory address specified by buff is not that always aligned to word boundary 
because the type of argument is defined as BYTE*. The misaligned read/write request 
can occure at direct transfer. If the bus architecture, especially DMA controller, 
does not allow misaligned memory access, it should be solved in this function. 
There are some workarounds described below to avoid this issue.

** Convert word transfer to byte transfer in this function. - Recommended.
** For f_read(), avoid long read request that includes a whole of sector. - Direct transfer will never occure.
** For f_read(fp, buff, btr, &br), make sure that (((UINT)buff & 3) == (f_tell(fp) & 3)) is true. - Word aligned direct transfer is guaranteed.

Generally, a multiple sector transfer request must not be split into single sector transactions to the storage device, or you will not get good read throughput.
*/
static void Fatfs_Test(void)
{
    FATFS fs;
    FIL fw,fr;
    UINT cnt;
    BYTE buf[] = "hello world";

    if(FR_OK != f_mount(0,&fs)){
        USART1_printf(USART1, "Failed to mount SD card!\r\n");
        return;
    }
    if(FR_OK != f_open(&fw, "demo.txt", FA_WRITE | FA_OPEN_ALWAYS)){
        USART1_printf(USART1, "Failed to open file for writing!\r\n");
        return;
    }

    f_write(&fw, buf, sizeof(buf), &cnt);

    USART1_printf(USART1, "%d Bytes Written\r\n", (int)cnt);

    f_close(&fw);

    if(FR_OK != f_open(&fr, "demo.txt", FA_OPEN_EXISTING | FA_READ)){
        USART1_printf(USART1, "Failed to open file for reading!\r\n");
        return;
    }

    f_read(&fr, buf, sizeof(buf), &cnt);

    USART1_printf(USART1, "%d Bytes Read\r\n", (int)cnt);
    
    buf[cnt] = 0;

    USART1_printf(USART1, "File content: %s\r\n", buf);

    f_close(&fr);

    f_mount(0, 0);
}

/**
    * @brief  Main program.
    * @param  None
    * @retval None
    */
int main(void)
{
    /*!< At this stage the microcontroller clock setting is already configured,
             this is done through SystemInit() function which is called from startup
             file (startup_stm32f10x_xx.s) before to branch to application main.
             To reconfigure the default setting of SystemInit() function, refer to
             system_stm32f10x.c file
    */

    LED_Config();
    USART1_Config();
    SysTick_Init();

    Delay_ms(500);

    SD_Error err = SDIO_Test();
    if(err != SD_OK)
        USART1_printf(USART1, "SD_Error: %d\r\n", err);
    else {
        Fatfs_Test();
    }

    while (1) {
        LED_Board(LED_ON);
        Delay_ms(1000);
        LED_Board(LED_OFF);
        Delay_ms(1000);

        USART1_printf(USART1, "hello\r\n");
    }
}

#ifdef  USE_FULL_ASSERT

/**
    * @brief  Reports the name of the source file and the source line number
    *         where the assert_param error has occurred.
    * @param  file: pointer to the source file name
    * @param  line: assert_param error line source number
    * @retval None
    */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
         ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1) {
    }
}

#endif

/**
    * @}
    */

/**
    * @}
    */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
