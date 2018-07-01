/**
 * LED PWM ²¨ÐÎ¿ØÖÆÂß¼­
 * by Bob Green
 */
#ifndef _INCLUDE_LED_PWM_H
#define _INCLUDE_LED_PWM_H

#define LED_LIGHT_LEVEL_TOTAL (8)

extern int led_light_level;
extern int led1_light;
extern int led2_light;
extern int led3_light;

void calc_light_levels (float q, int* led1, int* led2, int* led3);

void led_pwm_timer_interrupt (void);
void init_timer_output();
void update_led_light();


#endif