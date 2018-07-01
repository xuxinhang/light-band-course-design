#include "achieved/HAL_Board.h"
#include "msp430.h" // #include "io430.h"
#include "achieved/HAL_Buttons.h"

#include "led_pwm.h"
#include "command_parser.h"
#include "bluetooth_port.h"
#include "light_control.h"

/* Message Handler */
char rx_buffer[] = "0,255,255,0,;1000,12,0,255,;1999,255,255,0,;";


int main( void )
{
  
  __enable_interrupt();

  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  // Basic GPIO initialization
  Board_init();
  
  P1DIR |=  0xF;
  
  // Set Vcore to accomodate for max. allowed system speed
  // SetVCore(3);

  // Board_ledOn(LED1+LED2);

  // P1DIR &= 0xff;
  P1OUT |= 0x01;
  P1OUT |= 0x0001u;
  
  // P8DIR &= 0xff;
  P8OUT |= 0x01;
  
  // Deal with switch interrupt
  Buttons_init(BUTTON_S1);
  Buttons_interruptEnable(BUTTON_S1);
  
  /*while (SFRIFG1 & OFIFG){  
  // Set clock signal frequency
  // UCSCTL4 |= SELA0; // ACLK source = VLOCLK
  // UCSCTL4 &= ~SELA1;
  // UCSCTL4 &= ~SELA2;
  // UCSCTL4 = UCSCTL4&(~(SELS_7|SELM_7))|SELS_2|SELM_2;
  // UCSCTL4 &= (~(SELA_7))|SELA_1;
    // 清除三类时钟标志位  这里需要清除三种标志位，因为任何一种 标志位都会将OFIFG置位  
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
    SFRIFG1 &= ~OFIFG;  // 清除时钟错误标志位  
  }*/
  
  init_timer_output();
 
  // Message Handler
  light_control_abort();
  light_control_update(rx_buffer);
  
  // Connect to Bluetooth moudel
  bluetooth_port_init();
  
  return 0;
}


#pragma vector = PORT1_VECTOR
__interrupt void port1_interrupt (void) {
  // Board_ledOff(LED1);
  if (P1IFG & BIT7) {
    Board_ledToggle(LED1);
    P1IFG &= ~BIT7;
    // Update LED Light Value
    led_light_level = led_light_level == 1 ? (LED_LIGHT_LEVEL_TOTAL) : (led_light_level - 1);
    led3_light = (256-1) * led_light_level / LED_LIGHT_LEVEL_TOTAL;
    led2_light = led3_light / 4;
    led1_light = led2_light / 5;
    update_led_light();
  }
  __disable_interrupt();
}


int led_pwm_counter = 0;


int dynamic_led_light_level = 3;


// 这里的TimerA0终端处理程序就算没用写个空函数也要留着！
#pragma vector = TIMER0_A0_VECTOR
__interrupt void timer1_A0_interrupt() {
  TA0CCR0 += 2000 / 8;
  
  // 更新LED亮度变量
  interrupt_update_led_lights();

  /*led1_light = 1;
  led2_light = 1;
  led3_light = 1;
  calc_light_levels(dynamic_led_light_level / 3.0, &led1_light, &led2_light, &led3_light);
  dynamic_led_light_level = dynamic_led_light_level == 0 ? 3 : dynamic_led_light_level - 1;*/
  
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void timer0_A1_interrupt (void) {
  TA0CCTL1 &= (~CCIE);
  __enable_interrupt();

  led_pwm_timer_interrupt();
  // TA0CCR1 += 7;
  
  TA0CCTL1 |= CCIE;
  
  __disable_interrupt();
}


/*
0,0,0,0,;400,0,0,0,;800,0,0,20,;1200,0,20,255,;1600,0,255,255,;2000,0,255,20,;2400,0,20,0,;2800,0,0,0,;
*/




