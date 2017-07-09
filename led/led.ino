void setup() {
  for(int i = A0; i<=A7; i++)
    pinMode(i,OUTPUT);
}

void loop() {
  for(int i = A0; i<=A7; i++) {
    analogWrite(i,1023);
    delay(500);
    analogWrite(i,0);
    //delay(500);
  }
}

