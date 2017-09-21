

void triggers() {
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

  for (int i = 0; i < 8; i++) { //cycle through slaves (64!)
    int MCPid = 0x20 + i;

    //************************************************
    // First, check the current status of this slave
    //************************************************

    Wire.beginTransmission(MCPid); //select 1st MCP "000"
    Wire.write(0x12); // set MCP23017 memory pointer to GPIOa address
    int staatus = Wire.endTransmission();  //transmission status from i2c bus (0 = success)

    if (testmode == 1) {
      Serial.print("S.");
      Serial.print(MCPid - 31); //print MCP number from 1-8
      Serial.print(" read: ");
    }
    if (staatus == 0) {
      Wire.requestFrom(MCPid, 1); // request one byte of data from MCP20317
      inputs = Wire.read(); // store the incoming byte into "inputs"
      if (testmode == 1) {
        Serial.print(inputs, BIN); // display the contents of the GPIOB register in binary
        //Serial.print("  time (ms): "); // time report for testing/debugging
        //Serial.println(millis()); // time report for testing/debugging
      }
    }
    else {
      if (testmode == 1) {
        inputs = 0;
        Serial.print(inputs, BIN);
        //Serial.print("no reply");
      }
    }

    //************************************************
    // Now, cycle through each output address on this slave, and decide whether to change the output
    // This is the hard part. Bitmath ho!!!
    //************************************************



    for (int j = 0; j < 8; j++) { //cycle through each hg per slave (512! only testing those on this controller...)
      HGID = 999; // HGID will be 999 if not found, otherwise actual reference (1-128)

      for (int k = 0; k < 128; k++) { // cycle through 128 values of ref array
        if (((controlNumber * 64) + (i * 8) + j) == pgm_read_word(&refArray[k][1])) {
          HGID = pgm_read_word(&refArray[k][0]); // Which ref are we talkin' bout? HGID!
          break;
        }
      } // close search loop because current ID is stored in HGID or HGID = 999

      if (testmode == 1) {
        Serial.print(" ID:");
      }
      if (testmode == 1) {
        Serial.print(HGID); // Checked! This does infact contain the 1-128 reference or is 999.
      }

      // so now we know that at position (1-512, based on loop position), HGID is the real ID (1-128)
      // (remember to -1 in the arrays to 0 ref everytihng)
      // if the relay is off and seconds elapsed % frequency is 0;

      if (HGID < 999) { //if there is an HG at this position...
        if (testmode == 1) {Serial.print("|"); Serial.print(pgm_read_word(&HGfreq[HGID]));
                            Serial.print("|"); Serial.print(HGstatus[HGID]);}
        if ((HGstatus[HGID] == 0) && (secondsTotal % pgm_read_word(&HGfreq[HGID]) == 0)) {  // if it's time, and current HG is off,
          HGstatus[HGID] = 1; //then turn it on!
          HGupdate[HGID] = secondsTotal; // and record the time!
          if (testmode == 1) {Serial.print(">"); Serial.print(HGupdate[HGID]); Serial.print("|"); Serial.print(HGstatus[HGID]);} //verbose boolean and trigger testing; don't use unless you're really screwed
        }// Confirmed = THIS WORKS! :: modulo works, seconds are recorded, status is updated
        if ((HGstatus[HGID] == 1) && (secondsTotal - HGupdate[HGID] > debouncetime)) { // if it's on AND it's out of debounce
          
          byte l = (1 << j); //Serial.print("?");Serial.print(l, BIN);Serial.print("?a");Serial.print(inputs, BIN);
          l = (l & inputs); //Serial.print("?b");Serial.print(l, BIN);
          l = (l >> j); //Serial.print("?c");Serial.print(l, BIN);
          if (l==1) { // check if index k HG is 1 or 0, if 1 shutoff, or emerge shutoff if active for too long
            HGstatus[HGID] = 0; //Turn it off
            if (testmode == 1) {Serial.print("X");} //indicate that OFF SENT DUE TO input.
          }
          else if ((secondsTotal - HGupdate[HGID]) > maxactive) { // check if index k HG is 1 or 0, if 1 shutoff, or emerge shutoff if active for too long
            HGstatus[HGID] = 0; //Turn it off
            if (testmode == 1) {Serial.print("T");} //indicate that OFF SENT DUE TO maxtime.
          }
          else {
            if (testmode == 1) {Serial.print("!");} //indicate that OFF PENDING - just waiting for either MAX or for input.
          }
        }
      } // and ignore the other 300-some positions - these will remain at 0 FOREVER MWAHAHA

      // put the current output value into OUTPUTS
      byte bit_mask = 1 << j;  // mask the bit I want
      outputs = ((outputs & (~bit_mask)) | (HGstatus[HGID] << j)); //change the bit I want
    }

    //************************************************
    // Send outputs to the MCP
    //************************************************

    Wire.beginTransmission(MCPid); //select 1st MCP "000"
    Wire.write(0x13); // set MCP23017 memory pointer to GPIOb address
    Wire.write(~outputs);  //NOTE LOGIC IS REVERSED!!
    Wire.endTransmission();  //transmission status from i2c bus (0 = success)
    if (testmode == 1) {
      //Serial.print("slave ");
      //Serial.print(MCPid-31); //print MCP number from 1-8
      Serial.print("   write: ");
      Serial.println(outputs, BIN); // display the contents of the GPIOB register in binary
      //Serial.print("   time(ms) "); // time report for testing/debugging
      //Serial.println(millis()); // time report for testing/debugging
    }
  }
  //outputs = outputs+1; // increment
}

