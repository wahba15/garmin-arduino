#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.begin();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Fady");
  lcd.setCursor(0,1);
  lcd.print("Safi");
    lcd.setCursor(0,2);
  lcd.print("Kosom");
    lcd.setCursor(0,3);
  lcd.print("El mohem ya3ny");
}


void loop()
{
}
