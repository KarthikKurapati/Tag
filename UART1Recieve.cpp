#include <ti/devices/msp/msp.h>
#include "../inc/Clock.h"
#include "FIFO2.h"
#include "../inc/ST7735.h"
#define PA22INDEX 46

// initalization with interrupts
Queue Uart2RxQueue;
uint32_t Uart2LostData = 0;

void UART2_Init_Simple(void){
    // do not reset or activate PortA, already done in LaunchPad_Init
    // RSTCLR to UART2 peripherals
    // bits 31-24 unlock key 0xB1
    // bit 1 is Clear reset sticky bit
    // bit 0 is reset gpio port

    // Enable power to UART1 peripherals
    // write this
    UART2->GPRCM.RSTCTL = 0xB1000003;
    UART2->GPRCM.PWREN = 0x26000001;
    Clock_Delay(24);

    IOMUX->SECCFG.PINCM[PA22INDEX] = 0x00040082; // U2_RX

    

    UART2->CLKSEL = 0x08;
    UART2->CLKDIV = 0x00;
    UART2->CTL0 &= ~0x01;
    UART2->CTL0 = 0x00020018;

    UART2->IBRD = 21; // for 115,191 baud rate 
    UART2->FBRD = 45;
    UART2->LCRH = 0x00000030;

    UART2->CPU_INT.IMASK = 0x0001; // only RTOUT
    UART2->IFLS = 0x0422; // leave as instructed

    NVIC->ICPR[0] = 1<<14;
    NVIC->ISER[0] = 1<<14;
    NVIC->IP[3] = (NVIC->IP[3] & (~0x00C00000)) | (2<<22);

    UART2->CTL0 |= 0x01;
}

char UART2_InChar(){
    char letter;
    while(Uart2RxQueue.Get(&letter) == false){}
    return letter;
}

extern "C" void UART2_IRQHandler(void){
    uint32_t status = UART2->CPU_INT.IIDX;

    if(status == 0x01){
        char letter;
        

        while((UART2->STAT & 0x04) == 0){   
            
            char letter = (char)(UART2->RXDATA);
            
            // put letter in fifo
            if(Uart2RxQueue.Put(letter) == false){
                // we lost data if we got here
                Uart2LostData++; 
            }
        }
    
    }

}

int UART2_RxSize(void){
    return Uart2RxQueue.Size();
}

bool UART2_TryReadPacket(uint8_t *x, uint8_t *y, uint8_t *z){
    if(Uart2RxQueue.Size() < 5) return false;
    
    char sync;
    Uart2RxQueue.Get(&sync);
    if(sync != (char)255) return false;

    char rx, ry, rz, rend;
    Uart2RxQueue.Get(&rx);
    Uart2RxQueue.Get(&ry);
    Uart2RxQueue.Get(&rz);
    Uart2RxQueue.Get(&rend);

    *x = (uint8_t)rx;
    *y = (uint8_t)ry;
    *z = (uint8_t)rz;
    return true;
}


char UART2_InChar_Simple(void){
    // Wait while the Receive FIFO Empty (RXFE) flag is 1 (Bit 6)
    while ((UART2->STAT & 0x04) == 0x04) {}; 
    
    // Once the flag is 0, data has arrived. Read and return it.
    return (char)(UART2->RXDATA);
}

