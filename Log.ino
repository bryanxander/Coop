String logContent = "";

void addToLog(String message) {

  // append timestamp and message
  logContent += "\n " + String(getTimeSeconds()) + " : " + message;
}

String getLog() {
  return logContent;
}

void flushLog() {
  logContent = "";
}


