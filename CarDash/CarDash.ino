//Libraries
#include <DHT.h>
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
static const double HOME_LAT = 32.382788, HOME_LON = -110.962640;
// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value
float tempF;
double altitudeGPSft;
double speedGPSmph;
double latitudeGPS;
double longitudeGPS;
double distanceMiToHome;
double courseToHome;
unsigned long timer;
unsigned long timerIF=0;

void setup()
{
//  Serial.begin(115200);
   ss.begin(GPSBaud);
  dht.begin();
  lcd.begin();                      // initialize the lcd 
 smartDelay(1000);
  // Print a message to the LCD.
//  lcd.backlight();
}

void loop()
{
    timer=millis();


    hum = dht.readHumidity();
    temp= dht.readTemperature();
    tempF=dht.convertCtoF(temp);

    altitudeGPSft=gps.altitude.meters()*3.280839895;
    speedGPSmph=gps.speed.mph();
    latitudeGPS=gps.location.lat();
    longitudeGPS=gps.location.lng();
//    Serial.println(altitudeGPSft);
    distanceMiToHome=TinyGPSPlus::distanceBetween(latitudeGPS, longitudeGPS, HOME_LAT, HOME_LON)/1609.344;

    
    if (timer-timerIF>500) {
       
      printLCDDistAlt(altitudeGPSft,0);
      printLCDDistSpeed(speedGPSmph,distanceMiToHome,1);
      printLCDLatLong(latitudeGPS,longitudeGPS,2); 
      printLCDtemp(temp,tempF,hum,3);
      timerIF=millis();
    }
    
    smartDelay(100);
//    Serial.println("Testing Frequency");

}

void printLCDDistAlt(float alt, int line){

    /* GPS Altitude */
    lcd.setCursor(0,line);
    lcd.print("Altitude: ");
    lcd.setCursor(10,line);
    lcd.print(round(alt),1);
    lcd.setCursor(15,line);
    lcd.print("ft");


}

void printLCDDistSpeed(float spd,float dist, int line){

    /* GPS Distance to House and Speed*/
    lcd.setCursor(0,line);
    lcd.print("V:");
    lcd.setCursor(2,line);
    if (spd<10) {
      lcd.print(spd,1);
      lcd.setCursor(5,line);
      lcd.print("mph,Home:");
    }
    else {
      lcd.print(round(spd),1);
      lcd.setCursor(4,line);
      lcd.print(" mph,Home:");
    }
    lcd.setCursor(5,line);
    lcd.print("mph,Home:");
    lcd.setCursor(14,line);
    if (dist<100){
      lcd.print(dist,1);
      lcd.setCursor(18,line);
      lcd.print("mi");}
    else  {
      lcd.print(int(round(dist)));
      lcd.setCursor(18,line);
      lcd.print("mi"); 
    }


}

void printLCDLatLong(float lat,float lon, int line){

        /* GPS Lat Long */
//    lcd.setCursor(0,line);
//    lcd.print("(");
//    lcd.setCursor(1,line);
//    lcd.print(lat,10);
//    lcd.setCursor(10,line);
//    lcd.print(",");
//    lcd.setCursor(11,line);
//    lcd.print(lon,10);
//    lcd.setCursor(19,line);
//    lcd.print(")");

//    lcd.setCursor(0,line);
//    lcd.print("(");
    lcd.setCursor(0,line);
    lcd.print(lat,6);
//    lcd.setCursor(10,line);
//    lcd.print(",");
    lcd.setCursor(9,line);
    lcd.print(lon,6);
//    lcd.setCursor(19,line);
//    lcd.print(")");

  
}

void printLCDtemp(float temp,float tempF,float hum, int line){

    /* Humidity and Temperature */
    lcd.setCursor(0,line);
    lcd.print("T: ");
    lcd.setCursor(3,line);
    lcd.print(int(round(temp)));
    lcd.setCursor(5,line);
    lcd.print("C/");
    lcd.setCursor(7,line);
    lcd.print(int(round(tempF)));
    lcd.setCursor(9,line);
    lcd.print("F, Hum: ");
    lcd.setCursor(16,line);
    lcd.print(int(round(hum)));
    lcd.setCursor(18,line);
    lcd.print("% ");

    }


// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
