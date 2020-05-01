//Libraries
#include <DHT.h>;
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//Constants
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 4800;
// The TinyGPS++ object
TinyGPSPlus gps;


//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value
float tempF;

void setup()
{
  Serial.begin(9600);
  dht.begin();
  lcd.begin();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
}

void loop()
{
    //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    tempF=dht.convertCtoF(temp);
    
    //Print temp and humidity values to serial monitor

    printLCD(temp,tempF,hum);

    

}

   void printLCD(temp,tempF,hum){

    /* GPS Altitude */
    lcd.setCursor(0,0);
    lcd.print("Fady");

    /* GPS Distance to House */
    lcd.setCursor(0,1);
    lcd.print("Safi");

    /* GPS Lat Long */
    lcd.setCursor(0,2);
    lcd.print("Kosom");

    /* Humidity and Temperature */
    lcd.setCursor(0,3);
    lcd.print("T: ");
    lcd.setCursor(3,3);
    lcd.print(temp,10);
    lcd.setCursor(5,3);
    lcd.print("C/");
    lcd.setCursor(7,3);
    lcd.print(tempF,10);
    lcd.setCursor(9,3);
    lcd.print("F, Hum: ");
    lcd.setCursor(16,3);
    lcd.print(hum,10);
    lcd.setCursor(18,3);
    lcd.print("%");
  
    }
