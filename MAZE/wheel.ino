


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
  
  previousSpeedL = l;
  previousSpeedR = r;

  if(l<0) l = abs(l);
  if(r<0) r = abs(r);

  analogWrite(motorR, r);
  analogWrite(motorL, l);
}
