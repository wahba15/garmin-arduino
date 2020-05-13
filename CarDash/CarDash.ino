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
  printLCDFixedText();
}

void loop()
{
  timer=millis();

  hum = dht.readHumidity();
  temp= dht.readTemperature();
  tempF=((temp*9)/5)+32;


  altitudeGPSft=gps.altitude.meters()*3.280839895;
  speedGPSmph=gps.speed.mph();
  latitudeGPS=gps.location.lat();
  longitudeGPS=gps.location.lng();
  distanceMiToHome=TinyGPSPlus::distanceBetween(latitudeGPS, longitudeGPS, HOME_LAT, HOME_LON)/1609.344;
 
  if (timer-timerIF>500) {
   
    printLCDDistAlt(altitudeGPSft,0);
    printLCDDistSpeed(speedGPSmph,distanceMiToHome,1);
    printLCDLatLong(latitudeGPS,longitudeGPS,2); 
    if (!isnan(temp)) {
      printLCDtemp(temp,tempF,hum,3);
    }
    timerIF=millis();
  }
  
  smartDelay(100);
}

void printLCDDistAlt(float alt, int line){

    /* GPS Altitude */
  lcd.setCursor(10,line);
  lcd.print(round(alt),1);

}

void printLCDDistSpeed(float spd,float dist, int line){

    /* GPS Distance to House and Speed*/
  lcd.setCursor(2,line);
  if (spd<10) {
    lcd.print(spd,1);
  }
  else if (spd>10 & spd<100){
    lcd.print(int(spd));
    lcd.setCursor(4,line);
    lcd.print(" ");
  }
  else  {
    lcd.print(int(spd));
  }
  
  lcd.setCursor(14,line);
  if (dist<10){
    lcd.print(dist,2);  
  }
  else if (dist>10 & dist<100){
    lcd.print(dist,1);
  }
  else  {
    lcd.print(int(dist));
  }
}

void printLCDLatLong(float lat,float lon, int line){

        /* GPS Lat Long */
  lcd.setCursor(0,line);
  lcd.print(lat,6);
  lcd.setCursor(9,line);
  lcd.print(lon,6);


  
}

void printLCDtemp(float temp,float tempF,float hum, int line){

    /* Humidity and Temperature */
  if (temp>-10){
    lcd.setCursor(3,line);
    lcd.print(int(temp));
    lcd.setCursor(5,3);
    lcd.print("C/");
    lcd.setCursor(7,line);
    lcd.print(int(tempF));
    if (tempF<100)  {
      lcd.setCursor(9,line);
      lcd.print(" ");  
    }
    lcd.setCursor(17,line);
    lcd.print(int(hum));
  }
  else {
    lcd.setCursor(3,line);
    lcd.print(int(temp));
    lcd.setCursor(6,3);
    lcd.print("C/");
    lcd.setCursor(8,line);
    lcd.print(int(tempF));
    lcd.setCursor(17,line);
    lcd.print(int(hum));
    
  }

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

void printLCDFixedText(){

    /* GPS Altitude */
  lcd.setCursor(0,0);
  lcd.print("Altitude: ");
  lcd.setCursor(15,0);
  lcd.print("ft");

 /* GPS Distance to House and Speed*/
  lcd.setCursor(0,1);
  lcd.print("V:");
  lcd.setCursor(5,1);
  lcd.print("mph,Home:");
  lcd.setCursor(18,1);
  lcd.print("mi"); 

/* Humidity and Temperature */
  lcd.setCursor(0,3);
  lcd.print("T: ");
  lcd.setCursor(10,3);
  lcd.print((char)223);
  lcd.setCursor(11,3);
  lcd.print("F,Hum:");
  lcd.setCursor(19,3);
  lcd.print("%");
}
