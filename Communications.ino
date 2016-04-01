// private variables
YunServer server(5555);


////////////////////////////////////////////////////////////////////////////////


void setupCommunications() {
  
  // start bridge and console
  addToLog("Starting bridge...");
  Bridge.begin();

    
  // start server
  addToLog("Bridge ready, starting server...");
  server.noListenOnLocalhost();
  server.begin();

  addToLog("Server ready.");
  
}


void loopCommunications() {

  // listen to and process to incoming requests
  processIncominRequests();
  
}


////////////////////////////////////////////////////////////////////////////////


// process incoming requests
void processIncominRequests() {
  
  YunClient client = server.accept();
  
  // if request found, process it
  if (client) {

      // play tune
      playTune(1047,1047);
  
      // process request
      // CORS header 
      client.println("Status: 200");
      client.println("Access-Control-Allow-Origin: *");   
      client.println("Access-Control-Allow-Methods: GET");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();
      
  
      // read command
      String command = client.readStringUntil('\r');

      if (command == "i") {
        addToLog("Remote request : init");
        client.print(harvestInitData());
      }
      else if (command == "g") {
        client.print(harvestData());
      }
      else if (command == "o") {       
        addToLog("Remote door open");
        client.print(openDoor(true));                 
      }
      else if (command == "c") {
        addToLog("Remote door close");
        client.print(closeDoor(true));      
      }
      else if (command == "s") {
        addToLog("Remote door stop");        
        client.print(stopDoor());  
      }
      else if (command == "r") {
        addToLog("Remote door release");   
        unsuspendDoor();
        client.print("door released");
      }    
      else if (command.startsWith("t")) {
        addToLog("Remote time sync");
        sync(command.substring(1, command.length() - 1));
        client.print("Synced");
      }
      else {
        client.print(F("Error : invalid command"));
      }
    
    // Close connection and free resources.
    client.stop();
  }
  
}


// harvest init data
String harvestInitData() {

/*
  String initData ="";

  // door suspension info
  initData.concat("Suspension-");
  initData.concat(getDoorSuspensionTimeOut());
  initData.concat("\n");

  // sunlight info
  initData.concat("Night-");
  initData.concat(getNightfall());
  initData.concat("-");
  initData.concat(getNightThreshold()); 
  initData.concat("\n");
  
  initData.concat("Day-");   
  initData.concat(getDaybreak());
  initData.concat("-"); 
  initData.concat(getDayThreshold());
  initData.concat("\n");  
*/
/*
  String initData = "{";
  initData.concat(concatJSON("Suspension", String(getDoorSuspensionTimeOut())) + ",");
  initData.concat(concatJSON("nightfall", String(getNightfall())) + ",");
  initData.concat(concatJSON("nightthreshold", String(getNightThreshold())) + ",");  
  initData.concat(concatJSON("daybreak", String(getDaybreak())) + ",");
  initData.concat(concatJSON("daythreshold", String(getDayThreshold())) + ",");
 */
  String keys[5];// = {"Suspension","nightfall","nightthreshold","daybreak","daythreshold"};
  String values[5];// = {String(getDoorSuspensionTimeOut()),String(getNightfall()),String(getNightThreshold()),String(getDaybreak()),String(getDayThreshold())};
  String initData;
  return initData;

}


// harvest data
String harvestData() {

  String data ="";

  // door info
  data.concat("Door-");
  data.concat(getCurrentDoorState());
  data.concat("\n");
  
  // door standby info
  data.concat("Suspension-");
  data.concat(getDoorSuspensionDate());
  data.concat("\n");

  // sunlight info
  data.concat("Light-");
  data.concat(getSunlight());    
  data.concat("\n");
  
  // temperature info
  data.concat("Temperature-");
  data.concat(getTemperature());
  data.concat("\n");

  // humidity info
  data.concat("Humidity-");
  data.concat(getHumidity());
  data.concat("\n");
  
  // clock info
  data.concat("Time-");
  data.concat(getTime());
  data.concat("\n");
  data.concat("Date-");
  data.concat(getDate());
  data.concat("\n");
  
  // log info
  data.concat("Log-");
  data.concat(getLog());
  data.concat("\n");
  
  flushLog();
 
  return data;
  
}


// sync
void sync(String syncData) {
  Console.println('day :');
  Console.println(extractToken(syncData, 'D'));
  setTime(  extractToken(syncData, 'Y'),
            extractToken(syncData, 'M'),
            extractToken(syncData, 'D'),
            extractToken(syncData, 'H'),
            extractToken(syncData, 'I'),
            extractToken(syncData, 'S') );
}


// extract given int data from response string
uint16_t extractToken(String input, char tag) {  
  String extract = input.substring(input.indexOf(tag)+1,input.lastIndexOf(tag));
  return extract.toInt();
}


// JSON concat
String concatJSON (String key, String value) {
  
  return ("\"" + key + "\":\"" + value + "\"");
  
}

