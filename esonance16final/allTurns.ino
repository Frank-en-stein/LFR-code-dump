// turn select
void nolineTurnSelect() {
  if(left90 && right90) {
    if(leftPrefer) LeftTurn();
    else RightTurn();
  }
  else if(outOfLine == leftOut) LeftTurn();
  else if(outOfLine == rightOut) RightTurn();
  else if(backuplineOut == leftOut) LeftTurn();
  else if(backuplineOut == rightOut) RightTurn();
                   
  outOfLine = backuplineOut = notOut;
  left90=right90=false;//delay(5000);
}

void intersectionTurnSelect() {
  if(left90 && right90) {
    if(leftPrefer) intersection_LeftTurn();
    else intersection_RightTurn();
  }
  else if(leftPrefer && left90) intersection_LeftTurn();
  else if(!leftPrefer && right90) intersection_RightTurn;
  else if(outOfLine == leftOut) intersection_LeftTurn();
  else if(outOfLine == rightOut) intersection_RightTurn();
  else if(backuplineOut == leftOut) intersection_LeftTurn();
  else if(backuplineOut == rightOut) intersection_RightTurn();
  else wheel(200,200);
        
  outOfLine = backuplineOut = notOut;
  left90=right90=false;  
}
//turn seelect ends

//turns
void RightTurn(){
  //wheel(-200,-200);
  //delay(30);
  //hardbreak();
  //delay(10);
  //if(irUpdate()>2) blindRight90();
  while(!((analogRead(C) > threshhold[3]) ? !blackline:blackline)){
    wheel(120,-120);
  }
  wheel(-200,200);
  delay(20);
  wheel(0,0);
  delay(15);
//  while(!((analogRead(C) > threshhold[3]) ? !blackline:blackline)){
//    wheel(-70,70);
//  }
//  irUpdate();
//  while(S[5]+S[6]>0){
//    wheel(-90, 90);
//    irUpdate();
//  }
//  Stop();
//  delay(10);
//  wheel(THROTTLE, THROTTLE);
//  delay(50);
}
void LeftTurn(){
  //wheel(-200,-200);
  //delay(30);
  //hardbreak();
  ///delay(10);                   
  //if(irUpdate()>2) blindLeft90();  
  while(!((analogRead(C) > threshhold[3]) ? !blackline:blackline)){
    wheel(-120,120);
  }
  wheel(200,-200);
  delay(20);
  wheel(0,0);
  delay(15);
//  while(!((analogRead(C) > threshhold[3]) ? !blackline:blackline)){
//    wheel(70,-70);
//  }
//  irUpdate();
//  while(S[0]+S[1]>0){
//    wheel(90, -90);
//    irUpdate();
//  }
//  Stop();
//  delay(10);
//  wheel(THROTTLE, THROTTLE);
//  delay(50);
}

void intersection_RightTurn(){
//  wheel(200,200);
//  delay(25);
//  stop();
//  delay(10);
  //if(irUpdate()>2) blindRight90();
  while(!((analogRead(MR) > threshhold[5]) ? !blackline:blackline)){
    wheel(120,-120);
    
  }
  while(!((analogRead(C) > threshhold[3]) ? !blackline:blackline)){
    wheel(90,-90);
  }
  
  wheel(-255,255);
  delay(10);
  wheel(0,0);
  delay(15);
  
  //delay(5000);
}
void intersection_LeftTurn(){
//  wheel(200,200);
//  delay(25);
//  stop();
//  delay(10);
  //if(irUpdate()>2) blindLeft90();
  while(!((analogRead(L) > threshhold[1]) ? !blackline:blackline)){
    wheel(-120,120);
   
  }
  while(!((analogRead(C) > threshhold[3]) ? !blackline:blackline)){
    wheel(-90,90);
   
  }
  wheel(255,-255);
  delay(10);
  wheel(0,0);
  delay(15);
  
  //delay(5000);
}


void blindLeft90() {
  hardbrake();
  delay(10);
  wheel(-100, 100);
  delay(380);
  
  wheel(100, -100);
  delay(50);
  wheel(0,0);
  delay(50);
}

void blindRight90() {
  hardbrake();
  delay(10);
  wheel(100, -100);
  delay(380);
  
  wheel(-100, 100);
  delay(50);
  wheel(0,0);
  delay(50);
}
//turns end

