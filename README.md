# Winstar_16x2_LCD
This is the C++ library for WH1602B-NYG-JT made by Winstar Co Limited

# Example code
``` C++
#include "mbed.h"
#include "Winstar_16x2_LCD.h"

int main(){
    Winstar_16x2_LCD lcd(
                            LCD_DB0,
                            LCD_DB1,
                            LCD_DB2,
                            LCD_DB3,
                            LCD_DB4,
                            LCD_DB5,
                            LCD_DB6,
                            LCD_DB7,
                            LCD_EN,
                            LCD_RS
                        );
    lcd.WriteLine0("Hello World");
    lcd.WriteLine1("Test");
    while(true){
        
    }
}
```
