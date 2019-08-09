// GPS velocity counter nii 2019/8/1

#include <Adafruit_GPS.h>

// what's the name of the hardware serial port?
#define GPSSerial Serial1

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

#define BEEP A6

uint32_t timer = millis();
uint32_t oldX = 0;
uint32_t oldY = 0;

void setup()
{
  pinMode(BEEP, OUTPUT);
  Serial.begin(250000);
  Serial.println("Adafruit GPS library basic test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(38400);

  calcVal();
}

void loop() // run over and over again
{
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    //Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
    timer = millis(); // reset the timer
    Serial.print("delay:");
    Serial.print(timer + 10000000L);
    Serial.print("Time: ");
    if (GPS.hour < 10) { Serial.print('0'); }
    Serial.print(GPS.hour, DEC); Serial.print(':');
    if (GPS.minute < 10) { Serial.print('0'); }
    Serial.print(GPS.minute, DEC); Serial.print(':');
    if (GPS.seconds < 10) { Serial.print('0'); }
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    if (GPS.milliseconds < 10) {
      Serial.print("0");
    }
    Serial.print(GPS.milliseconds, DEC);
    
    Serial.print(" quality: "); Serial.print((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Satellites: "); Serial.print((int)GPS.satellites);
      Serial.print("fix lat:"); Serial.print(GPS.latitude_fixed);
      Serial.print("fix lon:"); Serial.print(GPS.longitude_fixed);
      decodeGPS();
    }
    Serial.println("*");
  }
}

// calc the length of one degree.
//10000000 = 1 degree
float lenX, lenY;
void calcVal() {
  const float pi = 3.14159265;
  const float r_earth=6356752; //unit: m
  float t;
  t = r_earth*cos(35.7*pi/180)*2*pi;
  lenX = t /360.0;
  lenX = lenX * 0.00001;
  t = r_earth*2*pi;
  lenY = t /360.0;
  lenY = lenY * 0.00001;
  Serial.print(lenX);
  Serial.print(",");
  Serial.println(lenY);
  //1degree, lenX=90097.63m,lenY=110946.26m
}

// test one [m] and generate beep
void decodeGPS() {
  uint32_t x,y;
  float dx,dy,l;
  int8_t sd;
  int16_t blen;
  x = GPS.longitude_fixed;
  y = GPS.latitude_fixed;
  sd = GPS.fixquality;
  if (oldX == 0) {
    oldX = x;
  }
  if (oldY == 0) {
    oldY = y;
  }

  Serial.print(" D ");
  Serial.print((int32_t)x - (int32_t)oldX);
  Serial.print(",");
  Serial.print((int32_t)y - (int32_t)oldY);
  dx = ((int32_t)x - (int32_t)oldX) * lenX;
  dy = ((int32_t)y - (int32_t)oldY) * lenY;
  Serial.print(" Dif(");
  Serial.print(dx);
  Serial.print(",");
  Serial.print(dy);
  Serial.print(")");
  switch(sd) {
    case 4:// RTK FIXed
      blen = 100;
      break;
    case 5:// RTK Float
      blen = 150;
      break;
    default:
      blen = 250;
  }
  l = dx*dx+dy*dy;
  if(l >= 10000) {
    l = 1/sqrt(l/10000.0);
    Serial.print("l");
    Serial.print(l);
    if(l < 0.5) {
      oldX = x;
      oldX = y;
    } else {
      digitalWrite(BEEP, HIGH);
      delay(blen);
      digitalWrite(BEEP, LOW);
      Serial.print("Old");
      Serial.print(oldX);
      Serial.print(",");
      Serial.print(oldY);
      oldX = oldX + (int32_t)(((int32_t)x - (int32_t)oldX) * l);
      oldY = oldY + (int32_t)(((int32_t)y - (int32_t)oldY) * l);
    }
    Serial.print("New");
    Serial.print(oldX);
    Serial.print(",");
    Serial.print(oldY);
  }
}
