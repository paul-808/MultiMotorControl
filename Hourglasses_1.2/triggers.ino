

void triggers(){
  // This function cycles through all 8 possible MCP addresses
  // Records the readings from each, reports them to serieal,
  // Records their values in an array, and then refers to that 
  // array to set corresponding outputs. These are debounced,. 
  // and turned off after 10 seconds if they don't otherwise turn off


// http://tronixstuff.com/2011/08/26/tutorial-maximising-your-arduinos-io-ports/
// http://www.unifiedmicro.com/IOX-16_Input_Demo.txt
// https://www.mathsisfun.com/binary-decimal-hexadecimal-converter.html
// http://playground.arduino.cc/Code/BitMath


 int isaddress = 0;
 
 // read values From each input
 for(int i = 0; i < 8; i++){
  int MCPid = 0x20 + i;
 Wire.beginTransmission(MCPid); //select 1st MCP "000"
 Wire.write(0x12); // set MCP23017 memory pointer to GPIOa address
 int staatus = Wire.endTransmission();  //transmission status from i2c bus (0 = success)
 
 if(staatus == 0){
   Wire.requestFrom(MCPid, 1); // request one byte of data from MCP20317
   inputs=Wire.read(); // store the incoming byte into "inputs"
   if(testmode == 1){
     Serial.print("input series ");
     Serial.print(MCPid-31); //print MCP number from 1-8
     Serial.print(": ");
     Serial.print(inputs, BIN); // display the contents of the GPIOB register in binary
     Serial.print("  at system time (ms): "); // time report for testing/debugging
     Serial.println(millis()); // time report for testing/debugging    
     }
   }
 else {
    if(testmode == 1){
      Serial.print("input series ");
      Serial.print(MCPid-31);
      Serial.println(": is not connected");
    }
  }
 }
 
 for(int i = 0; i < 8; i++){
  int MCPid = 0x20 + i;  
   Wire.beginTransmission(MCPid); //select 1st MCP "000"
   Wire.write(0x13); // set MCP23017 memory pointer to GPIOb address

   // TODO:
   // calcultate new value for outputs;
   // if BIT == 0, first, check if seconds elapsed % frequency is 0;
   // Then check the last time it activated, if difference is greater than 5s, corresponding bit == 1
   // if BIT == 1, check if inputs == 1, and if so, switch to 0;
   // OR if seconds elapsed since last flip > maxactive, switch to 0;
   
   Wire.write(outputs);
   Wire.endTransmission();  //transmission status from i2c bus (0 = success)
   if(testmode == 1){
     Serial.print("output series ");
     Serial.print(MCPid-31); //print MCP number from 1-8
     Serial.print(": ");
     Serial.print(outputs, BIN); // display the contents of the GPIOB register in binary
     Serial.print("  at system time (ms): "); // time report for testing/debugging
     Serial.println(millis()); // time report for testing/debugging 
   }
  }
  outputs = ~outputs; // reverse all outputs for testing purposes
}

