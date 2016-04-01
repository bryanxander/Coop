#define doorSuspensionTimeOut 1200000 // 20 min

bool doorIsSuspended = false;
uint32_t doorSuspensionDate = 0;
bool motorState = true; // true : open, false : close
bool doorIsMoving = false;

////////////////////////////////////////////////////////////////////////////////


// setup
void setupDoor() {

  // read motor state before shut down
  applyMotorState(!digitalRead(iDoorBottom));
}


// loop
void loopDoor() {

  // check for door reach
  if (doorIsMoving) {

    if (motorState && digitalRead(iDoorTop)) {
        addToLog("Door opened.");  
        playTune(1047,2794);
        doorIsMoving = false;
    }
    
    if (!motorState && digitalRead(iDoorBottom)) {
        addToLog("Door closed.");
        playTune(2794, 1047);
        doorIsMoving = false;    
    }
  }
  
  // if door is currently suspended, check for timeout
  if (doorIsSuspended) {
    
    if (millis() - doorSuspensionDate > doorSuspensionTimeOut) {
      unsuspendDoor();
    }
    else {
      return;
    }
    
  }
  else {
    
    // check light and time 
    if (isDay() && !motorState) {    
      addToLog("Day time !");    
      addToLog(openDoor(false));
    
    }  
    else if (isNight() && motorState) {
      addToLog("Night time !");    
      addToLog(closeDoor(false));    
    }
    
  }

}


////////////////////////////////////////////////////////////////////////////////


// toggle door
String toggleDoor(bool suspend) {

  // if requested, suspend door
  if (suspend) {
    suspendDoor();
  }
  
  // proceed
  if(motorState) {
    return closeDoor(true);
  }
  else {
    return openDoor(true);
  }
    
}


// open door
String openDoor(bool suspend) {
  
  // if door is already opened, notify and skip
  if (motorState) {
    if (digitalRead(iDoorTop)) {
      return "Door already opened";
    }
    else {
      return "Door already opening";
    }    
  }

  // if requested, suspend door
  if (suspend) {
    suspendDoor();
  }  

  // update motor
  applyMotorState(true);

  return "Door opening...";
}


// close door
String closeDoor(bool suspend) {
  
  // if door is already closed, notify and skip
  if (!motorState) {
    if (digitalRead(iDoorBottom)) {
      return "Door already closed";
    }
    else {
      return "Door already closing";
    }    
  }
  
  // if requested, suspend door
  if (suspend) {
    suspendDoor();
  }

  // update motor
  applyMotorState(false);

  return "Door closing...";
}


// stop door
String stopDoor() {

  // stop motor
  digitalWrite(oMotorDown, HIGH);
  digitalWrite(oMotorUp, HIGH);
  analogWrite(oMotorSpeed, 255); 

  return "Door stopped...";
}


// suspend Door
void suspendDoor() {

  // suspend
  doorIsSuspended = true;
  doorSuspensionDate = millis();
}


// unsuspend door
void unsuspendDoor() {

  // unsuspend
  doorIsSuspended = false;
  doorSuspensionDate = 0;
}


// get current door state
String getCurrentDoorState() {
 
  // if door is already opened, notify and skip
  if (motorState) {
    if (iDoorTop) {
      return "opened";
    }
    else {
      return "opening";
    }    
  }
  // if door is already closed, notify and skip
  if (!motorState) {
    if (digitalRead(iDoorBottom)) {
      return "closed";
    }
    else {
      return "closing";
    }    
  }
}


// apply changes to motor
void applyMotorState(bool newMotorState) {

  // update pin voltage
  digitalWrite(oMotorDown, newMotorState);
  digitalWrite(oMotorUp, !newMotorState);
  analogWrite(oMotorSpeed, 255);
 
  
  // update variables
  motorState = newMotorState;
  doorIsMoving = true;
}


// get door time out
uint32_t getDoorSuspensionTimeOut() {
  return doorSuspensionTimeOut;
}


// get door suspension date
uint32_t getDoorSuspensionDate() {
  return doorSuspensionDate;
}


