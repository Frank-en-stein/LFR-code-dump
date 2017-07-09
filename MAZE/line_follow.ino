void lineFollowing()
    {
      errorCalculation();
      PID();
      motorControl();
    }
    
    
    
void PID()
{
  if(currentError == 0) diffError = 0;
  else if(currentError == -7) return;
  else if(currentError != 0) diffError = currentError - previousError;
  pwm = (Kp*currentError) + (Kd*diffError);
  previousError = currentError;
}


void motorControl()
{
  /*if(currentError == 0){
   		wheel(250, 250);     //will be made 250 if slow
   
   	}*/
  /*else if(currentError == -7) {
   		delay(relax);
   //wheel(speedL, speedR);
   		return;
   	}*/
  //else 
  {
    int MLeft = speedL + pwm;
    int MRight = speedR - pwm;
    if (MLeft > 250) MLeft = 250;
    if (MRight > 250) MRight = 250;
    if (MLeft < -30) MLeft = -30;		// to avoid extra reverse thrust //
    if (MRight < -30) MRight = -30;		// to avoid extra reverse thrust //
    wheel(MLeft, MRight);
  }
}
