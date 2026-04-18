// Lab9HMain.cpp
// Runs on MSPM0G3507
// Lab 9 ECE319H
// Karthik Kurapati
// Last Modified: January 12, 2026

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/SlidePot.h"
#include "../inc/DAC5.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "images/images.h"
#include "Entities.h"
#include "Joystick.h"
// extern "C" void __disable_irq(void);
// extern "C" void __enable_irq(void);
extern "C" void TIMG12_IRQHandler(void);
// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void){ // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}
uint8_t currentZ = 0;
uint8_t DrawFlag = 0;
uint32_t M=1;
void DrawMap(const uint8_t map[ROWS][COLS]);

Joystick MyJoy(2048, 50);

uint32_t Random32(void){
  M = 1664525*M+1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32()>>16)%n;
}


Player Player1(55, 100, PlayerShip0, 1);

void RoutePlayer(uint8_t doorID){
switch (doorID) {
  case 2:
    currentZ = 1;       // Load Room 1
    Player1.setX(10);   // Spawn them on the left side
    Player1.setY(24);   // Line up the Y coordinates with the door
    break;
  case 3:
    currentZ = 0;
    Player1.setX(50);
    Player1.setY(30);
    break;
  case 4:
    currentZ = 2;
    Player1.setX(10);
    Player1.setY(24);
    break;
}
}

void DrawStaminaBar(int32_t currentStamina) {
    // Let's make the bar 100 pixels tall, starting at Y = 20, on the right edge (X = 122)
    int32_t barX = 122;
    int32_t barTopY = 30;
    int32_t barWidth = 4;
    int32_t barHeight = 100;

    int32_t emptyHeight = barHeight - currentStamina;

    // 1. Draw the Empty Part (Black, starting from the top and growing down)
    if (emptyHeight > 0) {
        ST7735_FillRect(barX, barTopY, barWidth, emptyHeight, ST7735_BLACK);
    }

    // 2. Draw the Filled Part (Blue, starting below the empty part and filling to the bottom)
    if (currentStamina > 0) {
        ST7735_FillRect(barX, barTopY + emptyHeight, barWidth, currentStamina, ST7735_RED);
    }
    
    // Draw a nice white border around the whole thing so we can see the max capacity!
    ST7735_DrawFastVLine(barX - 1, barTopY - 1, barHeight + 2, ST7735_WHITE);
    ST7735_DrawFastVLine(barX + barWidth, barTopY - 1, barHeight + 2, ST7735_WHITE);
    ST7735_DrawFastHLine(barX - 1, barTopY - 1, barWidth + 2, ST7735_WHITE);
    ST7735_DrawFastHLine(barX - 1, barTopY + barHeight, barWidth + 2, ST7735_WHITE);
}


// games  engine runs at 30Hz
void TIMG12_IRQHandler(void){uint32_t pos,msg;
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
// game engine goes here
    // 1) sample slide pot
    // 2) read input switches
    // 3) move sprites
    // 4) start sounds
    // 5) set semaphore
    // NO LCD OUTPUT IN INTERRUPT SERVICE ROUTINES
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)

    MyJoy.In();

    uint32_t buttons = Switch_In();
    uint8_t isHoldingBoost = (buttons & 0x01) ? 1 : 0;

    uint8_t isBoosting = 0;
    if (isHoldingBoost && Player1.getStamina() > 0) {
        isBoosting = 1;
    }

  Player1.updateStamina(isBoosting);

    // 3) Update new coordinates automatically!
    int32_t dx = MyJoy.getXChange();
    int32_t dy = MyJoy.getYChange();


    if(dy != 0 || dx != 0){
      bool diag = (dx != 0) && (dy != 0);  
      int32_t speed = diag ? 2 : 3;

      if (isBoosting) {
          speed = diag ? 3 : 5;
      }

      int32_t moveX = dx * speed;
      int32_t moveY = dy * speed;

      

      uint8_t event = Player1.Move(moveX, moveY, World[currentZ]);

      if(event > 1){
        RoutePlayer(event);


        ST7735_FillScreen(ST7735_BLACK); 
        DrawMap(World[currentZ]);
        DrawFlag = 1;
      }

      if(Player1.getX() != Player1.getPrevX() || Player1.getY() != Player1.getPrevY()){
        DrawFlag = 1;
      }
    }
    if (Player1.getStamina() != Player1.getPrevStamina()) {
      DrawFlag = 1;
    }
    Player1.updateStamina(isBoosting);
    GPIOB->DOUTTGL31_0 = GREEN;
  }
}
uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}

typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Language_Portuguese[]="Portugu\x88s";
const char Language_French[]="Fran\x87" "ais";
const char *Phrases[3][4]={
  {Hello_English,Hello_Spanish,Hello_Portuguese,Hello_French},
  {Goodbye_English,Goodbye_Spanish,Goodbye_Portuguese,Goodbye_French},
  {Language_English,Language_Spanish,Language_Portuguese,Language_French}
};
// use main1 to observe special characters
int main1(void){ // main1
    char l;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf(INITR_REDTAB); // INITR_REDTAB for AdaFruit, INITR_BLACKTAB for HiLetGo
  ST7735_FillScreen(0x0000);            // set screen to black
  for(int myPhrase=0; myPhrase<= 2; myPhrase++){
    for(int myL=0; myL<= 3; myL++){
         ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
      ST7735_OutChar(' ');
         ST7735_OutString((char *)Phrases[myPhrase][myL]);
      ST7735_OutChar(13);
    }
  }
  Clock_Delay1ms(3000);
  ST7735_FillScreen(0x0000);       // set screen to black
  l = 128;
  while(1){
    Clock_Delay1ms(2000);
    for(int j=0; j < 3; j++){
      for(int i=0;i<16;i++){
        ST7735_SetCursor(7*j+0,i);
        ST7735_OutUDec(l);
        ST7735_OutChar(' ');
        ST7735_OutChar(' ');
        ST7735_SetCursor(7*j+4,i);
        ST7735_OutChar(l);
        l++;
      }
    }
  }
}

// use main2 to observe graphics
int main2(void){ // main2
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf(INITR_BLACKTAB); // INITR_REDTAB for AdaFruit, INITR_BLACKTAB for HiLetGo
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_DrawBitmap(22, 159, PlayerShip0, 18,8); // player ship bottom
  ST7735_DrawBitmap(53, 151, Bunker0, 18,5);
  ST7735_DrawBitmap(42, 159, PlayerShip1, 18,8); // player ship bottom
  ST7735_DrawBitmap(62, 159, PlayerShip2, 18,8); // player ship bottom
  ST7735_DrawBitmap(82, 159, PlayerShip3, 18,8); // player ship bottom
  ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
  ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
  ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
  ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16,10);
  ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);

  for(uint32_t t=500;t>0;t=t-5){
    SmallFont_OutVertical(t,104,6); // top left
    Clock_Delay1ms(50);              // delay 50 msec
  }
  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString((char *)"GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString((char *)"Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString((char *)"Earthling!");
  ST7735_SetCursor(2, 4);
  ST7735_OutUDec(1234);
  while(1){
  }
}

// use main3 to test switches and LEDs
int main3(void){ // main3
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  while(1){
    // write code to test switches and LEDs

  }
}
void DrawMap(const uint8_t map[ROWS][COLS]){
  int32_t pixel_x = 0;
  int32_t pixel_y = 0;
  for(int i = 0; i < ROWS;i++){
    for(int j = 0; j < COLS;j++){
      if(map[i][j] == 1){
        pixel_x = j * 8;
        pixel_y = i * 8;
        ST7735_FillRect(pixel_x,pixel_y, 8, 8, ST7735_WHITE);
      }
      if(map[i][j] > 1){
        pixel_x = j * 8;
        pixel_y = i * 8;
        ST7735_FillRect(pixel_x,pixel_y, 8, 8, ST7735_BLUE);
      }
    }
  }
}

// use main4 to test sound outputs
int main4(void){ uint32_t last=0,now;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(ADC0,6,0); // ADC1 channel 6 is PB20, TExaS scope
  __enable_irq();
  while(1){
    now = Switch_In(); // one of your buttons
    if((last == 0)&&(now == 1)){
      Sound_Shoot(); // call one of your sounds
    }
    if((last == 0)&&(now == 2)){
      Sound_Killed(); // call one of your sounds
    }
    if((last == 0)&&(now == 4)){
      Sound_Explosion(); // call one of your sounds
    }
    if((last == 0)&&(now == 8)){
      Sound_Fastinvader1(); // call one of your sounds
    }
    // modify this to test all your sounds
  }
}
// ALL ST7735 OUTPUT MUST OCCUR IN MAIN
int main(void){ // final main
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf(INITR_BLACKTAB); // INITR_REDTAB for AdaFruit, INITR_BLACKTAB for HiLetGo
  ST7735_FillScreen(ST7735_BLACK);
  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
  
  MyJoy.Init();
    // initialize interrupts on TimerG12 at 30 Hz
  TimerG12_IntArm(80000000/30, 2);
  // initialize all data structures
  __enable_irq();


  DrawMap(World[currentZ]);


  DrawFlag = 1;
  while(1){
    while(DrawFlag == 0) {}
    DrawFlag = 0;
    
    DrawStaminaBar(Player1.getStamina());
    Player1.Erase();
    Player1.Draw();
  }
}
