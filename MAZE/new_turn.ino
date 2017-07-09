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
  wheel(200,200);
  delay(80);
  hardbreak();
  delay(10);
  while(!(analogRead(MR)<threshhold[6])){
    wheel(170,-180);
    
  }
  while(!(analogRead(CR)<threshhold[4])){
    wheel(80,-100);
  }
  
  wheel(-220,220);
  delay(20);
  wheel(0,0);
  delay(10);
  
  //delay(2000);
}
void LeftTurn(){
  wheel(200,200);
  delay(80);
  hardbreak();
  delay(10);
  while(!(analogRead(ML)<threshhold[0])){
    wheel(-170,180);
   
  }
  while(!(analogRead(CL)<threshhold[2])){
    wheel(-80,100);
   
  }
  
  wheel(220,-220);
  delay(20);
  wheel(0,0);
  delay(10);
  
  //delay(2000);
}

void ReverseTurn(){
  //wheel(200,200);
  //delay(80);
  //hardbreak();
  //delay(10);
  while(!(analogRead(ML)<threshhold[0])){
    wheel(-200,180);
   
  }
  while(!(analogRead(CL)<threshhold[2])){
    wheel(-80,100);
   
  }
  
  wheel(220,-220);
  delay(20);
  wheel(0,0);
  delay(10);

}

void turn(char dir)
{
  switch(dir)
  {
    // Turn left 90deg
    case 'S':    
      inch();
      break;
    case 'B':  
      ReverseTurn();
      break;
    case 'L':  
      beepbuzzer();  
      LeftTurn();
      break;
      
    // Turn right 90deg
    case 'R':        
      RightTurn();
      break;

    // Straight ahead
    default:
      // do nothing
      break;
  }
}

