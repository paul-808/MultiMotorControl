// gets time and calculates seconds today  
void secondsToday(TinyGPS &gps){
  long lat, lon;
  float flat, flon;
  unsigned long age, date, time, chars;
  int year,hours;
  byte month, day, hour, minute, second, hundredths;
  unsigned short sentences, failed;
    gps.get_datetime(&date, &time, &age);
    if(testmode == 1){
      Serial.print("Date ddmmyy: "); Serial.print(date); Serial.print(" Time hhmmsscc: ");
      Serial.print(time);
      Serial.print(" Fix age: "); Serial.print(age); Serial.println("ms.");}

    gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
    if(testmode == 1){Serial.print("Date YYC: "); }
        if (time < 4000000){day--;}  // adjust day for UTC 
        if(testmode == 1){Serial.print(static_cast<int>(month)); Serial.print("/"); 
        Serial.print(static_cast<int>(day)); Serial.print("/"); Serial.print(year);}
   if(testmode == 1){ Serial.print("  Time YYC: "); }
        if(hour+18 >= 24){hours = hour -6;} // prevent hours above midnight
        else {hours = hour + 18;}
        if(testmode == 1){Serial.print(static_cast<int>(hours));}
        secondsTotal =  (static_cast<int>(hours));
        secondsTotal = secondsTotal *60*60;
        if(testmode == 1){ Serial.print(":"); }
    if(testmode == 1){Serial.print(static_cast<int>(minute)); }
        secondsTotal = secondsTotal + (minute*60) + second;
        millisAtTime = millis();
    if(testmode == 1){Serial.print(":"); Serial.print(static_cast<int>(second));
    Serial.print("."); Serial.print(static_cast<int>(hundredths)); Serial.print(" UTC -06:00 Calgary");
    Serial.print("  Fix age: ");  Serial.print(age); Serial.println("ms.");}

    if(testmode == 1){
    Serial.print("Seconds today/millis: ");
    Serial.print(secondsTotal);Serial.print("//");
    Serial.println(millis());
    //Serial.print(" test:");Serial.print((B1010010));
    }

}
