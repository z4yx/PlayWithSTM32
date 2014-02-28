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

    if (Status == SD_OK) {
        /* Read CSD/CID MSD registers */
        Status = SD_GetCardInfo( &SDCardInfo );
    }

    if (Status == SD_OK)
    {
    	unsigned long a,b;
		    
	    a = SDCardInfo.SD_csd.DeviceSize + 1;
	    b = 1 << (SDCardInfo.SD_csd.DeviceSizeMul + 2);

	    USART1_printf("SD Card: %d Blocks, Block size %d \r\n", a*b, 1<<SDCardInfo.SD_csd.RdBlockLen);
    }

    if (Status == SD_OK) {
        /* Select Card */
        Status = SD_SelectDeselect( (u32) (SDCardInfo.RCA << 16) );
    }

    if (Status == SD_OK) {
        /* set bus wide */
        Status = SD_EnableWideBusOperation( SDIO_BusWide_1b );
    }

    /* Set Device Transfer Mode to DMA */
    if (Status == SD_OK) {
        /* 任选一种都可以工作 */
        Status = SD_SetDeviceMode( SD_DMA_MODE );
        //Status = SD_SetDeviceMode( SD_POLLING_MODE );
        //Status = SD_SetDeviceMode( SD_INTERRUPT_MODE );
    }
    return ( Status );
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
    SD_Error err = SDIO_Test();
    USART1_printf(USART1, "SD_Error: %d\r\n", err);

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
