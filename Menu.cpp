#include "Menu.h"
#include "../inc/ST7735.h"
#include "Switch.h"
#include <ti/devices/msp/msp.h>

#include "../inc/Clock.h"
#include "images/images.h"

int language = 0;


void Menu_Init(){
    ST7735_FillScreen(ST7735_BLACK);
}

/*void Menu_Start(){

    
    int select = 0;
    int langSelect = 0;
    int state = 0;
    int prevSelect = 0;

    while(1){
        int input = Switch_In();

        if(state == 0){

        ST7735_FillScreen(ST7735_BLACK);

        ST7735_SetCursor(2,2);
        ST7735_OutString("TAG");

         ST7735_SetCursor(2,4);
        ST7735_OutString("Press right button to select");


         ST7735_SetCursor(2,6);
        
         if(select == 0){
            if(language == 0){
                ST7735_OutString(" > Start Game");
            } else {
                ST7735_OutString(" > Iniciar Juego");
            }
         } else {
            if(language == 0){
                ST7735_OutString("Start Game");
            } else {
                ST7735_OutString("Iniciar Juego");
            }
         }

         ST7735_SetCursor(2,8);
         if(select == 1){
            if(language == 0){
                ST7735_OutString(" > Language");
            } else {
                ST7735_OutString(" > Idioma");
            }
         } else {
            if(language == 0){
                ST7735_OutString("Language");
            } else {
                ST7735_OutString("Idioma");
            }
         }

         if(input & 0x01){
            select--;
            if(select < 0){
                select = 0;
            }
         }
         if(input & 0x02){
            select++;
            if(select > 1){
                select = 1;
            }
         }

         if(input & 0x04){
            if(select == 1){
                state = 1;
                langSelect = 0;
            } 
            if(select == 0){
                return;
            }
         }

        }

        else if(state == 1){


            ST7735_FillScreen(ST7735_BLACK);
          
            ST7735_SetCursor(2,2);
            ST7735_OutString("Select Language");

            ST7735_SetCursor(2,4);
            ST7735_OutString("Press right button to select");

            ST7735_SetCursor(2,6);
         if(langSelect == 0){
            ST7735_OutString(" > English");
         } else {
            ST7735_OutString("English");
         }

         ST7735_SetCursor(2,8);
         if(langSelect  == 1){
            ST7735_OutString(" > Spanish");
         } else {
            ST7735_OutString("Spanish");
         }

         if(input & 0x01){
            langSelect--;
            if(langSelect < 0){
                langSelect = 0;
            }
         }
         if(input & 0x02){
            langSelect++;
            if(langSelect > 1){
                langSelect = 1;
            }
         }

         if(input & 0x04){
            language = langSelect;
            state = 0;
            select = 0;
            Clock_Delay1ms(300); 
         }

         continue;
        }


        Clock_Delay1ms(150);

    }
    
}
*/

void Menu_Start(){
    int select = 0;
    int langSelect = 0;
    int state = 0;
    int needsRedraw = 1;

    while(1){
        int input = Switch_In();
        int prevSelect = select;
        int prevLangSelect = langSelect;
        int prevState = state;

        if(state == 0){
            if(input & 0x01){ 
                select--; 
                if(select < 0) 
                select = 0; 
            }
            if(input & 0x02){ 
                select++; 
                if(select > 1) 
                select = 1; 
            }
            if(input & 0x04){
                if(select == 1){ 
                    state = 1; 
                    langSelect = 0; 
                    }
                if(select == 0){ 
                    ST7735_FillScreen(ST7735_BLACK);
                    ST7735_SetRotation(0);
                    return; 
                    }
            }
        }
        else if(state == 1){
            if(input & 0x01){ 
                langSelect--; 
                if(langSelect < 0) 
                langSelect = 0; 
            }
            if(input & 0x02){ 
                langSelect++; 
                if(langSelect > 1) 
                langSelect = 1; }
            if(input & 0x04){
                language = langSelect;
                state = 0;
                select = 0;
                Clock_Delay1ms(300);
            }
        }

        if(select != prevSelect || langSelect != prevLangSelect || state != prevState || needsRedraw){
            needsRedraw = 0;
            ST7735_FillScreen(ST7735_BLACK);
            ST7735_DrawBitmap(0, 127, backgroun, 160, 128);

            if(state == 0){
                ST7735_SetCursor(2,2);
                if(language == 0){
                    ST7735_OutString((char *)"Tag");
                } else {
                    ST7735_OutString((char *)"ETIQUETA");
                }

                ST7735_SetCursor(2,4);
                if(language == 0){
                    ST7735_OutString((char *)"Left is select");
                } else {
                    ST7735_OutString((char *)"Presiona izquierda");
                }

                ST7735_SetCursor(2,6);
                if(select == 0){
                    if(language == 0){
                        ST7735_OutString((char *)" > Start Game");
                    } else {
                        ST7735_OutString((char *)" > Iniciar Juego");
                    }
                } else {
                    if(language == 0){
                        ST7735_OutString((char *)"Start Game");
                    } else {
                        ST7735_OutString((char *)"Iniciar Juego");
                    }
                }

                ST7735_SetCursor(2,8);
                if(select == 1){
                    if(language == 0){
                        ST7735_OutString((char *)" > Language");
                    } else {
                        ST7735_OutString((char *)" > Idioma");
                    }
                } else {
                    if(language == 0){
                        ST7735_OutString((char *)"Language");
                    } else {
                        ST7735_OutString((char *)"Idioma");
                    }
                }
            }
            else if(state == 1){
                ST7735_SetCursor(2,2);
                if(language == 0){
                    ST7735_OutString((char *)"Language");
                } else {
                    ST7735_OutString((char *)"Idioma");
                }

                ST7735_SetCursor(2,4);
                if(language == 0){
                    ST7735_OutString((char *)"Press left to select");
                } else {
                    ST7735_OutString((char *)"Presiona izquierda");
                }

                ST7735_SetCursor(2,6);
                if(langSelect == 0){
                    ST7735_OutString((char *)" > English");
                } else {
                    ST7735_OutString((char *)"English");
                }

                ST7735_SetCursor(2,8);
                if(langSelect == 1){
                    ST7735_OutString((char *)" > Spanish");
                } else {
                    ST7735_OutString((char *)"Spanish");
                }
            }
        }

        Clock_Delay1ms(150);

    }
}