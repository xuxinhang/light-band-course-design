#include <msp430.h>

extern char command_string[128];

int bluetooth_port_init(void);

__interrupt void USCI_A1_ISR(void);

void test_rewrite_rx_buffer (void);



