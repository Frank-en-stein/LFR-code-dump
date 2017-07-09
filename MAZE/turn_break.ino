void hardBreak()
{
  wheel(-200, -200);
  delay(20);
  wheel(0, 0);
  delay(10);
}

void o_hardBreak()
{
  wheel(-250, -250);
  delay(40);
  wheel(0,0);
  delay(10);
}


void stop()
{
//  wheel(-200, -200);
//  delay(50);
  wheel(0, 0);
  delay(50);		// this delay is important, applied by Khaled Bhai //
}

void forward()
{
  wheel(previousSpeedL, previousSpeedR);
}
void forward2()
{
  bluef();
  redf();
  beep();
  wheel(170,200);
  delay(3000);
  wheel(-200,-200);
  delay(20);
  wheel(0,0);
  delay(10);
  bluef();
  redf();
  beep();
}


void rightTurn()
{
  wheel(200, 200);
  delay(60);
  hardBreak();
  delay(5000);
  while(!(analogRead(CR) < threshhold[4])) {
   beepbuzzer();
   redf();
    wheel(250, -250);
    
    
    redf();
    
  }
  delay(5000);
  wheel(-220, 220);
  delay(20);
  wheel(0,0);
  delay(10);
}


void leftTurn()
{
  wheel(200, 200);
  delay(60);
  hardBreak();
  delay(2000);
  while(!(analogRead(CL) < threshhold[2])) {
    
    beepbuzzer();
    
    wheel(-250, 250);
    
    bluef();
  }
  delay(5000);
  wheel(220, -220);
  delay(20);
  wheel(0,0);
  delay(10);
}

void o_leftTurn()
{
//  wheel(100, 100);
//  delay(25);
  hardBreak();
  while(!(analogRead(CL) > threshhold[2])) {
    //wheel(-250, 250);
    wheel(-140, 140);
  }
  wheel(120, -120);
  delay(20);
  wheel(0,0);
  delay(10);
}

void o_rightTurn()
{
//  wheel(100, 100);
//  delay(25);
  hardBreak();
  while(!(analogRead(CR) > threshhold[4])) {
    //wheel(-250, 250);
    wheel(140, -140);
  }
  wheel(-120, 120);
  delay(20);
  wheel(0,0);
  delay(10);
}
