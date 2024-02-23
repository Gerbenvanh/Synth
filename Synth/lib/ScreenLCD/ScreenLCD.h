
#pragma once


#include "Arduino.h"
#include <LiquidCrystal_I2C.h>


const int COLUMNS = 16;
const int ROWS = 2;
class ScreenLCD 
{
private:
    int _scl;
    int _sda;
    int _addr;
    LiquidCrystal_I2C _lcd;

public:
    ScreenLCD(int, int, int);
    void begin();
    void display(String, String = "");
    
};
