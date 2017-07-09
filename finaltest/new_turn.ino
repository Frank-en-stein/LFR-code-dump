void hardbreak(){
  wheel(-200, -200);
  delay(20);
  wheel(0,0);
  delay(10);
}
void Stop(){
  wheel(0,0);
  delay(50);
}
void RightTurn(){
  //wheel(-255,-255);
  //delay(30);
  hardbreak();
  delay(10);
  while(!((analogRead(C) > threshhold[3]) ? !blackline:blackline)){
    wheel(90,-90);
  }
  wheel(-255,255);
  delay(50);
  wheel(0,0);
  hardbreak();
  delay(15);
  irUpdate();
  while(S[5]+S[6]>0){
    wheel(-90, 90);
    irUpdate();
  }
  wheel(THROTTLE, THROTTLE);
  delay(100);
}
void LeftTurn(){
  //wheel(-255,-255);
  //delay(30);
  hardbreak();
  delay(10);                           
  while(!((analogRead(C) > threshhold[3]) ? !blackline:blackline)){
    wheel(-90,90);
   
  }
  wheel(255,-255);
  delay(50);
  wheel(0,0);
  hardbreak();
  delay(15);
  irUpdate();
  while(S[0]+S[1]>0){
    wheel(90, -90);
    irUpdate();
  }
  delay(10);
  wheel(THROTTLE, THROTTLE);
  delay(100);
}

