// Sound.cpp
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// your name
// your data 
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "sounds/sounds.h"
#include "../inc/DAC5.h"
#include "../inc/Timer.h"

static const uint8_t *SoundPoint; //points to sound array
static uint32_t SoundCount;


void SysTick_IntArm(uint32_t period, uint32_t priority){
  // write this
  SysTick->CTRL = 0;
  SysTick->LOAD = period - 1;
  SysTick->VAL = 0;
  NVIC_SetPriority(SysTick_IRQn, priority);
  SysTick->CTRL = 0x07; 

}
// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5 bit DAC
void Sound_Init(void){
  DAC5_Init();
  SoundPoint = 0;
  SoundCount = 0;

  SysTick->CTRL = 0;
  SysTick->LOAD = 7272 -1;
  SysTick->VAL = 0;
  SysTick->CTRL = 0x05;

}

extern "C" void SysTick_Handler(void);
void SysTick_Handler(void){ // called at 11 kHz
  // output one value to DAC if a sound is active
    // output one value to DAC if a sound is active
   if(SoundCount > 0){
     DAC5_Out(*SoundPoint);
      SoundPoint++;
      SoundCount--;
   } else {
    SysTick->CTRL = 0x05;
   }
}

//******* Sound_Start ************
// This function does not output to the DAC. 
// Rather, it sets a pointer and counter, and then enables the SysTick interrupt.
// It starts the sound, and the SysTick ISR does the output
// feel free to change the parameters
// Sound should play once and stop
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement
void Sound_Start(const uint8_t *pt, uint32_t count){
    SoundCount = 0;
    SysTick->CTRL = 0x05;
    SoundPoint = pt;
    SoundCount = count;
    SysTick->CTRL = 0x07;
}

void Sound_Stop(void){
  SoundCount = 0;
  DAC5_Out(0);
  SysTick->CTRL = 0x05;
}

void Sound_Shoot(void){
// write this
  
}

void Sound_Walk(void){
  Sound_Start(footsteps, 2000);
}

void Sound_Tag(void){
  Sound_Start(tag, 29477);

}
void Sound_Explosion(void){
// write this

}

void Sound_Fastinvader1(void){

}
void Sound_Fastinvader2(void){

}
void Sound_Fastinvader3(void){

}
void Sound_Fastinvader4(void){

}
void Sound_Highpitch(void){

}