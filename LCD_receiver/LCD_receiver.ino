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

void loop() {
  uint8_t buf[10]={0};    //Set buffer equal to message recieved.
  uint8_t buflen = sizeof(buf);
  if(driver.recv(buf, &buflen))   //Valid message ->print
  {
    int i;
    Serial.print("Message: ");
    Serial.println((char*)buf);
    lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
    lcd.print("Distance: "); // Prints string "Distance" on the LCD
    lcd.print((char*)buf); // Prints the distance value from the sensor
    lcd.print(" XM  "); 
    delay(500);
  }
}

//SCL : A5
//SDA : A4
// data: D11
