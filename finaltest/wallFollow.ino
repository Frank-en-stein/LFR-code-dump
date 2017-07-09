
void wallget() {
    mySerial.flush();
    bluef();
      reading = (char) mySerial.read();
      //Serial.print(reading);
      //Serial.print(" ");
      if(reading==125) {
        do{
          SM =(char) mySerial.read();
        }while(SM==-1);
        do{
          SL =(char) mySerial.read();
        }while(SL==-1);
        do{
          SR =(char) mySerial.read();
        }while(SR==-1);
        
        right_wall = SR>0;
        left_wall = SL>0;
        wall_found = right_wall | left_wall;
        obj_found = SM>0;
      
    //Serial.println();
  }
}

void front_sonar() {
    mySerial.flush();
    bluef();
      reading = (char) mySerial.read();
      //Serial.print(reading);
      //Serial.print(" ");
      if(reading==125) {
        do{
          SM =(char) mySerial.read();
        }while(SM==-1);
      }
}
void wallfollow() {
  if(obj_found && SM<setpoint_wall) {
    wheel(0,0);
    delay(10);
    wallget();
  }
  if(obj_found && SM<setpoint_wall) {
    if(SM<7) {
      hardbreak();
      delay(10);
      if(left_wall) MLeft=cave_turn, MRight=-cave_turn;
      else MLeft=-cave_turn, MRight=cave_turn;
      do{
        wallget();
      }while(SM!=0);
      hardbreak();
      delay(10);
    }
    else {
      if(left_wall) MLeft=cave_turn, MRight=-cave_revMAX;
      else MLeft=-cave_revMAX, MRight=cave_turn;
    }
  }
  else if(SL+SR==0) { 
    if(lastwall_right) MLeft=cave_turn, MRight=-cave_revMAX;
    else MLeft=-cave_revMAX, MRight=cave_turn;
  }
  else {
    if(SR==0) SR=35;
    if(SL==0) SL=35;
    int error;// = (right_wall ? SR - setpoint_wall : setpoint_wall - SL);
    if(right_wall) {
      error = SR - setpoint_wall;
      lastwall_right = true;
    }
    else {
      error = setpoint_wall - SL;
      lastwall_right = false;
    }
    int turn = error*kp_wall + (error - lasterror_wall)*kd_wall;
    
    MLeft = cave_MAX + turn;
    MRight = cave_MAX - turn;
    if (MLeft > cave_MAX) MLeft = cave_MAX;
    if (MRight > cave_MAX) MRight = cave_MAX;
    if (MLeft < -cave_revMAX) MLeft = -cave_revMAX;		// to avoid extra reverse thrust //
    if (MRight < -cave_revMAX) MRight = -cave_revMAX;		// to avoid extra reverse thrust //
    
    lasterror_wall = error;
  }
  wheel(MLeft, MRight);
}

void wallfollowRight() {
  if(SR==0) {
    wheel(0,0);
    return;
  }
  int error = SR - setpoint_wall;
  if(SM>0 && SM<16){
    do{
      wheel(-90,90);
      wallget();
    }while(SM>0);
  }
  int turn = error*kp_wall + (error - lasterror_wall)*kd_wall;
  
  MLeft = cave_throttle + turn;
  MRight = cave_throttle - turn;
  if (MLeft > cave_MAX) MLeft = cave_MAX;
  if (MRight > cave_MAX) MRight = cave_MAX;
  if (MLeft < -cave_revMAX) MLeft = -cave_revMAX;		// to avoid extra reverse thrust //
  if (MRight < -cave_revMAX) MRight = -cave_revMAX;		// to avoid extra reverse thrust //
    
  lasterror_wall = error;
  wheel(MLeft, MRight);
}


