void setup() {
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}

void loop() {
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  delay(300);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  delay(300);
}
