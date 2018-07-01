/**
 * LED PWM ���ο����߼�
 * by Bob Green
 */
#include "msp430.h"
#include "achieved/HAL_Board.h"
#include "led_pwm.h"
#include <stdio.h>


int led_light_level = LED_LIGHT_LEVEL_TOTAL;
int led1_light = 10;
int led2_light = 3;
int led3_light = 255;

   
void calc_light_levels (float q, int* led1, int* led2, int* led3) {
  int basis = (int)(q * 256);
  *led1 = (int)(basis / 20);
  *led2 = (int)(basis / 5);
  *led3 = (int)(basis / 1);
}


void led_pwm_timer_interrupt (void) {
  static int led_pwm_counter = 0;
  
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
}


void init_timer_output() {
  // Timer_A0
  TA0CCR0 = 100;
  TA0CCTL0 |= CCIE;
  TA0CCR1 = 5000;
  TA0CCTL1 |= CCIE | OUTMOD_7;
  // TA0CCTL1 &= ~CCIE;
  TA0CTL = TASSEL_1 + MC_2 + TACLR + ID_3;
  
  //Timer A1 & A2
  TA2CTL = TA1CTL = TASSEL_1+TACLR;
  TA2CCR0 = TA1CCR0 = 255;
  TA2CCTL1 = TA1CCTL1 = OUTMOD_7;
  TA2CCR1 = TA1CCR1 = 10;
  TA2CCTL2 = TA1CCTL2=OUTMOD_7;
  TA2CCR2 = TA1CCR2 = 128;
  TA1CTL |= MC_1; TA2CTL |= MC_1;
  P2DIR |= BIT4 | BIT1 | BIT0; // Switch Port Functions
  P2SEL |= BIT4 | BIT1 | BIT0;
  
  update_led_light();
}

void update_led_light() {
  TA1CCR1 = led1_light; // P2.0
  TA1CCR2 = led2_light; // P2.1
  TA2CCR1 = led3_light; // P2.4
}





