bool errorCalculation()
{
  s[0]=analogRead(ML);
  s[1]=analogRead(L);
  s[2]=analogRead(CL);
  s[3]=analogRead(C);
  s[4]=analogRead(CR);
  s[5]=analogRead(R);
  s[6]=analogRead(MR);

  int avg = 0, val = 0;
  sum=0;
  for (int i = 0; i < 7; i++)
  {
    S[i] = (s[i] > threshhold[i]) ? !blackline : blackline;
//    Serial.print(s[i]);
//    Serial.print(' ');
    avg += (S[i] * i);
    sum +=  S[i];
    
  }
  
//backuplineOut  
  if(leftPrefer) {
    if(S[0]) backuplineOut = leftOut;
    else if(S[6]) backuplineOut = rightOut;
  }
  else {
    if(S[6]) backuplineOut = rightOut;
    else if(S[0]) backuplineOut = leftOut;
  }
//backuplineOut ends  
  
  if(ar[S[0]][S[1]][S[2]][S[3]][S[4]][S[5]][S[6]]==1) {
    if(sum>4) { 
      irUpdate();
      if(ar[S[0]][S[1]][S[2]][S[3]][S[4]][S[5]][S[6]]==1 && sum>4) {
        stop();
        delay(10);
        irUpdate();
        if(ar[S[0]][S[1]][S[2]][S[3]][S[4]][S[5]][S[6]]==1 && sum>4) {   
          blackline = !blackline;
          outOfLine = backuplineOut = notOut;
          left90=right90=false;
          wheel(200,200);
          delay(50);
        }
      }
      return false;
    }
    else if(S[3]) {
      if(S[2]) currentError = -1;
      else if(S[4]) currentError = 1;
      else currentError = 0;
      return true;
    }
  }

    if(sum == 0) val = -1;
    else val = int((avg * 2) / sum);
    currentError = val - 6;      //careful what u wish for :p
    return true;
}

int irUpdate()
{
  s[0]=analogRead(ML);
  s[1]=analogRead(L);
  s[2]=analogRead(CL);
  s[3]=analogRead(C);
  s[4]=analogRead(CR);
  s[5]=analogRead(R);
  s[6]=analogRead(MR);
  for (int i = 0; i < 7; i++)
  {
    S[i] = (s[i] > threshhold[i]) ? !blackline:blackline;
    sum+=S[i];
  }
  return sum;
}

void lineFollowing()
{
  while(!errorCalculation());
  //intersection catching
  if(ar[S[0]][S[1]][S[2]][S[3]][S[4]][S[5]][S[6]]==5) right90=left90=true;
  else if(S[6]+S[5]+S[4]+S[3]==4) left90=!(right90=true);
  else if(S[0]+S[1]+S[2]+S[3]==4) right90=!(left90=true);
  
  //if(left90 || right90) wheel(0,0); 
  //intersection catching ends
  //lineout
  if(blackline) {
    if(analogRead(MR) < threshhold[6]) {
      delay(5);
      if(analogRead(MR) < threshhold[6]) 
        outOfLine = rightOut;
    }
    if(analogRead(ML) < threshhold[0]) {
      delay(5);
      if(analogRead(ML) < threshhold[0]) 
        outOfLine = leftOut;
    }
  }
  else {
    if(analogRead(MR) > threshhold[6]) {
      delay(5);
      if(analogRead(MR) > threshhold[6]) 
        outOfLine = rightOut;
    }
    if(analogRead(ML) > threshhold[0]) {
      delay(5);
      if(analogRead(ML) > threshhold[0]) 
        outOfLine = leftOut;
    }
  }
//    if(blackline) {
//      if(analogRead(R) < threshhold[5]) {
//        delay(5);
//        if(analogRead(R) < threshhold[5]) 
//          outOfLine = rightOut;
//      }
//      if(analogRead(L) < threshhold[1]) {
//        delay(5);
//        if(analogRead(L) < threshhold[1]) 
//          outOfLine = leftOut;
//      }
//    }
//    else {
//      if(analogRead(R) > threshhold[5]) {
//        delay(5);
//        if(analogRead(R) > threshhold[5]) 
//          outOfLine = rightOut;
//      }
//      if(analogRead(L) > threshhold[1]) {
//        delay(5);
//        if(analogRead(L) > threshhold[1]) 
//          outOfLine = leftOut;
//      }
//    }
  //lineout ends
  
  //PID
  if(currentError == 0) diffError = 0;
  else if(currentError == -7) return;
  else if(currentError != 0) diffError = currentError - previousError;
  pwm = (Kp*currentError) + (Kd*diffError) + (Ki*integral);
  previousError = currentError;
  integral+=currentError;
  //PID ends
  
  //motor control
  MLeft = speedL + (pwm<0 ? pwm:0);
  MRight = speedR - (pwm<0 ? 0:pwm);
  //MLeft = speedL+pwm;
  //MRight = speedR-pwm;
  
  if (MLeft > MAX) MLeft = MAX;
  else if (MLeft < -revMAX) MLeft = -revMAX;		// to avoid extra reverse thrust //
  if (MRight > MAX) MRight = MAX;
  else if (MRight < -revMAX) MRight = -revMAX;		// to avoid extra reverse thrust //
  wheel(MLeft, MRight);
  //motor conreol ends
  
  static int timer=60;
  if(timer==0) {
    emergencyBrakeOn = true;
    timer=60;
  }
  else timer--;
  
  
}
