#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ThingerWifi.h>
#include <ThingerESP8266.h>
#include <LiquidCrystal_I2C.h>

#define USERNAME "ahmadlufiau"
#define DEVICE_ID "SmartStove"
#define DEVICE_CREDENTIAL "SmartStove2019"
#define SSID "Lek2019"
#define SSID_PASSWORD "tuleswaesaja450"

#define MQ5_PIN A0            // SENSOR GAS
const int buz_pin     = D7;   // BUZZER

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);


// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

unsigned long oldTime = 0;

bool showLedAlarm = true; 

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

void setup() {
  thing.add_wifi(SSID, SSID_PASSWORD);
  Serial.begin(115200);
 
  lcd.init();
  lcd.begin(16, 2);
  lcd.print("MONITORING");
  lcd.setCursor(0, 1);
  lcd.print("KEBOCORAN GAS");
  
  pinMode(MQ5_PIN, INPUT); // SENSOR GAS
  pinMode(buz_pin, OUTPUT); // BUZZER
  
// resource output example (i.e. reading a sensor value)
  thing["Gas MQ-2"] >> outputValue(analogRead(A0));

  // Control Buzeer
    thing["Control Buzzer"] << [](pson& out){
      if(out.is_empty()){
          // We send back the pin value to thinger platform
          out = (bool) digitalRead(buz_pin);
      }
      else{
          digitalWrite(buz_pin, out ? HIGH : LOW);
      };
    };
  
// turn on LCD backlight                      
  lcd.backlight();
}

void loop() {
  thing.handle();  
  
  int dataLpg = analogRead(MQ5_PIN); //ranges from 0 to 1024
  Serial.print(dataLpg);
  Serial.print("\n");
  lcd.setCursor(0, 0);
  lcd.print(dataLpg,DEC); 
      if (dataLpg < 500)
      {
        Serial.println("Gas Tidak Bocor");
        lcd.setCursor(0,0);
        lcd.print("Gas Tidak Bocor");
        digitalWrite(buz_pin, LOW); 
        lcd.setCursor(0,1);
        lcd.print("Gas PPM: ");
        lcd.print(dataLpg);
        lcd.setCursor(12, 1);
        lcd.print("/Sec");
        delay(1000);
        lcd.clear();
      } else
      {
        //lcd.clear();
        Serial.println("Gas Sedang Bocor");
        lcd.setCursor(0,0);
        lcd.print("Gas Sedang Bocor");
        digitalWrite(buz_pin, HIGH); 
        lcd.setCursor(0,1);
        lcd.print("Gas PPM: ");
        lcd.print(dataLpg);
        lcd.setCursor(12, 1);
        lcd.print("/Sec");
        delay(1000);
        lcd.clear();
      }
}
