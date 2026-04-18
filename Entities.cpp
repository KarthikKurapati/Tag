#include "Entities.h"

uint8_t CheckCollision(int32_t next_x, int32_t next_y, int32_t width, int32_t height, const uint8_t map[][COLS]) {
    uint8_t touchedTile = 0;
    int32_t left = next_x;
    int32_t right = next_x + (width - 1); 
    int32_t bottom = next_y;
    int32_t top = next_y - (height - 1);

    // Calculate grid coordinates
    int32_t col_left = left / 8;
    int32_t col_right = right / 8;
    int32_t row_bottom = bottom / 8;
    int32_t row_top = top / 8;

    // Screen Boundary Safety
    if(col_left < 0 || col_right >= COLS || row_top < 0 || row_bottom >= ROWS) return 1; 

    // THE UPGRADE: Loop through EVERY cell the sprite is touching!
    for(int32_t row = row_top; row <= row_bottom; row++) {
        for(int32_t col = col_left; col <= col_right; col++) {
            
            // If ANY of the blocks we are touching is a wall, stop!
            if(map[row][col] == 1) {
                return 1; 
            }
            if(map[row][col] > 1){
                touchedTile = map[row][col];
            }
        }
    }

    // If the loop finishes without finding a 1, the whole area is safe!
    return touchedTile;
}

Sprite::Sprite(int32_t startX, int32_t startY, int32_t w, int32_t h, const uint16_t *img) {
    x = startX;
    y = startY;
    prevX = startX;
    prevY = startY;
    width = w;
    height = h;
    image = img;
}
void Sprite :: Erase(void){
    ST7735_FillRect(prevX, prevY - height + 1, width, height, ST7735_BLACK);
}
void Sprite::Draw(void){
    ST7735_DrawBitmap(x, y, image, width, height);
}
int32_t Sprite::getX() { return x; }
int32_t Sprite::getY() { return y; }
int32_t Sprite::getPrevX() { return prevX; }
int32_t Sprite::getPrevY() { return prevY; }

Player::Player(int32_t startX, int32_t startY, const uint16_t *img,uint8_t taggerStatus) 
: Sprite(startX,startY, 18, 8, img){
    isTagger = taggerStatus;
    isAlive = 1;
    stamina = 100;
    prevStamina = 100;
    maxStamina = 100;
    recharge = 0;
}
uint8_t Player::Move(int32_t dx, int32_t dy, const uint8_t map[ROWS][COLS]) {
    prevX = x;
    prevY = y;
    int32_t proposed_x = x + dx;
    int32_t proposed_y = y + dy;

    uint8_t hitX = CheckCollision(proposed_x, y, width, height, map);
    uint8_t hitY = CheckCollision(x, proposed_y, width, height, map);

    if(hitX != 1){
        x = proposed_x;
    }
    if(hitY != 1){
        y = proposed_y;
    }

    if(hitX > 1) return hitX;
    if(hitY > 1)return hitY;

    return 0; 
}
void Sprite::setX(int32_t x){
    this->prevX = x;
    this->x = x;
}
void Sprite::setY(int32_t y){
    this->prevY = y;
    this->y = y;
}
int32_t Player::getStamina() { return stamina; }
int32_t Player::getPrevStamina() { return prevStamina; }
void Player::updateStamina(uint8_t isBoosting) {
    prevStamina = stamina; // Remember where we were before math

    if (isBoosting == 1 && stamina > 0) {
        stamina -= 2;
        if (stamina < 0) stamina = 0; 

        recharge = 0;
    } 
    else if (isBoosting == 0 && stamina < maxStamina) {
        recharge++;
        if (recharge >= 3) {
            stamina += 1;
            recharge = 0;
        }
    }
}
