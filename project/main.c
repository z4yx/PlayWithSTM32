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
    SD_CardInfo SDCardInfo;

    /* SD Init */
    Status = SD_Init();

    if(Status != SD_OK)
        return Status;

    /* Read CSD/CID MSD registers */
    Status = SD_GetCardInfo( &SDCardInfo );

    if(Status != SD_OK)
        return Status;

    USART1_printf(USART1, "CardCapacity: %dK, %d Blocks, Block Size %d \r\n",
    	SDCardInfo.CardCapacity/1024,
    	SDCardInfo.CardCapacity/SDCardInfo.CardBlockSize,
    	SDCardInfo.CardBlockSize);

    Status = SD_SelectDeselect( (u32) (SDCardInfo.RCA << 16) );
    if(Status != SD_OK)
        return Status;

    Status = SD_EnableWideBusOperation( SDIO_BusWide_1b );
    if(Status != SD_OK)
        return Status;

    Status = SD_SetDeviceMode(SD_DMA_MODE);
    if(Status != SD_OK)
        return Status;

    return ( Status );
}

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

    SD_NVIC_Configuration();

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
