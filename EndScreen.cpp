#include "EndScreen.h"
#include "Menu.h"
#include "../inc/ST7735.h"
#include "Switch.h"
#include <ti/devices/msp/msp.h>

#include "../inc/Clock.h"
#include "images/images.h"



void EndScreenInit(){
    ST7735_FillScreen(ST7735_BLACK);
}

void EndScreenRun(uint32_t score){
    int needsRedraw = 0;


    while(1){
        int input = Switch_In();

        if(needsRedraw == 0){



        ST7735_SetCursor(2,2);
        ST7735_OutString((char *)"Game Over!");

         ST7735_SetCursor(2,4);
        if(score == 0){
            if(language == 0){
                ST7735_OutString((char *)"You Survived!");
            } else {
                ST7735_OutString((char *)"¡Sobre Viviste!");
            }
        } else {
           if(language == 0){
               ST7735_OutString((char *)"You died!");
          } else {
               ST7735_OutString((char *)"¡Has muerto!");
           }
             ST7735_SetCursor(2,5);
             if(language == 0){
                ST7735_OutString((char *)"Score out of 60:");
             } else {
                ST7735_OutString((char *)"Puntuacion Sobre 60");
             }
             ST7735_SetCursor(2,6);
            ST7735_OutUDec(score);

            ST7735_SetCursor(2,8);
        if(language == 0){
            ST7735_OutString((char *)" > Restart Game?");
        } else {
            ST7735_OutString((char *)" > Reinciar Juego?");
        }

        needsRedraw = 1;
    }

    }
    if(input & 0x04){
            Clock_Delay1ms(200);
            Menu_Init();
            Menu_Start();
            return;
        }

    
    }
}
