#include "achieved/HAL_Board.h"
#include "msp430.h" // #include "io430.h"
#include "HAL_Buttons.h"

#include "led_pwm.h"
#include "command_parser.h"


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
  
  // Timer_A
  TA0CCR0 = 10000;
  TA0CCTL0 = CCIE;
  TA0CCR1 = 10000 - 1;
  TA0CCTL1 = CCIE;
  TA0CTL = TASSEL_1 + MC_1 + TACLR + ID_3;

  return 0;
}


/*
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
  }
  __disable_interrupt();
}
*/


int led_pwm_counter = 0;


int dynamic_led_light_level = 3;

#pragma vector = TIMER0_A0_VECTOR
__interrupt void timer1_A0_interrupt() {
  /*led1_light = 1;
  led2_light = 1;
  led3_light = 1;
  calc_light_levels(dynamic_led_light_level / 3.0, &led1_light, &led2_light, &led3_light);
  dynamic_led_light_level = dynamic_led_light_level == 0 ? 3 : dynamic_led_light_level - 1;
  */
}






