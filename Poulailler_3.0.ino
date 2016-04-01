// external libs
#include <Bridge.h>
#include <HttpClient.h>
#include <YunServer.h>
#include <YunClient.h>  
#include <DHT.h>
#include <Wire.h>
#include <RTClib.h>
#include <Console.h> // debug


// pin definitions
#define iDoorBottom A2
#define iDoorTop A1
#define iSunlight A4
#define iTempHum 4
#define iButton 7

#define oMotorUp A0
#define oMotorDown A3
#define oBuzzer 5
#define oButtonLight 6
#define oMotorSpeed 9


void setup() {

  // set pin modes
  pinMode(iDoorTop, INPUT);
  pinMode(iDoorBottom, INPUT);
  pinMode(iSunlight, INPUT);
  pinMode(iTempHum, INPUT_PULLUP);
  pinMode(iButton, INPUT);

  pinMode(oMotorDown, OUTPUT);
  pinMode(oMotorUp, OUTPUT);
  pinMode(oBuzzer, OUTPUT);
  pinMode(oButtonLight, OUTPUT);
  pinMode(oMotorSpeed, OUTPUT);

      
  // init peripherals
  setupButton();
  setupTime();
  //setupDoor();
  setupTemperature_Humidity();
  setupCommunications();
  setupDoor();

  // play tune to notify setup is complete
  playTune(1047, 1319);

  Console.begin(); // debug
    
}


void loop() {
  
  // put your main code here, to run repeatedly:
  loopButton();
  loopDoor();
  loopCommunications();
  
}

