void errorCalculation()
{
  s[0]=analogRead(ML);
  s[1]=analogRead(L);
  s[2]=analogRead(CL);
  s[3]=analogRead(C);
  s[4]=analogRead(CR);
  s[5]=analogRead(R);
  s[6]=analogRead(MR);

  int avg = 0, val = 0, half1=0, half2=0, pivot = 1;
  sum=0;
  for (int i = 0; i < 7; i++)
  {
    S[i] = (s[i] > threshhold[i]) ? !blackline : blackline;
//    Serial.print(s[i]);
//    Serial.print(' ');
    avg = avg + (S[i] * i);
    sum = sum + S[i];
//    if(i<3) half1+=S[i];
//    else if(i>3) half2+=S[i];
    if(i>0) {
      if(S[i-1]-S[i]==pivot) {
        if(pivot>0) half1 = sum;
        pivot = -pivot;
      }  
    }
  }
  half2 = sum-half1;
//  Serial.print('\t');
//  Serial.print(half1);
//  Serial.print(' ');
//  Serial.print(half2);
//  Serial.print(' ');
//  Serial.print(blackline);
//  Serial.print('\t');
  if(half1>0 && half2>0) {
    if(S[3]+S[2]+S[4]<3) {
      blackline= !blackline;
      return;
    }
    else {
      if(sum == 0) val = -1;
      else val = int((avg * 2) / sum);
      currentError = val - 6;      //careful what u wish for :p
    }
  }
  if(sum == 0) val = -1;
  else val = int((avg * 2) / sum);
  currentError = val - 6;      //careful what u wish for :p
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
  int sum=0;
  for (int i = 0; i < 7; i++)
  {
    S[i] = (s[i] > threshhold[i]) ? !blackline:blackline;
    sum+=S[i];
  }
  return sum;
}
