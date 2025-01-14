
/* F1 device-specific code */

#include "can.h"
#include "device.h"
#include "hal_include.h"
#include "timing_tsmaster.h"


/*
TOSUN TSmaster support

Calculator http://www.bittiming.can-wiki.info/
calculates the value Seg 1 = Prop_Seg+Phase_Seg1 -> phase_seg1 - 1
*/
static const struct gs_device_bittiming bittimingMap36MHz[] = {
    //75%

    // 1000k
    { .prop_seg = 1u, .phase_seg1 = 12u, .phase_seg2 = 4u, .sjw = 1u, .brp = 2u },
    // 800k
    { .prop_seg = 1u, .phase_seg1 =  9u, .phase_seg2 = 4u, .sjw = 1u, .brp = 3u },
    // 500k
	{ .prop_seg = 1u, .phase_seg1 = 12u, .phase_seg2 = 4u, .sjw = 1u, .brp = 4u },
    // 250k
    { .prop_seg = 1u, .phase_seg1 = 10u, .phase_seg2 = 4u, .sjw = 1u, .brp = 9u },

    //87.5%
    //1000k
    { .prop_seg = 1u, .phase_seg1 = 14u, .phase_seg2 = 2u, .sjw = 1u, .brp = 2u },
    //800k
    { .prop_seg = 1u, .phase_seg1 = 11u, .phase_seg2 = 2u, .sjw = 1u, .brp = 3u },
    //500k
    { .prop_seg = 1u, .phase_seg1 = 14u, .phase_seg2 = 2u, .sjw = 1u, .brp = 4u },
    //250k
    { .prop_seg = 1u, .phase_seg1 = 12u, .phase_seg2 = 2u, .sjw = 1u, .brp = 9u }

};


void device_can_init(can_data_t *channel, CAN_TypeDef *instance) {
	__HAL_RCC_CAN1_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct = {0};

    // CAN port
    GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_CAN1_2();

	channel->instance   = instance;
	channel->brp        = 4;
	channel->sjw        = 1;
	channel->phase_seg1 = 15;
	channel->phase_seg2 = 2;
	return;
}

void device_sysclock_config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
  	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  	RCC_OscInitStruct.Prediv1Source = RCC_PREDIV1_SOURCE_HSE;
  	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	RCC_OscInitStruct.PLL2.PLL2State = RCC_PLL_NONE;
  	HAL_RCC_OscConfig(&RCC_OscInitStruct);


  	/** Initializes the CPU, AHB and APB buses clocks
  	*/
  	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
        	                      |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV3;
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/*
TOSUN TSmaster support
*/
void USBD_GS_CAN_BittimingRemapper(const struct gs_device_bittiming **timing)
{
    for (uint8_t i = 0U; i < sizeof(bittimingMap48MHz); i++)
    {
        struct gs_device_bittiming bittiming48 = bittimingMap48MHz[i];
        if (bittiming48.prop_seg == (*timing)->prop_seg &&
              bittiming48.phase_seg1 == (*timing)->phase_seg1 &&
              bittiming48.phase_seg2 == (*timing)->phase_seg2 &&
              bittiming48.sjw == (*timing)->sjw &&
              bittiming48.brp == (*timing)->brp)
              {
                *timing = &bittimingMap36MHz[i];
                return;
              }
    }
}