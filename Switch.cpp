/*
 * Switch.cpp
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table
void Switch_Init(void){
    // write this
  IOMUX->SECCFG.PINCM[PA28INDEX] = 0x00040081; // input, no pull
  IOMUX->SECCFG.PINCM[PA27INDEX] = 0x00040081; // input, no pull
  IOMUX->SECCFG.PINCM[PA16INDEX] = 0x00040081; // input, no pull
  IOMUX->SECCFG.PINCM[PA15INDEX] = 0x00040081; // input, no pull
  IOMUX->SECCFG.PINCM[PA18INDEX] = 0x00050081; //pull up resistors
  IOMUX->SECCFG.PINCM[PB21INDEX] = 0x00060081; // pull down resistors
  IOMUX->SECCFG.PINCM[PA10INDEX] = 0x00040081; // pull down resistors
  IOMUX->SECCFG.PINCM[PA17INDEX] = 0x00040081; 

}
// return current state of switches
uint32_t Switch_In(void){
    // write this
  uint32_t data = GPIOA->DIN31_0;
  //data = ((data>>15)&0x03) | ((data&((1<<28)|(1<<27)))>>25);
  //return data; // return 0; //replace this your code

  uint32_t input = 0;

  if(data & (1<<16)) input |= 0x01; // up button
  if(data & (1<<17)) input |= 0x02; // down button
  if(data & (1<<15)) input |= 0x04; // left
  if(data & (1<<10)) input |= 0x08; // right

  return input;

}