void setup() {
}

// the loop function runs over and over again forever
void loop() {
  for(int i=1;i<20;i++){
    pinMode(i,OUTPUT);
    digitalWrite(i,HIGH);
    delay(250);
    digitalWrite(i,LOW);
    delay(750);
  }
}
