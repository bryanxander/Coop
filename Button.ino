#define buttonLightPulsation 0.004
#define debounceDelay 250
 
volatile bool debouncing = false;
volatile uint32_t lastPushDate = 0;


////////////////////////////////////////////////////////////////////////////////


// setup
void setupButton() {

  // set led as fully bright
  analogWrite(oButtonLight, 255);

  // set ISR
  interrupts();
  attachInterrupt(4, onButtonPush, RISING);
 
}


// loop
void loopButton() {

  // compute and set light level
  float buttonLightLevel = 128 * ( 1 + sin( buttonLightPulsation * millis() ) );
  analogWrite(oButtonLight, buttonLightLevel);


  // re-attach ISR after debounce delay
  if (debouncing && millis() > lastPushDate + debounceDelay) {

    // end debounce
    debouncing = false;

    // re-ttach ISR
    interrupts();
    //attachInterrupt(4, onButtonPush, RISING);    
    
  }
  
}


////////////////////////////////////////////////////////////////////////////////


// button ISR
void onButtonPush() {
  
  if (!digitalRead(iButton)) {
    return;
  }

  // detach ISR
  noInterrupts();
  //detachInterrupt(4);

  // store push date
  debouncing = true;
  lastPushDate = millis();

  // play tune
  playTune(1047, 1047);

  // toggle door
  toggleDoor(true);
  
}

