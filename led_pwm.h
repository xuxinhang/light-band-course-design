/**
 * LED PWM ²¨ÐÎ¿ØÖÆÂß¼­
 * by Bob Green
 */
#ifndef _INCLUDE_LED_PWM_H
#define _INCLUDE_LED_PWM_H


#define LED_LIGHT_LEVEL_TOTAL (8)

void calc_light_levels (float q, int* led1, int* led2, int* led3);

#pragma vector = TIMER0_A1_VECTOR
__interrupt void timer0_A1_interrupt (void);


#endif