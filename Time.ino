#define sunlightReadingCount 4
#define sunlightNightThreshold 10
#define sunlightDayThreshold 100
#define equinoxDaybreak 360 // 6h in minutes
#define equinoxNightfall 1080 // 18h in minutes


RTC_DS1307 RTC;
uint32_t daybreak = 400;
uint32_t nightfall = 2000;
bool useLightSensor = false;


////////////////////////////////////////////////////////////////////////////////


void setupTime() {

  // init
  Wire.begin();
  RTC.begin();

  // if not working, throw error and use light
  if (! RTC.isrunning()) {    
    addToLog("ERROR : RTC is not running ! Switching to light sensor.");
    useLightSensor = true;
  }
  else {
    evaluateSunHours();
  }
  
}


////////////////////////////////////////////////////////////////////////////////


// calculate approx sunset and sunrise
void evaluateSunHours() {

  float angle = (getDay() - 172) * 2 * 3.14159 / 365;
  float delta = cos(angle) * 120; // in minutes

  //uint8_t deltaHour = delta / 60;
  //uint8_t deltaMinutes = uint16_t(delta) % 60;
  
  uint16_t daybreakInMinutes = equinoxDaybreak - delta; //(deltaHour * 100 + deltaMinutes);
  uint16_t nightfallInMinutes = equinoxNightfall + delta; //(deltaHour + 1) * 100 + deltaMinutes;  

  daybreak = (daybreakInMinutes / 60) * 100 + daybreakInMinutes%60;
  nightfall = (nightfallInMinutes / 60 + 1) * 100 + nightfallInMinutes%60;
  
}


// get current time
uint16_t getTime() {  
  DateTime dt = RTC.now();
  return (dt.hour() * 100 + dt.minute());
}

float getTimeSeconds() {
  DateTime dt = RTC.now();
  return (dt.hour() * 100 + dt.minute() + dt.second() * 0.01);
}


// get current date
uint16_t getDate() {
  DateTime dt = RTC.now();
  return dt.month() * 100 + dt.day();
}


// get current day approx number
uint16_t getDay() {
  DateTime dt = RTC.now();
  return (dt.month() - 1) * 30.5 + dt.day();
}


void setTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec) {
  
  if (RTC.isrunning()) {    
    DateTime now = DateTime(year,month,day,hour,min,sec);
    RTC.adjust(now);
        
    addToLog("RTC adjusted");
  }
  else
  {
    addToLog("RTC not running, couldn't adjust time");
  }
}


// get current light level
uint16_t getSunlight() {
  return analogRead(iSunlight);  
}


// get sunlight night threshold
uint16_t getNightThreshold() {
  return sunlightNightThreshold;
}


// get sunlight day threshold
uint16_t getDayThreshold() {
  return sunlightDayThreshold;
}


// get daybreak hour
uint16_t getDaybreak() {
  return daybreak;
}


// get nighfall hour
uint16_t getNightfall() {
  return nightfall;
}


// is it day ?
bool isDay() {
  
  bool isDay;
  if (useLightSensor) {
    isDay = getSunlight() > sunlightDayThreshold;
  }
  else {
    isDay = (getTime() > daybreak) && (getTime() < nightfall);
  }
  return isDay;
}


// is it night ?
bool isNight() {

  bool isNight;
  if (useLightSensor) {
    isNight = getSunlight() < sunlightNightThreshold;
  }
  else {
    isNight = (getTime() < daybreak) || (getTime() > nightfall);
  }  
  return isNight;
}

