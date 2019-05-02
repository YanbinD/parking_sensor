#include <RH_ASK.h>
#include <SPI.h>
//SPI
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

RH_ASK driver;    //Object -driver
// define the 1602 LCD 
LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x3F for a 16 chars and 2 line display


void setup() {
  Serial.begin(9600);   //Debugger
  if(!driver.init()) {
     Serial.println("init failed");
  }
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
}

struct data_packages {
  char measurement[3];
  int sensor_number; 
};



void loop() {
 
  uint8_t buf[6];
  uint8_t buflen = sizeof(buf);
    
  if(driver.recv((uint8_t*)buf, &buflen)) 
  {
    int i;
   lcd.print("      ");
    if (buf[0] == '0') {
        lcd.setCursor(0,0);

    }
    else if (buf[0] == '1') {
        lcd.setCursor(6,0);
    }
    else if (buf[0] == '2') {
        lcd.setCursor(0,1);
    }
    else {
        lcd.setCursor(6,1);
    }
    lcd.print((char *) buf);
    Serial.println((char*)buf);
    
  
//    Serial.print(" Distance: ");
//    Serial.println((char*)buf);
//    lcd.setCursor(0,1); // Sets the location at which subsequent text written to the LCD will be displayed
//    lcd.print("Distance:"); // Prints string "Distance" on the LCD
//    lcd.print((char*)buf); 
//    lcd.print(" cm  ");
    
    delay(20);
//    lcd.clear();
  }
}

//SCL : A5
//SDA : A4
// data: D11
