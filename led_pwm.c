/**
 * LED PWM 波形控制逻辑
 * by Bob Green
 */


#include "msp430.h"
#include "achieved/HAL_Board.h"
#include "led_pwm.h"


int led_light_level = LED_LIGHT_LEVEL_TOTAL;
int led1_light = 10;
int led2_light = 70;
int led3_light = 255;

void calc_light_levels (float q, int* led1, int* led2, int* led3) {
  int basis = (int)(q * 256);
  *led1 = (int)(basis / 20);
  *led2 = (int)(basis / 5);
  *led3 = (int)(basis / 1);
}


#pragma vector = TIMER0_A1_VECTOR
__interrupt void timer0_A1_interrupt (void) {
  static int led_pwm_counter = 0;
  
  TA0CCTL1 &= (~CCIE);
  __enable_interrupt();

  if (led_pwm_counter == 0) {
    Board_ledOn(LED1);
    Board_ledOn(LED2);
    Board_ledOn(LED3);
  }
  
  if (led_pwm_counter == led1_light) {
    Board_ledOff(LED1);
  }
  
  if (led_pwm_counter == led2_light) {
    Board_ledOff(LED2);
  }
  
  if (led_pwm_counter == led3_light) {
    Board_ledOff(LED3);
  }

  /*
    对于单源中断,只要响应中断,系统硬件自动清中断标志位
    对于TA/TB定时器的比较/捕获中断,只要访问TAIV/TBIV,标志位倍被自动清除;
    对于多源中断要手动清标志位,比如P1/P2口中断,要手工清除相应的标志,
  如果在这种中断用"EINT();"开中断,而在打开中断前没有清标志,就会有相同的中断不断
  嵌入,而导致堆栈溢出引起复位,所以在这类中断中必须先清标志再打开中断开关.
  */
  led_pwm_counter = led_pwm_counter == 255 ? 0 : led_pwm_counter + 1;
  
  TA0CCTL1 |= CCIE;
  __disable_interrupt();
}
