void playTune(uint16_t firstNote, uint16_t secondNote) {
  tone(oBuzzer, firstNote, 200);
  delay(100);
  tone(oBuzzer, secondNote, 200); 
}

