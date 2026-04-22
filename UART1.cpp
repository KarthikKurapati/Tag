#include <ti/devices/msp/msp.h>
#include "../inc/Clock.h"
#include "UART1.h"
#define PA8INDEX  18

void UART1_Init(void){
    // do not reset or activate PortA, already done in LaunchPad_Init
     // write this
  UART1->GPRCM.RSTCTL = 0xB1000003;
  UART1->GPRCM.PWREN = 0x26000001;
  Clock_Delay(24); // time for uart to power up
  IOMUX->SECCFG.PINCM[PA8INDEX]  = 0x00000082;
  
  UART1->CLKSEL = 0x08; // bus clock
  UART1->CLKDIV = 0x00; // no divide
  UART1->CTL0 &= ~0x01; // disable UART0
  UART1->CTL0 = 0x00020018;
 // assumes an 80 MHz bus clock
  UART1->IBRD = 21;//   divider = 21+45/64 = 21.703125
  UART1->FBRD = 45; // baud =2,500,000/21.703125 = 115,191 baud rate
  UART1->LCRH = 0x00000030;
  UART1->CTL0 |= 0x01; // enable UART0

}

void UART1_basicUartOut(char data){
    UART1->TXDATA = data;
}
