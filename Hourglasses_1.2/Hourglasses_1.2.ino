/*********************


Thanks to the many people whose code has inspired this piece:
Mikal Hart (TinyGPS and testing scripts), 


 *********************/

#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Wire.h>

SoftwareSerial mySerial(7, 8); // RX PIN = 7// TX PIN = 8
TinyGPS gps;

void gpsdump(TinyGPS &gps);
void localTime(TinyGPS &gps);
void printFloat(double f, int digits = 2);
void secondsToday(TinyGPS &gps);
void triggers();


bool testmode = 1;
int testpin = 12;
int controlNumber = 0;

int add1 = 9;
int add2 = 10;
int add3 = 11;
int controlNo = 0;

unsigned long start = 0;
unsigned long initiate = 0;
unsigned long secondsTotal;
uint16_t counter = 0;
uint8_t inputs = 0;
uint8_t outputs = 0;
int maxactive = 10; // max period a motor will be active!!! safety limit!!

uint32_t frequency[128][4] = {
// ID, controller Number, frequency (in seconds), last flip timestamp (seconds)
  {1,1,1,0},
  {2,1,2,0},
  {3,1,3,0},
  {4,1,4,0},
  {5,1,5,0},
  {6,1,6,0},
  {7,1,7,0},
  {8,1,8,0}
};



void setup()  
  
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  delay(1000);
  Serial.println("Arduino Booted... ");
  Serial.println("Serial channels initated..."); 
  // Inititate MCP Multiplexer
  Wire.begin(); // wake up I2C bus
  Wire.beginTransmission(0x20); // select multiplexer "000"  TODO: Insert selector here!
  Wire.write(0x00); // IODIRA register
  Wire.write(0x01); // set all of port A to intputs
  Wire.endTransmission();
  Wire.beginTransmission(0x20);  // select multiplexer "000"  TODO: Insert selector here!
  Wire.write(0x01); // IODIRB register
  Wire.write(0x00); // set all of port B to outputs
  Wire.endTransmission();
  Serial.println("Multiplexer initiated..."); 
  Serial.print("This is Control Unit # ");
    pinMode(add1,INPUT);
    pinMode(add2,INPUT); 
    pinMode(add3,INPUT);
    controlNumber = digitalRead(add3) ;
    controlNumber = (controlNumber << 1) | digitalRead(add2) ;
    controlNumber = (controlNumber << 1) | digitalRead(add1) ;
    Serial.println(controlNumber, DEC);
    
  // Debug GPS buffer messages
  // Serial.print("Sizeof(gpsobject) = "); 
  // Serial.println(sizeof(TinyGPS));
  // Check for debugging mode
  pinMode(testpin,INPUT);
  testmode = digitalRead(testpin);
  if(testmode == 1){
     Serial.println("This unit is in TESTING/DEBUG mode.");
     Serial.println("Performance may be limited by serial writes.");
  }
    if(testmode == 0){
     Serial.println("This unit is in RUN/PRODUCTION mode.");
     Serial.println("Status messages will not be written to serial.");
  }
  // And wait....
  Serial.println("HourglassTron INITIATE");   
  Serial.println();
  /*
  Serial.println(" [[______________________]]");  
  Serial.println(" |(ლ(́◉◞౪◟◉‵ლ)tli;:.:. .  )|");  
  Serial.println(" | T^^^^^^^^^^^^^^^^^^^^T |");  
  Serial.println(" | `;;:::.: .    .     .' |");  
  Serial.println(" |  l;;:. ..  .     .: ;  |");  
  Serial.println(" |  `;;::.. .    .  ; .'  |");  
  Serial.println(" |    \;;:. .   .,'  /     |");  
  Serial.println(" |     `\;:.. ..'  .'      |");  
  Serial.println(" |         \;:. /          |");  
  Serial.println(" |          `;f'          |");  
  Serial.println("0|===========||===========|0");  
  Serial.println("             ;l.");  
  Serial.println("           / ;  \ ");  
  Serial.println("       ./` . :     `.");  
  Serial.println("      /` ,`  :       \ ");  
  Serial.println("    ,' ;  .  :        `.");  
  Serial.println("    f ;  .   :      .  i");  
  Serial.println("   .'    :  ::       . `.");  
  Serial.println("   |___,/;;:. . .`._____|");  
  Serial.println("  (QB0ZDOLC7ಥ‿ಥz!;:.:. .  )");  
  */

}

void loop() {


  bool newdata2 = false;
  while (millis() - initiate > 10000) // MS check freq!
  {
    if (mySerial.available()) 
    {
      char c = mySerial.read();
      if (gps.encode(c)) 
      {
        newdata2 = true;
        break;  
        
      }

    }

  }
  if (newdata2) 
  {
    counter = 0;
    newdata2 = false;
    initiate = millis();
    
    if(testmode == 1){Serial.println("-------------");}
      secondsToday(gps);
    if(testmode == 1){Serial.println("-------------");}
      triggers();
  }
}




