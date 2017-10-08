
// Testing script for MCP + relay modules.
// pins 15~17 to GND, I2C bus address is 0x20
// THE BELOW WORKS!!!! REQUIRES EXTERNAL BIAS ON ALL INPUT PINS
// Running this on each MCP seems to help... for unknown reasons...

#include "Wire.h"

byte OUTT = 0;
byte inputs=0;

#define GPPUA    0x0C


void setup()
{
 Serial.begin(9600);

 Wire.begin(); // wake up I2C bus

  Wire.beginTransmission(0x20); // select multiplexer 
  Wire.write(0x00); // IODIRA register
  Wire.write(0xFF); // set all of port A to intputs
  Wire.endTransmission();

  Wire.beginTransmission(0x20); // select multiplexer 
  Wire.write(0x01); // IODIRB register
  Wire.write(0x00); // set all of port b to OUT
  Wire.endTransmission();
   
  Wire.beginTransmission(0x20); 
  Wire.write(0x16); // DEFVALA (pull-up) register (if this doesnt work the hall effect won't work, outputs should read +5v)
  // note: if errors try 00x16*,0x06, 0x0C, and 0x0D
  Wire.write(0xFF); // turn on pull-ups
  Wire.endTransmission();

Serial.println("ok");

  
}
void loop()
{
 Wire.beginTransmission(0x20);
 Wire.write(0x12); // set MCP23017 memory pointer to GPIOB address
 Wire.endTransmission();
 Wire.requestFrom(0x20, 1); // request one byte of data from MCP20317
 inputs=Wire.read(); // store the incoming byte into "inputs"
 
 if (inputs>0) // if a button was pressed
 {
 Serial.print(inputs, BIN); // display the contents of the GPIOB register in binary
 inputs = 0;
 delay(300); 
 }

  Wire.beginTransmission(0x20);
  Wire.write(0x13); // IODIRA register
  Wire.write(~OUTT); // set all of port b to OUT
  Wire.endTransmission();


  Serial.print(" Out: ");
  Serial.println(OUTT, BIN);

  OUTT++;
 
}

