
#include "ScreenLCD.h"
#include <iostream>

/**
 * @brief Construct a new StationScreen::StationScreen object
 *
 * @param scl
 * @param sda
 */
ScreenLCD::ScreenLCD(
    int scl,
    int sda,
    int addr
    )
    : _scl(scl),
      _sda(sda),
      _addr(addr),
      _lcd(LiquidCrystal_I2C(_addr, COLUMNS, ROWS))
{
}

void ScreenLCD:: begin()
{
    
    _lcd.init();
    _lcd.backlight();
    _lcd.setCursor(0, 0);
}

/**
 * @brief Function for displaying StationScreen
 *
 * @param message Input message to be displayed on StationScreen
 * @return int for future unit testing implementation
 */
void ScreenLCD::display(String msg1, String msg2)
{
    _lcd.clear();
    _lcd.setCursor(0,0);
    _lcd.print(msg1);
    _lcd.setCursor(0,1);
    _lcd.print(msg2);
}

