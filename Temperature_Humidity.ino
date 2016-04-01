DHT dht(iTempHum, DHT22);


////////////////////////////////////////////////////////////////////////////////


// setup
void setupTemperature_Humidity() {
  
  dht.begin();
  
}


////////////////////////////////////////////////////////////////////////////////


// get current temperature
float getTemperature() {
  return dht.readTemperature();  
}


// get humidity temperature
float getHumidity() {
  return dht.readHumidity();  
}

