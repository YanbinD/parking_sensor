
/*
    UNO:  http://arduino.cc/en/Hacking/PinMapping168
    NANO: (matches UNO but has fewer pins)
    MEGA 2560: http://arduino.cc/en/Hacking/PinMapping2560
*/
#define SCL_PIN 5              //Default SDA is Pin5 PORTC for the UNO -- you can set this to any tristate pin
#define SCL_PORT PORTC 
#define SDA_PIN 4              //Default SCL is Pin4 PORTC for the UNO -- you can set this to any tristate pin
#define SDA_PORT PORTC
#define I2C_TIMEOUT 100        //Define a timeout of 100 ms -- do not wait for clock stretching longer than this time
#define SENSOR_COUNT 4         // number of sensors used in the sensor array 
/*
I have included a couple of extra useful settings for easy reference.
//#define I2C_CPUFREQ (F_CPU/8)//Useful if you plan on doing any clock switching
#define I2C_FASTMODE 1         //Run in fast mode (400 kHz)
#define I2C_SLOWMODE 1         //If you do not define the mode it will run at 100kHz with this define set to 1 it will run at 25kHz
*/
#include <SoftI2CMaster.h>     //You will need to install this library
#include <RH_ASK.h>   //Has been added for the transmitter to work
#include <SPI.h>    //not used, but needed for the RH_ASK.h library to work
RH_ASK driver;    //Object -driver

byte addresses[SENSOR_COUNT] = {200, 202, 204, 224};
int sensorReading[SENSOR_COUNT]; 


void setup(){
  // Initialize both the serial and I2C bus
  Serial.begin(9600);
  i2c_init();
  // (OPTIONAL) Check each address for a sensor
  address_polling_example();
  
  //  default_address_change_example();  

  // Your code here
  trans_setup();    //Has been added for the transmit debugger
  
}


void loop()
{
  // (OPTIONAL) Read a sensor at the default address
//  int value = read_the_sensor_example();   //Has been added to save sensor as a value 
  mux_read_sensor();
  delay(50); // experiment with different value
  for (int i = 0 ; i < SENSOR_COUNT; i++) {
    trans_data(sensorReading[i], i);
  }
//  trans_data(value);   //Has been added to transmit value as a message to the reciever  
   
  // Your code here
}


void mux_read_sensor() {
    boolean error = 0;
    int range; 
    
    for (int i = 0; i < SENSOR_COUNT; i++) {
        error = start_sensor(addresses[i]);
        if (!error) {
            sensorReading[i] = read_sensor(addresses[i]);
             Serial.print(i);
             Serial.print(" : ");
             Serial.print(sensorReading[i]);
             Serial.print("\n");
        }
        delay(100); // experiment with different value
    }
}


///////////////////////////////////////////////////
// Function: Start a range reading on the sensor //
///////////////////////////////////////////////////
boolean start_sensor(byte bit8address){
  boolean errorlevel = 0;
  bit8address = bit8address & B11111110;               //Do a bitwise 'and' operation to force the last bit to be zero -- we are writing to the address.
  errorlevel = !i2c_start(bit8address) | errorlevel;   //Run i2c_start(address) while doing so, collect any errors where 1 = there was an error.
  errorlevel = !i2c_write(81) | errorlevel;            //Send the 'take range reading' command. (notice how the library has error = 0 so I had to use "!" (not) to invert the error)
  i2c_stop();
  return errorlevel;
}


///////////////////////////////////////////////////////////////////////
// Function: Read the range from the sensor at the specified address //
///////////////////////////////////////////////////////////////////////
//Uses the I2C library to read a sensor at the given address
//Collects errors and reports an invalid range of "0" if there was a problem.
//INPUTS: byte  bit8address = the address of the sensor to read from
//OUPUTS: int   range = the distance in cm that the sensor reported; if "0" there was a communication error
int read_sensor(byte bit8address){
  boolean errorlevel = 0;
  int range = 0;
  byte range_highbyte = 0;
  byte range_lowbyte = 0;
  bit8address = bit8address | B00000001;  //Do a bitwise 'or' operation to force the last bit to be 'one' -- we are reading from the address.
  errorlevel = !i2c_start(bit8address) | errorlevel;
  range_highbyte = i2c_read(0);           //Read a byte and send an ACK (acknowledge)
  range_lowbyte  = i2c_read(1);           //Read a byte and send a NACK to terminate the transmission
  i2c_stop();
  range = (range_highbyte * 256) + range_lowbyte;  //compile the range integer from the two bytes received.
  if(errorlevel){
    return 0;
  }
  else{
    return range;
  }
}


//=================== Wireless section ===================
// data pin to pin11
//////////////////////////////////////////////
//               Transmitter Setup          //
//////////////////////////////////////////////
//
void trans_setup(){
  //Serial.begin(9600);   //Debugger- no need for double Serial.begin(9600)
  if(!driver.init())
    Serial.println("init failed");
}

struct data_packages {
  char measurement[3];
  int sensor_number; 
};


//////////////////////////////////////////////
//              Transmit data               //
//////////////////////////////////////////////
void trans_data(int msg, int sensor_number){
  Serial.print("Transmit fcn test ");   //Test to see if the compiler is stepping through the fcn
  char measurement [10]={0};    //char array
  itoa(msg, measurement, 10);   //convert int to char array

  struct data_packages data_package; 
  itoa(msg, data_package.measurement, 10);
  Serial.println(data_package.measurement);
  
//  Serial.println(sensor_no);    //Test to see if the itoa fcn is working
  
  const char *msg2 = measurement;   //Message to be sent  
  driver.send((uint8_t*)msg2, strlen(msg2));    //Send Message
  driver.waitPacketSent();
}


////////////////////////////////////////////////////////////////
// Code Example: Poll all possible addresses to find a sensor //
////////////////////////////////////////////////////////////////
void address_polling_example(){
  boolean error = 0;  //Create a bit to check for catch errors as needed.
  int range = 0;
  Serial.println("Polling addresses...");
 
  //Walk through all possible addresses and check for a device that can receive the range command and will
  //    return two bytes.
  for (byte i=2; i!=0; i+=2){   //start at 2 and count up by 2 until wrapping to 0. Checks all addresses (2-254) except 0 (which cannot be used by a device)
    error = 0;
    error = start_sensor(i);    //Start the sensor and collect any error codes.
    if (!error){                //If you had an error starting the sensor there is little point in reading it.
      delay(100);
      range = read_sensor(i);   //reading the sensor will return an integer value -- if this value is 0 there was an error
      Serial.println(i);
      if (range != 0){
        Serial.print("Device found at:");Serial.print(i);Serial.print(" Reported value of:");Serial.println(range);
      }  
    }
    else{
      Serial.print("Couldn't start:");Serial.println(i);
    }
  }

  Serial.println("Address polling complete.");
}
