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
    int32_t getWidth();
    int32_t getHeight();
    void setX(int32_t x);
    void setY(int32_t y);
    uint8_t getIsAlive();
};
class Player : public Sprite {
  private:
    uint8_t isTagger;
    uint8_t isAlive;
    int32_t stamina;      
    int32_t maxStamina;   
    int32_t prevStamina;
    uint8_t recharge;
    uint8_t BFSMove(int32_t goalPixelX, int32_t goalPixelY, const uint8_t map[][COLS], uint8_t speed);

  public:
    Player(int32_t startX, int32_t startY, const uint16_t *img, uint8_t taggerStatus);
    void Tagged();
    uint8_t Move(int32_t dx, int32_t dy, const uint8_t map[][COLS]);
    void updateStamina(uint8_t isBoosting);
    int32_t getStamina();
    int32_t getPrevStamina();
};

class Enemy : public Sprite {
private:
    uint8_t  isAlive;
    uint8_t  prevIsAlive;
    uint8_t  spawnDelay;
    uint8_t  moveTimer;
 
    void Chase(int32_t playerX, int32_t playerY, const uint8_t map[][COLS]);
 
  public:
    Enemy(int32_t startX, int32_t startY, const uint16_t *img);
 
    // 30hz, use to change number of ticks 
    void SetSpawnDelay(int32_t spawnX, int32_t spawnY, uint8_t delayFrames);
 
    // handles the chasing
    void Update(int32_t playerX, int32_t playerY, const uint8_t map[][COLS]);
 
    // returns 1 if enemy is tagged
    uint8_t CheckTagged(int32_t playerX, int32_t playerY,
                        int32_t playerW, int32_t playerH);
 
    // erase / draw
    void EnemyErase(void);
    void EnemyDraw(void);
 
    uint8_t getIsAlive();
    uint8_t getPrevIsAlive();
};
#endif