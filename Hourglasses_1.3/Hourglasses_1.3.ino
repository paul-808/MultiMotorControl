
  ///////////////////////////////////////
  //     [[______________________]]"   //
  //     |(ლ(́◉◞౪◟◉‵ლ)tli;:.:. . )|"   //  
  //     | T^^^^^^^^^^^^^^^^^^^^T |"   //  
  //     | `;;:::.: .    .     .' |"   //  
  //     |  l;;:. ..  .     .: ;  |"   //  
  //     |  `;;::.. .    .  ; .'  |"   //  
  //     |    \;;:. .   .,'  /    |"   //  
  //     |     `\;:.. ..'  .'     |"   //  
  //     |         \;:. /         |"   //  
  //     |          `;f'          |"   //  
  //    0|===========||===========|0   //  
  //                 ;l."              //
  //               / ;  \ "            //
  //           ./` . :     `."         //
  //          /` ,`  :       \ "       //
  //        ,' ;  .  :        `."      //
  //        f ;  .   :      .  i"      //
  //       .'    :  ::       . `."     //
  //       |___,/;;:. . .`._____|"     //
  //      (QB0ZDOLC7ಥ‿ಥz!;:.:. .  )"    //
  //                                    //
  //    LETS FLIP SOME HOURGLASSES!!!   //
  //                                    //
  ////////////////////////////////////////

/*********************

V 1.0 Sept. 20 2017  by Paul Jarvey
Thanks to the many people whose code has inspired this piece:
Particularly Mikal Hart (TinyGPS and testing scripts)

 *********************/
 

 
#include <avr/pgmspace.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Wire.h>

SoftwareSerial mySerial(7, 8); // RX PIN = 7// TX PIN = 8
TinyGPS gps;

   //************************************************
   // Variable declaration
   //************************************************

void gpsdump(TinyGPS &gps);
void localTime(TinyGPS &gps);
void printFloat(double f, int digits = 2);
void secondsToday(TinyGPS &gps);
void triggers();


bool testmode = 1;
byte testpin = 12;

byte add1 = 9;
byte add2 = 10;
byte add3 = 11;
byte controlNumber = 0;
int HGID = 999;
byte ref = 254;
byte debouncetime = 3; // must be at least 2s, must be < maxactive

unsigned long start = 0;
unsigned long initiate = 0;
unsigned long secondsTotal = 0;
unsigned long millisAtTime = 0;
uint16_t counter = 0;
byte inputs = 0;
byte outputs = 0;
int maxactive = 10; // max period a motor will be active!!! safety limit!!

   //************************************************
   // Reference arrays !!MUST BE UPDATED!!
   //************************************************

// The last time the array/output was updated (seconds)
uint32_t HGupdate[128] = 
 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
;

// The last known array output status (boolean)
boolean HGstatus[128] = 
 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
;

//Actual frequency array ; useful for testing (currently contains dummy values)
const uint32_t PROGMEM HGfreq[128] = 
{3,4,5,6,7,8,3,4,15,12,13,14,15,16,17,18,
 6,7,3,4,5,6,9,8,9,2,8,4,5,6,7,8,
 6,9,3,4,5,6,7,8,1,2,3,8,5,8,7,5,
 8,7,8,4,5,6,7,5,9,8,3,4,8,5,7,8,
 6,9,3,4,5,6,9,8,1,2,8,4,5,8,7,8,
 8,7,8,4,5,6,7,5,9,2,3,8,5,6,5,8,
 7,9,3,4,5,6,7,8,1,8,3,4,8,6,7,5,
 7,7,3,4,5,6,9,5,9,2,8,4,5,8,5,8};   

// Reference array: which position is each HG id wired into?
// position = Controller*64 + slave*8 + position number (each indexed to 0)
// 1-128 hourglass number, 0-511 position index

const uint16_t PROGMEM refArray[128][2] = {
{0,0},
{1,1},
{2,2},
{3,3},
{4,4},
{5,5},
{6,6},
{7,7},
{8,8},
{9,9},
{10,10},
{11,11},
{12,12},
{13,130},
{14,140},
{15,150},
{16,160},
{17,17},
{18,18},
{19,19},
{20,20},
{21,21},
{22,22},
{23,23},
{24,24},
{25,25},
{26,26},
{27,27},
{28,28},
{29,29},
{30,30},
{31,31},
{32,32},
{33,33},
{34,34},
{35,35},
{36,36},
{37,37},
{38,38},
{39,39},
{40,40},
{41,41},
{42,42},
{43,43},
{44,44},
{45,45},
{46,46},
{47,47},
{48,48},
{49,49},
{50,50},
{51,51},
{52,52},
{53,53},
{54,54},
{55,55},
{56,56},
{57,57},
{58,58},
{59,59},
{60,60},
{61,61},
{62,62},
{63,63},
{64,64},
{65,65},
{66,66},
{67,67},
{68,68},
{69,69},
{70,70},
{71,71},
{72,72},
{73,73},
{74,74},
{75,75},
{76,76},
{77,77},
{78,78},
{79,79},
{80,80},
{81,81},
{82,82},
{83,83},
{84,84},
{85,85},
{86,86},
{87,87},
{88,88},
{89,89},
{90,90},
{91,91},
{92,92},
{93,93},
{94,94},
{95,95},
{96,96},
{97,97},
{98,98},
{99,99},
{100,100},
{101,101},
{102,102},
{103,103},
{104,104},
{105,105},
{106,106},
{107,107},
{108,108},
{109,109},
{110,110},
{111,111},
{112,112},
{113,113},
{114,114},
{115,115},
{116,116},
{117,117},
{118,118},
{119,119},
{120,120},
{121,121},
{122,122},
{123,123},
{124,124},
{125,125},
{126,126},
{127,127}
};

void setup()  
  
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  delay(1000);
  Serial.println("Booted... ");
  Serial.println("Serial ok..."); 
  // Inititate MCP Multiplexer

  for (int i = 0 ; i<8; i++){
  Wire.begin(); // wake up I2C bus
  Wire.beginTransmission(0x20+i); // select multiplexer 
  Wire.write(0x00); // IODIRA register
  Wire.write(0x01); // set all of port A to intputs
  Wire.endTransmission();
  Wire.beginTransmission(0x20+i); 
  Wire.write(0x16); // DEFVALA (pull-up) register (if this doesnt work the hall effect won't work, outputs should read +5v)
  // note: if errors try 0x06, 0xC, and 0xD
  Wire.write(0xFF); // turn on pull-ups
  Wire.endTransmission();
  Wire.beginTransmission(0x20+i);  
  Wire.write(0x01); // IODIRB register
  Wire.write(0x00); // set all of port B to outputs
  Wire.endTransmission();
  }
  Serial.println("Multiplexer ok..."); 
  
  Serial.print("This is Controller #");
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
     Serial.println("In TESTING/DEBUG (verbose&slow) mode.");
     Serial.println("Legend: ID:#|freq|status  999 = not used");
  }
    if(testmode == 0){
     Serial.println("In RUN/PRODUCTION (silent&fast) mode.");
  }
  // And wait....

  Serial.println();

}

void loop() {


  bool newdata2 = false;
  while (millis() - initiate > 100) // MS check freq!
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
    
    if(testmode == 1){Serial.println("---");}
      secondsToday(gps);
    if(testmode == 1){Serial.println("---");}
  
  }
    triggers();  //move this outside of the secondstoday loop into the main loop
}




