#include "Joystick.h"
#include <ti/devices/msp/msp.h>
#include <stdio.h>
#include <stdint.h>
#include "../inc/Clock.h"
Joystick::Joystick(uint32_t center_val, uint32_t deadzone_val){
    dataX = center_val;
    dataY = center_val;
    center = center_val;
    deadzone = deadzone_val;
}

void Joystick::Init(void){
    ADC1->ULLMEM.GPRCM.RSTCTL = 0xB1000003; 
    ADC1->ULLMEM.GPRCM.PWREN = 0x26000001;  
    Clock_Delay(24);

    ADC1->ULLMEM.GPRCM.CLKCFG = 0xA9000000;                        
    
    IOMUX->SECCFG.PINCM[43] = 0x00000081;   
    IOMUX->SECCFG.PINCM[45] = 0x00000081;
    
    ADC1->ULLMEM.CLKFREQ = 0x00000007;    
    ADC1->ULLMEM.CTL0 = 0x03010000;         
    ADC1->ULLMEM.CTL1 = 0x00000000; 
    ADC1->ULLMEM.CTL2 = 0x00000000;
}

void Joystick::In(void){
    uint32_t sumX = 0;
    uint32_t sumY = 0;
    for(int i = 0;i < 32;i++){
        // this is for x axis
        ADC1->ULLMEM.CTL0 &= ~0x00000001;
        ADC1->ULLMEM.MEMCTL[0] = 6;
        // Enable conversions
        ADC1->ULLMEM.CTL0 |= 0x00000001;  
        // Start ADC
        ADC1->ULLMEM.CTL1 |= 0x00000100;    
        // delay for adc start
        uint32_t volatile delay = ADC1->ULLMEM.STATUS;
        // wait for it to sample
        while((ADC1->ULLMEM.STATUS & 0x01) == 0x01){}  
        // return result
        sumX += ADC1->ULLMEM.MEMRES[0];

        // this is for y axis
        ADC1->ULLMEM.CTL0 &= ~0x00000001;
        ADC1->ULLMEM.MEMCTL[0] = 4;
        // Enable conversions
        ADC1->ULLMEM.CTL0 |= 0x00000001;  
        // Start ADC
        ADC1->ULLMEM.CTL1 |= 0x00000100;    
        // delay for adc start
        delay = ADC1->ULLMEM.STATUS;
        // wait for it to sample
        while((ADC1->ULLMEM.STATUS & 0x01) == 0x01){}  
        // return result
        sumY += ADC1->ULLMEM.MEMRES[0];
    }
    dataX = sumX >> 5;
    dataY = sumY >> 5;
}

uint32_t Joystick::getX(void){
    return dataX;
}
uint32_t Joystick::getY(void){
    return dataY;
}
int32_t Joystick::getXChange(void){
    if(dataX > (center + deadzone)) return 1;  // Right
    if(dataX < (center - deadzone)) return -1; // Left
    return 0;                                  // Deadzone
}
int32_t Joystick::getYChange(void){
    if(dataY > (center + deadzone)) return -1; // Up (Remember ST7735 Y=0 is top!)
    if(dataY < (center - deadzone)) return 1;  // Down
    return 0;
}


