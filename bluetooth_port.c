#include "bluetooth_port.h"
#include <msp430.h>
#include "light_control.h"
#include "led_pwm.h"
#include "command_parser.h"

char command_string[128] = "";
char UCA1_RX_BUFFER[128];
char test_command[] = "0,7,128,0,;100,6,255,128,;@";
unsigned char *pp = (unsigned char *)UCA1RXBUF_;


int bluetooth_port_init(void)
{
  P4SEL = BIT4+BIT5;                        // P3.4,5 = USCI_A0 TXD/RXD
  UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
  UCA1CTL1 |= UCSSEL_2;                     // SMCLK
  UCA1BR0 = 6;                              // 1MHz 9600 (see User's Guide)
  UCA1BR1 = 0;                              // 1MHz 9600
  UCA1MCTL = UCBRS_0 + UCBRF_13 + UCOS16;   // Modln UCBRSx=0, UCBRFx=0,
                                            // over sampling
  UCA1CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

  // UCA1TXBUF = 0x00;

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
  __no_operation();                         // For debugger
  
  return 0;
}


void test_rewrite_rx_buffer (void) {
  static char* test_string_pointer = test_command;
  
  *pp = *test_string_pointer;
  test_string_pointer++;
  if (*test_string_pointer == 0) {
    test_string_pointer = test_command;
  }
  // USCI_A1_ISR();
  // UCA1IFG |= 0x01;
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
  static unsigned buffer_index = 0;
  char *payload;
  // UCA1IFG &= ~0x01;
    
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:                                   // Vector 2 - RXIFG
    while (!(UCA1IFG&UCTXIFG));             // USCI_A0 TX buffer ready?
    // Attach to buffer char array
    if (UCA1RXBUF == '@' || UCA1RXBUF == '\0' || UCA1RXBUF == '\n') {
      command_string[buffer_index] = '\0';
      UCA1_RX_BUFFER[buffer_index] = '\0';      
      buffer_index = 0;

      switch (parse_command(command_string, &payload)) {   // Update Light Frame Data
      case MODE_FRAME:
        light_control_update(payload);
        break;
      default:
        light_control_update(payload);
        break;
      }
    } else {
      command_string[buffer_index] = UCA1RXBUF;
      UCA1_RX_BUFFER[buffer_index] = UCA1RXBUF;
      buffer_index++;
    }
    
    UCA1TXBUF = UCA1RXBUF;                  // TX -> RXed character
    break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}


