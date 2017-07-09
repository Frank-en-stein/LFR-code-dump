//brakes

void stop() {
  wheel(0,0);
  delay(50);
}

void hardbrake(){
  wheel(-200, -200);
  delay(20);
  wheel(0,0);
  delay(10);
}

void emergencybrake() {
  if(!emergencyBrakeOn) {
    stop();
    return;
  }
  wheel(-255, -255);
  delay(100);
  wheel(0,0);
  delay(10);
  emergencyBrakeOn = false;
}

//brakes

//wheeling
void wheel(int l, int r)
{ 
  if(l > 0) {
    digitalWrite(motorLp, HIGH);
    digitalWrite(motorLn, LOW);
  }
  else if(l < 0) {
    digitalWrite(motorLp, LOW);
    digitalWrite(motorLn, HIGH);
  }
  else {
    digitalWrite(motorLp, HIGH);
    digitalWrite(motorLn, HIGH);
  }
  if(r > 0) {
    digitalWrite(motorRp, HIGH);
    digitalWrite(motorRn, LOW);
  }
  else if(r < 0) {
    digitalWrite(motorRp, LOW);
    digitalWrite(motorRn, HIGH);
  }
  else {
    digitalWrite(motorRp, HIGH);
    digitalWrite(motorRn, HIGH);
  }


  if(l<0) l = abs(l);
  if(r<0) r = abs(r);

  analogWrite(motorR, r);
  analogWrite(motorL, l);
}

void wheel_alt(int m2Speed, int m1Speed) {
  analogWrite(motorR, abs(m1Speed));
  analogWrite(motorL, abs(m2Speed));
  digitalWrite(motorRp, (m1Speed >= 0 ? HIGH:LOW));
  digitalWrite(motorRn, (m1Speed < 0 ? HIGH:LOW));
  digitalWrite(motorLp, (m2Speed >= 0 ? HIGH:LOW));
  digitalWrite(motorLn, (m2Speed < 0 ? HIGH:LOW));
}
//wheeling ends

//misc
void bluef(){
  digitalWrite(blue, HIGH);
  delay(10);
  digitalWrite(blue, LOW);
}
void redf(bool i){
  if(i) digitalWrite(red,HIGH);
  else digitalWrite(red,LOW);
}
void beepbuzzer()
{
  digitalWrite(buzzer, HIGH);
  delay(10);
  digitalWrite(buzzer , LOW);
}
int switchread()
{
  int s = 0;
  s = analogRead(Switch);
  Serial.println(s);
  delay(100);
  if(s >=920 && s < 950) return 1;
  else if(s >= 860 && s < 920) return 2;
  else if(s < 860) return 3;
  else return 0;
}

//int switchread()
//{
//  int s = 0;
//  s = analogRead(Switch);
//  Serial.println(s);
//  delay(100);
//  if(s >=0 && s < 25) return 1;
//  else if(s >= 100 && s < 500 ) return 2;
//  else if(s>=25 && s < 50 ) return 3;
//  else return 0;
//}
//
