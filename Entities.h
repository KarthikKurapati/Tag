#ifndef ENTITIES_H
#define ENTITIES_H

#include <stdint.h>
#include "../inc/ST7735.h"

#define COLS 16
#define ROWS 20

class Sprite{
protected:
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
    int32_t prevX;
    int32_t prevY;
    const uint16_t *image; 
public:
    Sprite(int32_t startX, int32_t startY, int32_t w, int32_t h, const uint16_t *img);
    void Erase(void);
    void Draw(void);
    int32_t getX();
    int32_t getY();
    int32_t getPrevX();
    int32_t getPrevY();
    void setX(int32_t x);
    void setY(int32_t y);
};
class Player : public Sprite {
  private:
    uint8_t isTagger;
    uint8_t isAlive;
    int32_t stamina;      
    int32_t maxStamina;   
    int32_t prevStamina;
    uint8_t recharge;

  public:
    Player(int32_t startX, int32_t startY, const uint16_t *img, uint8_t taggerStatus);
    void Tagged();
    uint8_t Move(int32_t dx, int32_t dy, const uint8_t map[][COLS]);
    void updateStamina(uint8_t isBoosting);
    int32_t getStamina();
    int32_t getPrevStamina();
};

#endif