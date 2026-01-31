#include "main.h"
#include <stdint.h>
/*
B1 user:  port PC13 (pin 2) 
LD2 user: port PA5 (pin 21)
*/
// RCC enable ports
#define GPIOA_EN (1 << 0)
#define GPIOC_EN (1 << 2)
// Pin mode
// PA5 reset
#define PA5_R (3 << (2 * 5))
// PA5 enable
#define PA5_EN (1 << (2 * 5))
// PC13 MODER reset
#define PC13_R (3 << (2 * 13))
// PC13 enable MODER 
#define PC13_EN (0 << (2 * 13))
// PC13 enable pull-up
#define PC13_EN_P (1 << (2 * 13))

// GPIO register addresses + mode offset
// GPIOA addresses: 0x4002 0000 - 0x4002 03FF
// ODR register offset for all: 0x14
// MODER register offset: 0x00
#define GPIOA_MODER_REG_ADDR (volatile uint32_t *)(0x40020000UL + 0x00UL)
#define GPIOA_ODR_REG_ADDR (volatile uint32_t *)(0x40020000UL + 0x14UL)
// GPIOC addresses: 0x4002 0800 - 0x4002 0BFF
#define GPIOx_MODER_REG_ADDR (volatile uint32_t *)(0x40020800UL + 0x00UL)
// IDR register offset: 0x10
#define GPIOC_IDR_REG_ADDR (volatile uint32_t *)(0x40020800UL + 0x10UL)
// PUPDR register offset: 0x0C
#define GPIOC_PUPDR_REG_ADDR (volatile uint32_t *)(0x40020800UL + 0x0CUL)
/*RCC addresses: 0x4002 3800 - 0x4002 3BFF
AHB1ENR register offset: 0x30*/
#define RCC_AHB1ENR_ADDRS (volatile uint32_t *)(0x40023800UL + 0x30UL)


int main(void) {
  // RCC on GPIOA and GPIOC
  *RCC_AHB1ENR_ADDRS |= GPIOA_EN | GPIOC_EN;
  // PA5 reset and set pin mode "output" in register GPIOA_MODER
  *GPIOA_MODER_REG_ADDR &= ~PA5_R;
  *GPIOA_MODER_REG_ADDR |= PA5_EN;
  // PC13 reset and set pin mode "input" in register GPIOC_MODER
  *GPIOx_MODER_REG_ADDR &= ~PC13_R;
  *GPIOx_MODER_REG_ADDR |= PC13_EN;
  // PC13 reset and set pull-up mode in register GPIOC_PUPDR
  *GPIOC_PUPDR_REG_ADDR &= ~PC13_R; 
  *GPIOC_PUPDR_REG_ADDR |= PC13_EN_P;

  while (1) {
    // PC13 button is pressed
    if(!(*GPIOC_IDR_REG_ADDR & (1 << 13))) {
      // debouncing delay
      for(volatile uint32_t i = 0; i < 100000; i++);
      // double-check is pressed 
      if(!(*GPIOC_IDR_REG_ADDR & (1 << 13))) {
        // led on
        *GPIOA_ODR_REG_ADDR |= (1 << 5);
      } 
    }
    else {
        // led off
        *GPIOA_ODR_REG_ADDR &= ~(1 << 5);
    }
  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }

}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */