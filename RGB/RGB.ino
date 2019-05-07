/*
Adafruit Arduino - Lesson 3. RGB LED
*/
 
int redPin = 11;
int greenPin = 10;
int bluePin = 9;
 
int s0 = 2;
int s1 = 3;
int s2 = 4;
int s3 = 5;

//uncomment this line if using a Common Anode LED
#define COMMON_ANODE
 
void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
}
 
void loop()
{
  for (int i = 2; i < 6; i++) {
    // digitalWrite(i, HIGH);
    setColor(255, 0, 0); // red
    delay(5);
    setPinHigh(i);
  }
}
//   setColor(255, 0, 0);  // red
//   delay(200);
//   setColor(0, 255, 0);  // green
//   delay(200);
//   setColor(0, 0, 255);  // blue
//   delay(200);
//   setColor(255, 255, 0);  // yellow
//   delay(200);  
//   setColor(80, 0, 80);  // purple
//   delay(200);
//   setColor(0, 255, 255);  // aqua
//   delay(200);
// }

void setPinHigh(int pin) {
  for (int i = 2; i < 6; i++)
  {
    digitalWrite(i, LOW);
    // delay(10);
  }
  digitalWrite(pin, HIGH);
}
 
void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
