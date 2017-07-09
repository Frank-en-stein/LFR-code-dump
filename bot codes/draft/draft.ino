int m1 = 6, m2 = 5;
int leftForward = 7, leftBack = 4, rightForward = 8, rightBack = 9;
int m1Speed = -180;   //right
int m2Speed = 180;   //left
void setup(){
  
}
void loop(){
  analogWrite(m1, abs(m1Speed));
  analogWrite(m2, abs(m2Speed));
  digitalWrite(rightForward, (m1Speed >= 0 ? HIGH:LOW));
  digitalWrite(rightBack, (m1Speed < 0 ? HIGH:LOW));
  digitalWrite(leftForward, (m2Speed >= 0 ? HIGH:LOW));
  digitalWrite(leftBack, (m2Speed < 0 ? HIGH:LOW));
}
