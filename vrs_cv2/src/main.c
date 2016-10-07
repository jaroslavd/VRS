/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Environment : Atollic TrueSTUDIO/STM32
**                STMicroelectronics STM32Lxx Standard Peripherals Library
**
**  Distribution: The file is distributed "as is", without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. This file may only be built (assembled or compiled and linked)
**  using the Atollic TrueSTUDIO(R) product. The use of this file together
**  with other tools than Atollic TrueSTUDIO(R) is not permitted.
**
*****************************************************************************
*/

/* Includes */
#include <stddef.h>
#include "stm32l1xx.h"


/* Private typedef */
/* Private define  */
/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */


/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
  int i = 0;

  /**
  *  IMPORTANT NOTE!
  *  See the <system_*.c> file and how/if the SystemInit() function updates 
  *  SCB->VTOR register. Sometimes the symbol VECT_TAB_SRAM needs to be defined 
  *  when building the project if code has been located to RAM and interrupts 
  *  are used. Otherwise the interrupt table located in flash will be used.
  *  E.g.  SCB->VTOR = 0x20000000;  
  */

  /**
  *  At this stage the microcontroller clock setting is already configured,
  *  this is done through SystemInit() function which is called from startup
  *  file (startup_stm32l1xx_hd.s) before to branch to application main.
  *  To reconfigure the default setting of SystemInit() function, refer to
  *  system_stm32l1xx.c file
  */


  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  //GPIOA
  GPIOA->MODER |= 1<<10; //output
  GPIOA->OSPEEDR |= 3<<10;
  GPIOA->PUPDR |= 1<<10;
  if(GPIOA->OTYPER == 1<<5)
  {
	  GPIOA->OTYPER ^= 1<<5; // //output push-pull
  }


  //GPIOC
  /*if(GPIOC->MODER == 1<<27) //INPUT
  {
	  GPIOC->MODER ^= 1<<27;
  }
  if(GPIOC->MODER == 1<<26)//INPUT
  {
	  GPIOC->MODER ^= 1<<26;
  }

  if(GPIOC->OTYPER == 1<<13)//push-pull
  {
	  GPIOC->OTYPER ^= 1<<13;
  }

  if(GPIOC->PUPDR == 1<<27) //NO PULL
  {
	  GPIOC->PUPDR ^= 1<<27;
  }
  if(GPIOC->PUPDR == 1<<26)//NO PULL
  {
	  GPIOC->PUPDR ^= 1<<26;
  }*/
    if((GPIOC->MODER & 1<<27) == 1) //INPUT
    {
  	  GPIOC->MODER ^= 1<<27;
    }
    if((GPIOC->MODER & 1<<26) == 1)//INPUT
    {
  	  GPIOC->MODER ^= 1<<26;
    }

    if((GPIOC->OTYPER & 1<<13) == 1)//push-pull
    {
  	  GPIOC->OTYPER ^= 1<<13;
    }

    if((GPIOC->PUPDR & 1<<27) == 1) //NO PULL
    {
  	  GPIOC->PUPDR ^= 1<<27;
    }
    if((GPIOC->PUPDR & 1<<26) == 1)//NO PULL
    {
  	  GPIOC->PUPDR ^= 1<<26;
    }

  uint8_t BUTTON = 1;
  uint16_t blikanie = 0;
  uint16_t zatlacene_tlacidlo = 0;
  while (1)
  {
	  /*GPIOA->ODR |= 1<<5; //turn LED2 on

	  GPIOA->ODR ^= 1<<5; // turn LED2 off

	  GPIOA->BSRRL |= 1<<5; // turn LED2 on

	  GPIOA->BSRRH |= 1<<5;  // turn LED2 off

	  GPIOA->ODR ^= 1<<5; // toggle LED2

	  GPIOA->ODR ^= 1<<5; // toggle LED2

	  GPIOA->ODR ^= 1<<5; // toggle LED2

	  GPIOA->ODR ^= 1<<5; // toggle LED2

	i++;
*/

	if ((GPIOC->IDR & 1<<13) != 0)
	  {
			BUTTON = 0;
	  }
  else
	  {
	  	  BUTTON = 1;
	  }

	//Uloha 3
	/*GPIOA->ODR ^= 1<<5; // toggle LED2
	for(blikanie=0;blikanie<50000;blikanie++)
	{
	}
	GPIOA->ODR ^= 1<<5; // toggle LED2

	for(blikanie=0;blikanie<50000;blikanie++)
	{
	}*/

	/*if(BUTTON == 0)
	{
		GPIOA->BSRRH |= 1<<5;  // turn LED2 off
	}
	else
	{
		GPIOA->BSRRL |= 1<<5; // turn LED2 on
	}*/

	if(BUTTON == 1)
	{
		zatlacene_tlacidlo++;
		if(zatlacene_tlacidlo == 1000)
		{
			GPIOA->ODR ^= 1<<5; // toggle LED2

		}
	}
	else
	{
		zatlacene_tlacidlo = 0;
	}

  }
  return 0;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}
