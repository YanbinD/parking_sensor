#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compil
#define SENSOR_COUNT 4  
#define COMMON_ANODE
int R = 6;
int G = 5;
int B = 3;

int SENSOR_0 = 2;
int SENSOR_1 = 4;
int SENSOR_2 = 7;
int SENSOR_3 = 8;

RH_ASK driver;
int readings[SENSOR_COUNT] = {220, 220, 220, 220};
int currentPtr = 0;
 
void setup()
{
    Serial.begin(9600); // Debugging only
    setPinMode();
    if (!driver.init())
        Serial.println("init failed");
}

void loop()
{
    uint8_t buf[4];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
        int i;
        // Message with a good checksum received, dump it.
        Serial.print("Message: ");
        int reading = atoi(buf);
        int s_reading = reading % 1000;
        int s_number = reading / 1000;
        Serial.println("s_number");
        Serial.println(s_number);
        Serial.println("s_reading");
        Serial.println(s_reading);

        if (s_reading > 140)
        {
            // green
            Serial.println("==> 140");
            setColor(0, 255, 0);
        }
        else if (s_reading > 100)
        {
            // yellow
            Serial.println("==> 100");
            setColor(205, 255, 0);
        }
        else if (s_reading > 50)
        {
            // organge
            Serial.println("==> 50");
            setColor(255, 128, 0);
        } else {
            Serial.println("< 50");
            setColor(255, 0, 0);
        }
        reading_to_led(s_number);
    }
}

void setPinMode()
{
    pinMode(R, OUTPUT);
    pinMode(G, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(SENSOR_0, OUTPUT);
    pinMode(SENSOR_1, OUTPUT);
    pinMode(SENSOR_2, OUTPUT);
    pinMode(SENSOR_3, OUTPUT);
}

void reading_to_led(int s_number)
{
    digitalWrite(2, LOW);
    digitalWrite(4, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);

    if (s_number == 1)
    {
        // Serial.println("===>sensor_0");
        digitalWrite(SENSOR_0, HIGH);
    }
    else if (s_number == 2)
    {
        // Serial.println("===>sensor_1");
        digitalWrite(SENSOR_1, HIGH);
    }
    else if (s_number == 3)
    {
        // Serial.println("==>sensor_2");
        digitalWrite(SENSOR_2, HIGH);
    }
    else if (s_number == 4)
    {
        // Serial.println("===>sensor_3");
        digitalWrite(SENSOR_3, HIGH);
    }
}

void setColor(int r, int g, int b)
{
#ifdef COMMON_ANODE
    r = 255 - r;
    g = 255 - g;
    b = 255 - b;
#endif
    analogWrite(R, r);
    analogWrite(G, g);
    analogWrite(B, b);
}