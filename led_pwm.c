/**
 * LED PWM ���ο����߼�
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
    ���ڵ�Դ�ж�,ֻҪ��Ӧ�ж�,ϵͳӲ���Զ����жϱ�־λ
    ����TA/TB��ʱ���ıȽ�/�����ж�,ֻҪ����TAIV/TBIV,��־λ�����Զ����;
    ���ڶ�Դ�ж�Ҫ�ֶ����־λ,����P1/P2���ж�,Ҫ�ֹ������Ӧ�ı�־,
  ����������ж���"EINT();"���ж�,���ڴ��ж�ǰû�����־,�ͻ�����ͬ���жϲ���
  Ƕ��,�����¶�ջ�������λ,�����������ж��б��������־�ٴ��жϿ���.
  */
  led_pwm_counter = led_pwm_counter == 255 ? 0 : led_pwm_counter + 1;
  
  TA0CCTL1 |= CCIE;
  __disable_interrupt();
}
