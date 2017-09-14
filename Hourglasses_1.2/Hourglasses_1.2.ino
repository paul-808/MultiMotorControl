/*********************


Thanks to the many people whose code has inspired this piece:
Mikal Hart (TinyGPS and testing scripts), 


 *********************/

#include <SoftwareSerial.h>
#include <TinyGPS.h>

SoftwareSerial mySerial(7, 8); // RX PIN = 7// TX PIN = 8
TinyGPS gps;

void gpsdump(TinyGPS &gps);
void localTime(TinyGPS &gps);
void printFloat(double f, int digits = 2);
void secondsToday(TinyGPS &gps);
void triggers();

unsigned long start = 0;
unsigned long initiate = 0;
unsigned long secondsTotal;

uint32_t frequency[128][3] = {
// ID, controller Number, frequency (in seconds) 
  {1,1,72055},
  {2,1,22222},
  {3,1,33333},
  {4,1,44444},
  {5,1,55555},
  {6,1,66666},
  {7,1,77777},
  {8,1,88888}
  
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
  Serial.println("Data update in 10s..."); 
  Serial.print("Sizeof(gpsobject) = "); 
  Serial.println(sizeof(TinyGPS));
  Serial.println(); 

  
}

void loop() {


  bool newdata2 = false;
  while (millis() - initiate > 5000) // MS check freq!
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
    newdata2 = false;
    initiate = millis();
    Serial.println("-------------"); 
    //localTime(gps); // simple debug (uncomment only one)
    //gpsdump(gps); // complex debug (uncomment only one)
    secondsToday(gps);
    Serial.println(); 
    
  }

  
}




