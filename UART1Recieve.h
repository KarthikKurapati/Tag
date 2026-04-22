#ifndef __UART1Recieve_H__
#define __UART1Recieve_H__

#include "FIFO2.h"

void UART2_Init_Simple(void);
char UART2_InChar_Simple(void);
char UART2_InChar(void);
int UART2_RxSize(void);
bool UART2_TryReadPacket(uint8_t *x, uint8_t *y, uint8_t *z);
#endif