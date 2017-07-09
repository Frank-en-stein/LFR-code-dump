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
    avg = avg + (S[i] * i);
    sum = sum + S[i];
    
  }
  if(ar[S[0]][S[1]][S[2]][S[3]][S[4]][S[5]][S[6]]) {
    if(sum>4) { 
      irUpdate();
      if(ar[S[0]][S[1]][S[2]][S[3]][S[4]][S[5]][S[6]] && sum>4) { 
        blackline = !blackline;
        outOfLine = notOut;
        left90=right90=false;
        wheel(200,200);
        delay(100);
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
  int sum=0;
  for (int i = 0; i < 7; i++)
  {
    S[i] = (s[i] > threshhold[i]) ? !blackline:blackline;
    sum+=S[i];
  }
  return sum;
}
