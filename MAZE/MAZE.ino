

#include <EEPROM.h>


#define Kp 60
#define Kd 2000
#define MAX 170

#define ML A7
#define L A6
#define CL A5
#define C A4
#define CR A3
#define R  A2
#define MR A1

#define relax 100
#define leftrelax 375
#define rightrelax 400
#define endrelax 90

#define blue 10
#define red  12


#define Switch  A0

#define motorL 5
#define motorLp 7
#define motorLn 4
#define motorR 6
#define motorRp 8
#define motorRn 9
#define buzzer 13

#define leftOut 1
#define rightOut 2
#define notOut 3

#define calibrationSpeed 120

int sw;

int speedL = 150;
int speedR = 150;

int previousSpeedL;
int previousSpeedR;

char x = 'b';

int white[] = {
  1024 , 1024 , 1024 , 1024 , 1024 , 1024 , 1024};
int black[] = {
  -1, -1, -1, -1, -1, -1, -1};


int ir0 = 0, ir1 = 0, ir2 = 0, ir3 = 0, ir4 = 0, ir5 = 0, ir6 = 0;
int threshhold[7];
int s[7], S[7];

int path_length = 0;
unsigned char path[500]; 


double currentError = 0, diffError = 0, previousError = 0, sp=0.0;
bool foundLeft = false, foundRight = false, foundStraight = false, foundFinish = false;

double pwm;
int lineOut;
int outOfLine = notOut;
int outOfLine2 = -1;
double final, initial;

int leftMotorReading, rightMotorReading;

enum {right=-1, straight, left};
char situation;

void setup()
{
  Serial.begin(9600);
  //mySerial.begin(9600);

  pinMode(MR, INPUT);
  pinMode(R, INPUT);
  pinMode(L, INPUT);
  pinMode(C, INPUT);
  pinMode(ML, INPUT);
  pinMode(CL, INPUT);
  pinMode(CR, INPUT);

  pinMode(blue, OUTPUT);
  pinMode(red, OUTPUT);

  pinMode(Switch, INPUT_PULLUP);
  
  pinMode(buzzer, OUTPUT);

  pinMode(motorL, OUTPUT);
  pinMode(motorLp, OUTPUT);
  pinMode(motorLn, OUTPUT);
  pinMode(motorR, OUTPUT);
  pinMode(motorRn, OUTPUT);
  pinMode(motorRp, OUTPUT);


  //digitalWrite(motorLp, HIGH);
  //digitalWrite(motorLn, LOW);
  //analogWrite(motorL, 255);
  //digitalWrite(motorRp, HIGH);
  //digitalWrite(motorRn, HIGH);
  //analogWrite(motorR, 255);


  while(1) {
    sw = switchread();
    if(sw == 0) continue;
    else if(sw == 1) {
      threshholdread();
    }
    else if(sw == 2) {
      threshholdget();
      break;
    }
    else if(sw == 3) {
      threshholdget();
      break;
    }
  }
  delay(500);
  inch();
}

void loop()
{
  //lineFollowing();
  MazeSolve();
  /*irUpdate();
   Serial.print(ir0);
  Serial.print(' ');
  Serial.print(ir1);
  Serial.print(' ');
  Serial.print(ir2);
  Serial.print(' ');
  Serial.print(ir3);
  Serial.print(' ');
  Serial.print(ir4);
  Serial.print(' ');
  Serial.print(ir5);
  Serial.print(' ');
  Serial.print(ir6);
  Serial.print(' ');
  Serial.println();*/

}


/*int allWhite()
{
  if(analogRead(ML)>threshhold[0] && analogRead(L)>threshhold[1] && analogRead(CL)>threshhold[2] && analogRead(C)>threshhold[3] && analogRead(CR)>threshhold[4] && analogRead(R)>threshhold[5] && analogRead(MR)>threshhold[6]) return 1;
  else return 0;
}*/

int allBlack()
{
  if(analogRead(ML)<threshhold[0] && analogRead(L)<threshhold[1] && analogRead(CL)<threshhold[2] && analogRead(C)<threshhold[3] && analogRead(CR)<threshhold[4] && analogRead(R)<threshhold[5] && analogRead(MR)<threshhold[6]) return 1;
  else return 0;
}

/*int checkIntersect()
{
  return (analogRead(MR) > threshhold[6] || analogRead(R) > threshhold[5]) && (analogRead(ML) > threshhold[0] || analogRead(L) > threshhold[1]);
}
*/



int switchread()
{
  int s = 0;
  s = analogRead(Switch);
  //Serial.println(s);
  delay(100);
  if(s >=32 && s < 42) return 1;
  else if(s >= 24 && s < 31) return 2;
  else if(s >= 15 && s < 24) return 3;
  else return 0;
}

void inch() {
  wheel(230, 244);
  delay(90);
  hardbreak();
  delay(10);
}





void getInTrack()
{

  errorCalculation();

  if(currentError==0);

  else if(currentError>0)
  {
    while(currentError!=0)
    {
      errorCalculation();
      wheel(100, -100);
    }

    wheel(-100,100);
    delay(20);
    wheel(0,0);
//    delay(20);
  }

  else if(currentError<0)
  {
    while(currentError!=0)
    {
      errorCalculation();
      wheel(-100, 100);
    }
    wheel(100,-100);
    delay(20);
    wheel(0,0);
//    delay(20);
  }
}

void discontinutyLeh()
{
  wheel(speedL, speedR);
  delay(15);
  // anything more?? // 
}
void bluef(){
  digitalWrite(blue, HIGH);
  delay(10);
  digitalWrite(blue, LOW);
}
void redf(){
  digitalWrite(red,HIGH);
  delay(10);
  digitalWrite(red,LOW);
}
void beepbuzzer()
{
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer , LOW);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////SOLVER/////////////////////////////////////////////////////////////////////////
char select_turn(bool found_left, bool found_straight, bool found_right)
{
  if(found_left)
    return 'L';
  else if(found_straight)
    return 'S';
  else if(found_right)
    return 'R';
  else
    return 'B';
} 

void simplify_path()
{
  // only simplify the path if the second-to-last turn was a 'B'
  if(path_length < 3 || path[path_length-2] != 'B')
    return;

  int total_angle = 0;
  int i;
  for(i=1;i<=3;i++)
  {
    switch(path[path_length-i])
    {
      case 'R':
        total_angle += 90;
	break;
      case 'L':
	total_angle += 270;
	break;
      case 'B':
	total_angle += 180;
	break;
    }
  }

  // Get the angle as a number between 0 and 360 degrees.
  total_angle = total_angle % 360;

  // Replace all of those turns with a single one.
  switch(total_angle)
  {
    case 0:
	path[path_length - 3] = 'S';
	break;
    case 90:
	path[path_length - 3] = 'R';
	break;
    case 180:
	path[path_length - 3] = 'B';
	break;
    case 270:
	path[path_length - 3] = 'L';
	break;
  }

  // The path is now two steps shorter.
  path_length -= 2;
  
}
void getsituation() {
  if(ir1+ir3+ir5 == 3) situation = 'T';
  else if(ir0+ir1 == 2 && ir5+ir6 == 0) situation = 'L';
  else if(ir5+ir6 == 2 && ir0+ir1 == 0) situation = 'R';
  else if(ir0+ir1+ir2+ir3 +ir4+ir5+ir6 == 0) situation = 'B';
  else situation = 'N';
}

void MazeSolve()
{
  
  while(1)
  {
    do{
      lineFollowing();
      getsituation();
    }
    while(situation=='N');
    hardbreak();
    
    foundLeft = situation=='L'; 
    foundRight = situation=='R';
    if(situation == 'T') foundLeft = foundRight = true; 
    else foundFinish = false;
    delay(5);
    irUpdate();
    getsituation();
    foundLeft = situation=='L'; 
    foundRight = situation=='R';
    if(situation == 'T') foundLeft = foundRight = true; 
    //if(ir0+ir1+ir2+ir3+ir4+ir5+ir6 == 7) foundFinish = true;
    //else foundFinish = false;
    
    inch();
    irUpdate();
    getsituation();
    foundStraight = situation=='N';
    delay(5);
    irUpdate();
    getsituation();
    foundStraight = situation=='N';
    
    if(situation=='T' && ir0+ir1+ir2+ir3+ir4+ir5+ir6 == 7) {
      inch();
      irUpdate();
      if(ir0+ir1+ir2+ir3+ir4+ir5+ir6 == 7)
      wheel(0,0);
      hardbreak();
      delay(2000);
      break;
    }
    else foundFinish = false;
    
    unsigned char dir = select_turn(foundLeft, foundStraight, foundRight); 
    turn(dir);

    path[path_length++] = dir;
    simplify_path();
  }
  Serial.println();
  for(int i=0; i<path_length; i++) Serial.print((char)path[i]);
  Serial.println();


///Solution////////////////////////////////////////////
    wheel(0,0);
    for(int i=0; i<10; i++) {
      bluef();
    }
  
    
    for(int i=0; i<path_length; i++){  
      if(path[i]=='L') path[i]='R';
      else if(path[i]=='R') path[i]='L';
    }
    
    Serial.println();
    for(int i=path_length-1; i>=0; i--) Serial.print((char)path[i]);
    Serial.println();
    //while(1);
    
    /*wheel(-130,-150);
    delay(400);
    wheel(0,0);
    hardbreak();
    delay(30);*/
    //turn('B');
    
    wheel(0,0);
    hardbreak();
    delay(10);
    wheel(-200, 180);
    delay(300);
    turn('B');

    for(int i=path_length-1; i>=0; i--) {
      irUpdate();
      getsituation();
      while(situation=='N' ) {
        lineFollowing();
        getsituation();
      }
      Serial.println((char)path[i]);
      turn(path[i]);  
    }
    do{
        lineFollowing();
        getsituation();
    }while(situation=='N');
    
    
    
/////////////////solve done////////////////////    
  wheel(0,0);
  hardbreak();
  delay(50);
  for(int i=0; i<10; i++) {
    digitalWrite(12, HIGH);
    delay(300);
    digitalWrite(12, LOW);
    delay(300);
  }
  while(1);
} 


