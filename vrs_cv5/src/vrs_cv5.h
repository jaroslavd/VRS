/*
 * vrs_cv5.h
 *
 *  Created on: Oct 17, 2016
 *      Author: Jaroslav Duraj
 */

#ifndef VRS_CV5_H_
#define VRS_CV5_H_

void adc_init(void);
void UART1_init(void);
void USART1_IRQHandler(void);
void ADC1_IRQHandler(void);
void send_data(void);
void fill_buffer(void);

#endif /* VRS_CV5_H_ */
