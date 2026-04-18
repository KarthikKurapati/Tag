#ifndef JOYSTICK_H
#define JOYSTICK_H
#include <stdio.h>
#include <stdint.h>
class Joystick{
    private:
        uint32_t dataX;
        uint32_t dataY;
        uint32_t center;
        uint32_t deadzone;
        int32_t flag;
    public:
    Joystick(uint32_t center_val, uint32_t deadzone_val);
    void Init(void);
    void In(void);
    uint32_t getX(void);
    uint32_t getY(void);
    int32_t getXChange(void);
    int32_t getYChange(void);
};

#endif