#include "Entities.h"
#include "Sound.h"
#include "sounds/sounds.h"

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
int32_t Sprite::getWidth()  { return width; }
int32_t Sprite::getHeight() { return height; }
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

    static uint8_t stepBreak = 0;
    if(x != prevX  || y != prevY){
        if(stepBreak == 0){
            Sound_Walk();
            stepBreak = 8;
        }
    } else {
        if(stepBreak != 0){
            Sound_Stop();
            stepBreak = 0;
        }
    }
    if(stepBreak > 0){
        stepBreak--;
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
// -------------------------------------------------------
//  Enemy implementation
// -------------------------------------------------------
Enemy::Enemy(int32_t startX, int32_t startY, const uint16_t *img)
    : Sprite(startX, startY, 18, 8, img) {
    isAlive      = 0;   // starts inactive until first SetSpawnDelay call
    prevIsAlive  = 0;
    spawnDelay   = 0;
    moveTimer    = 0;
}
 
void Enemy::SetSpawnDelay(int32_t spawnX, int32_t spawnY, uint8_t delayFrames) {
    prevIsAlive = isAlive;  // remember state so main loop can erase old position
    isAlive     = 0;
    spawnDelay  = delayFrames;
    setX(spawnX);
    setY(spawnY);
    moveTimer   = 0;
}
 
// ---------------------------------------------------------------
//  Chase — greedy pathfinder:
//   1. Try to move diagonally toward player (dx + dy).
//   2. If diagonal X-axis is blocked, slide on Y only.
//   3. If diagonal Y-axis is blocked, slide on X only.
//   This lets the enemy hug walls and navigate corridors naturally.
// ---------------------------------------------------------------
void Enemy::Chase(int32_t playerX, int32_t playerY,
                  const uint8_t map[][COLS]) {
    prevX = x;
    prevY = y;
 
    // Determine direction signs toward player
    int32_t sx = 0, sy = 0;
    if      (playerX > x) sx =  2;
    else if (playerX < x) sx = -2;
    if      (playerY > y) sy =  2;
    else if (playerY < y) sy = -2;
 
    // Attempt X movement
    if (sx != 0) {
        uint8_t hitX = CheckCollision(x + sx, y, width, height, map);
        if (hitX != 1) x += sx;
    }
 
    // Attempt Y movement
    if (sy != 0) {
        uint8_t hitY = CheckCollision(x, y + sy, width, height, map);
        if (hitY != 1) y += sy;
    }
}
 
void Enemy::Update(int32_t playerX, int32_t playerY,
                   const uint8_t map[][COLS]) {
    prevIsAlive = isAlive;
 
    // ---- Countdown phase ----
    if (!isAlive) {
        if (spawnDelay > 0) {
            spawnDelay--;
        } else {
            isAlive = 1;  // time's up — appear!
        }
        return;
    }
 
    // ---- Active chase phase ----
    // Throttle: move every 2nd frame so enemy is slightly slower than player
    moveTimer++;
    if (moveTimer < 2) return;
    moveTimer = 0;
 
    Chase(playerX, playerY, map);
}
 
// AABB overlap check between enemy rect and player rect.
// ST7735 bitmaps: (x, y) is bottom-left corner, sprite goes UP by height-1.
uint8_t Enemy::CheckTagged(int32_t playerX, int32_t playerY,
                           int32_t playerW,  int32_t playerH) {
    if (!isAlive) return 0;
 
    int32_t ex1 = x,       ex2 = x + width  - 1;
    int32_t ey1 = y - height + 1, ey2 = y;
 
    int32_t px1 = playerX, px2 = playerX + playerW - 1;
    int32_t py1 = playerY - playerH + 1, py2 = playerY;
 
    if (ex1 <= px2 && ex2 >= px1 && ey1 <= py2 && ey2 >= py1) {
        return 1;  // TAGGED!
    }
    return 0;
}
 
// Erase only if the enemy was alive last frame (or just became dead/hidden)
void Enemy::EnemyErase(void) {
    if (prevIsAlive) {
        ST7735_FillRect(prevX, prevY - height + 1, width, height, ST7735_BLACK);
    }
}
 
// Draw only if alive right now
void Enemy::EnemyDraw(void) {
    if (isAlive) {
        ST7735_DrawBitmap(x, y, image, width, height);
    }
}
 
uint8_t Enemy::getIsAlive()     { return isAlive; }
uint8_t Enemy::getPrevIsAlive() { return prevIsAlive; }