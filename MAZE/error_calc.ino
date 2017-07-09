void errorCalculation()
{
  s[0]=analogRead(ML);
  s[1]=analogRead(L);
  s[2]=analogRead(CL);
  s[3]=analogRead(C);
  s[4]=analogRead(CR);
  s[5]=analogRead(R);
  s[6]=analogRead(MR);
  
  /*for(int i=0; i<7; i++) {
    Serial.print(s[i]);
    Serial.print(" ");
  }
  Serial.println();*/

  if (s[0] > threshhold[0]) {

    ir0 = 0;
  }
  else {

    ir0 = 1;
  }
  if(s[1] > threshhold[1]) {

    ir1 = 0;
  }
  else {

    ir1 = 1;
  }
  if(s[2] > threshhold[2]) {

    ir2 = 0;
  }
  else {

    ir2 = 1;
  }
  if(s[3] > threshhold[3]) {

    ir3 = 0;
  }
  else {

    ir3 = 1;
  }
  if(s[4] > threshhold[4]) {

    ir4 = 0;
  }
  else {

    ir4 = 1;
  }
  if(s[5] > threshhold[5]) {

    ir5 = 0;
  }
  else {

    ir5 = 1;
  }
  if (s[6] > threshhold[6]) {

    ir6 = 0;
  }
  else {

    ir6 = 1;
  }
  /*Serial.print(ir0);
    Serial.print(" ");
  Serial.print(ir1);
    Serial.print(" ");
  Serial.print(ir2);
    Serial.print(" ");
  Serial.print(ir3);
    Serial.print(" ");
  Serial.print(ir4);
    Serial.print(" ");
  Serial.print(ir5);
    Serial.print(" ");
  Serial.print(ir6);
    Serial.println(" ");  */
  
  int sum = 0, avg = 0, val = 0;
  for (int i = 0; i < 7; i++)
  {
    S[i] = (s[i] > threshhold[i]) ? 0 : 1;

    avg = avg + (S[i] * i);
    sum = sum + S[i];
  }
  if(sum == 0) val = -1;
  val = int((avg * 2) / sum);
  currentError = val - 6;      //carefull what u wish for :p
  //Serial.println(currentError);
  

}



void irUpdate()
{
  s[0]=analogRead(ML);
  s[1]=analogRead(L);
  s[2]=analogRead(CL);
  s[3]=analogRead(C);
  s[4]=analogRead(CR);
  s[5]=analogRead(R);
  s[6]=analogRead(MR);
 

  if (s[0] > threshhold[0]) ir0 = 0;
  else ir0 = 1;
  if(s[1] > threshhold[1]) ir1 = 0;
  else ir1 = 1;
  if(s[2] > threshhold[2]) ir2 = 0;
  else ir2 = 1;
  if(s[3] >threshhold[3]) ir3 = 0;
  else ir3 = 1;
  if(s[4] > threshhold[4]) ir4 = 0;
  else ir4 = 1;
  if(s[5] > threshhold[5]) ir5 = 0;
  else ir5 = 1;
  if (s[6] > threshhold[6]) ir6 = 0;
  else ir6 = 1;
}
