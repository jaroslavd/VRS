/*
 * vrs_cv5.c
 *
 *  Created on: Oct 17, 2016
 *      Author: Jaroslav Duraj
 */

#include "vrs_cv5.h"
#include "stm32l1xx.h"
#include "stdio.h"

int i = 0;
int j = 7;
char Buffer_Data_to_send[7] = "";
uint16_t received_data = 0;
uint16_t ADC1_value = 0;
float voltage = 0;
uint8_t sending_format = 0;

void ADC1_IRQHandler(void)
{
	if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != RESET)
	{
		ADC1_value = ADC_GetConversionValue(ADC1);
		ADC_ClearFlag(ADC1, ADC_FLAG_EOC);

	}
}

void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
	{
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		received_data = USART_ReceiveData(USART1);

        if(received_data == (uint16_t)'m')
        {
        	sending_format^=1;
        }
	}

	if((USART_GetFlagStatus(USART1, USART_FLAG_TC) != RESET) )
	{
		send_data();

		USART_ClearFlag(USART1, USART_FLAG_TC);
	}
}

void UART1_init(void)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */       //turning on the needed peripherals
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  //choosing peripherals for selected pins
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

   /* Configure USART Tx and Rx pins */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //usart configuration
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);

   //configuring interrupts
  /* NVIC configuration */
  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  //choosing which event should cause interrupt

  USART_ITConfig(USART1, USART_IT_RXNE | USART_IT_TC, ENABLE);
  /* Enable USART */
  USART_Cmd(USART1, ENABLE);
}

void adc_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	/* Configure ADCx Channel 2 as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* Enable the HSI oscillator */
	RCC_HSICmd(ENABLE);
	/* Check that HSI oscillator is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
	/* Enable ADC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);
	/* ADC1 configuration */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* Enable interrupt */
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	ADC_ITConfig(ADC1, ADC_IT_OVR, ENABLE);

	/* ADCx regular channel8 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_16Cycles);
	/* Enable the ADC */
	ADC_Cmd(ADC1, ENABLE);
	/* Wait until the ADC1 is ready */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
	{
	}
	/* Start ADC Software Conversion */
	ADC_SoftwareStartConv(ADC1);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void send_data(void)
{
		if(sending_format)
		{
			if(j>=6)
			{
				//buffer with data was sent
				fill_buffer();
				j=0;
			}
		}
		else
		{
			if(j>=7)
			{
				//buffer with data was sent
				fill_buffer();
				j=0;
			}
		}
		USART_SendData(USART1, (uint8_t)Buffer_Data_to_send[j]);

		j+=1;

}

void fill_buffer(void)
{
	if(sending_format)
	{
		sprintf(Buffer_Data_to_send,"%d\r\n",ADC1_value );
	}
	else
	{
		voltage = ADC1_value/4095.0*3.3;
		sprintf(Buffer_Data_to_send,"%.2fV\r\n",voltage);
	}
}
