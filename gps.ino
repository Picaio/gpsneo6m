
#include <Wire.h>
#include <LiquidCrystal_I2C.h>  
#include <TinyGPS++.h>

#define RXD2 16
#define TXD2 17
HardwareSerial neogps(1);
char   datoCmd = 0;

#define NMEA 0
#define I2C_ADDR    0x27  
LiquidCrystal_I2C lcd(I2C_ADDR, 2, 1, 0, 4, 5, 6, 7);

TinyGPSPlus gps;

//------------------------------------------------------------------------------------------------------------------------------------------

void setup() 
{
  Serial.begin(115200);
  neogps.begin(9600, SERIAL_8N1, RXD2, TXD2); 
  lcd.begin (16,2);                 // inicializar lcd 
  lcd.setBacklightPin(3,POSITIVE);  // Backlight OFF 
  lcd.setBacklight(HIGH);           // Backlight ON 
  lcd.clear();
  lcd.print(" PICAIO VIDEOS");  
  delay(2000);

}

//------------------------------------------------------------------------------------------------------------------------------------------

void loop() 
{

visualizacion_lcd ();
if (NMEA)
 {
    while (neogps.available())
    {
     datoCmd  = (char)neogps.read(); 
     Serial.print (datoCmd );
    }
 } 

else
 {
  boolean newData = false;
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (neogps.available())
    {
      if (gps.encode(neogps.read()))
      {
        newData = true;         
      }
    }
  }  

  if(newData == true)
  {
    newData = false;
    Serial.println(gps.satellites.value());    
    Visualizacion_Serial();
  }
  else
  {
    
  }  
  
 }
}

//------------------------------------------------------------------------------------------------------------------------------------------

void visualizacion_lcd (void)
{
 lcd.clear();
 lcd.print("LAT: ");
 lcd.print(gps.location.lat(),6);
 lcd.setCursor (0, 1);
 lcd.print("LON: "); 
 lcd.print(gps.location.lng(),6);  
 delay (2000);
 lcd.clear();
 lcd.print("VEL: ");
 lcd.print(gps.speed.kmph());
 lcd.print("Km/h");
 lcd.setCursor (0, 1);
 lcd.print("SAT: "); 
 lcd.print(gps.satellites.value());
 delay (2000);
 lcd.clear();
 lcd.print("ALT: ");
 lcd.print(gps.altitude.meters(), 0); 
 delay (2000);
 lcd.clear();
 lcd.print("HOR: "); 
 lcd.print(gps.time.hour());
 lcd.print(":");
 lcd.print(gps.time.minute());
 lcd.print(":");
 lcd.print(gps.time.second());
 lcd.setCursor (0, 1);
 lcd.print("FEC: "); 
 lcd.print(gps.date.day());
 lcd.print("/");
 lcd.print(gps.date.month());
 lcd.print("/");
 lcd.print(gps.date.year()); 
 delay (2000);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void Visualizacion_Serial(void)
{ 
   
  if (gps.location.isValid() ==  1)
  {  
    Serial.print("Lat: ");
    Serial.println(gps.location.lat(),6);
    Serial.print("Lng: ");
    Serial.println(gps.location.lng(),6);  
    Serial.print("Speed: ");
    Serial.println(gps.speed.kmph());    
    Serial.print("SAT:");
    Serial.println(gps.satellites.value());
    Serial.print("ALT:");   
    Serial.println(gps.altitude.meters(), 0);     

    Serial.print("Date: ");
    Serial.print(gps.date.day()); Serial.print("/");
    Serial.print(gps.date.month()); Serial.print("/");
    Serial.println(gps.date.year());

    Serial.print("Hour: ");
    Serial.print(gps.time.hour()); Serial.print(":");
    Serial.print(gps.time.minute()); Serial.print(":");
    Serial.println(gps.time.second());
    Serial.println("---------------------------");
  }
  else
  {
    Serial.println("Sin señal gps");  
  }  

}
