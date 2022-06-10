#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <RH_ASK.h>
#include <SPI.h>
#define mq3pin 0
#define fsp A1
#define XPin A3
#define YPin A4
#define ZPin A2

float mq3value , latitude , longitude;
int vibr_pin = 3; 
int LED_pin = 13;
int xval , yval , zval , acc1 , acc2 , acc3;
SoftwareSerial gpsSerial(3,4);
TinyGPS gps;
RH_ASK rf_driver;

void setup() {
    rf_driver.init();
    pinMode(vibr_pin , INPUT);
    pinMode(LED_pin , OUTPUT);
    pinMode(XPin , OUTPUT);
    pinMode(YPin , OUTPUT);
    pinMode(ZPin , OUTPUT);
    Serial.begin(9600);
    gpsSerial.begin(9600);
    delay(20000); //to let mq3 sensor warm up
}

void loop() {
    int val1 , val2;
    int fspVal = analogRead(fsp);
    mq3value = analogRead(mq3pin);
    if (fspVal < 10){
      const char *msg = "HELMET NOT FOUND , turn bike off";
      rf_driver.send((uint8_t *)msg, strlen(msg));
      rf_driver.waitPacketSent();
      delay(1000);
      Serial.begin("HELMET NOT FOUND");
    }
    else if (mq3value > 400){
      const char *msg = "DRUNK!! , turn bike off";
      rf_driver.send((uint8_t *)msg, strlen(msg));
      rf_driver.waitPacketSent();
      delay(1000);
      Serial.begin("DRUNK!!");
    }
    else{
      val1 = digitalRead(vibr_pin);
      delay(2000);
      val2 = digitalRead(vibr_pin);
      
      if(abs(val1-val2) > 1000){
        Serial.println("ACCIDENT MIGHT HAVE OCCURED");
        
      if(gpsSerial.available() > 0){ // check for gps data 
      if(gps.encode(gpsSerial.read())){   //encoding GPS data
      gps.f_get_position(&latitude, &longitude); //getting coordinates
    }
     Serial.println("Latitude: " + String(latitude) + " Longtitude: " + String(longitude)); 
  }
      }
    }
} 
