#include <SoftwareSerial.h>
#include <EEPROM.h>

SoftwareSerial mySerial(3, 2);

#define Kp 42
#define Kd 190
#define Ki 0.0

#define MAX 250
#define revMAX 150

#define THROTTLE 190
#define SAFE_THROTTLE 90


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

#define motorR 6
#define motorRp 8
#define motorRn 9
#define motorL 5
#define motorLp 7
#define motorLn 4
#define buzzer 13

#define leftOut 1
#define rightOut 2
#define notOut 3

#define calibrationSpeed 120

bool blackline= true;
bool leftPrefer=false;
bool left90, right90;

int ar[2][2][2][2][2][2][2];
int sw,sw2;
int integral=0;
int speedL = THROTTLE;
int speedR = THROTTLE;

int MLeft,MRight;

int previousSpeedL;
int previousSpeedR;

bool hexlock=false;
int sum=0;

int white[] = {
  1024 , 1024 , 1024 , 1024 , 1024 , 1024 , 1024};
int black[] = {
  -1, -1, -1, -1, -1, -1, -1};


int ir0 = 0, ir1 = 0, ir2 = 0, ir3 = 0, ir4 = 0, ir5 = 0, ir6 = 0;
int threshhold[7];
int s[7], S[7];


double currentError = 0, diffError = 0, previousError = 0, sp=0.0;

double pwm;
int lineOut;
int outOfLine = notOut;
int outOfLine2 = -1;
double final, initial;
int reading;

//////////////////////////////////Wall vars////////////////////////////////////
#define kp_wall 6
#define kd_wall 50
#define cave_MAX 90
#define cave_revMAX 0
#define cave_turn 100
#define cave_throttle 90

int SL,SM,SR;
double setpoint_wall = 11;
int lasterror_wall = 0;
bool lastwall_right = false;
///////////////////////////////////////////////////////////////////////////////

void setup()
{
  Serial.begin(9600);
//  mySerial.begin(9600);

  setColorCases();

  pinMode(MR, INPUT);
  pinMode(R, INPUT);
  pinMode(L, INPUT);
  pinMode(C, INPUT);
  pinMode(ML, INPUT);
  pinMode(CL, INPUT);
  pinMode(CR, INPUT);

  pinMode(blue, OUTPUT);  pinMode(red, OUTPUT);

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

  //while(1) wheel(0, -250);
  int temp;
  while(1) {
    do{
      temp = switchread();
    }while(temp==0);
    sw = temp;
    do{
      temp = switchread();
    }while(temp==sw);
    do{
      temp = switchread();
    }while(temp==0);
    sw2 = temp;
    do{
      temp = switchread();
    }while(temp==sw2);
    do{
      temp = switchread();
    }while(temp!=0);
    
    if(sw == 2 && sw2 == 2) {
      threshholdread();
    }
    else if(sw == 2 && sw2 == 3) {
      leftPrefer=false;
      beepbuzzer();
      delay(100);
    }
    else if(sw == 3 && sw2 == 2) {
      leftPrefer=true;
      beepbuzzer();
      delay(100);
      beepbuzzer();
    }
    else if(sw == 3 && sw2 == 3) {
      threshholdget();
      break;
    }
  }
    
//  threshholdread();
//  threshholdget();
//  delay(3000);
//  mySerial.flush();
}

void loop()
{
  lineFollowing();
  right90 |= (S[5]+S[4]+S[3]) == 3;
  left90 |= (S[1]+S[2]+S[3]) == 3; 

//      errorCalculation();
      for (int i = 0; i < 7; i++) {
        Serial.print(S[i]);
        Serial.print(' ');
      }
//      Serial.print('\t');
//      Serial.print(currentError);
//      Serial.print(' ');
//      Serial.print(SL);
//      Serial.print(' ');
//      Serial.print(SM);
//      Serial.print(' ');
//      Serial.print(SR);
      Serial.print('\t');
      Serial.print(outOfLine);
      Serial.print('\t');
      Serial.print(blackline);
      Serial.print(' ');
      Serial.print(left90);
      Serial.print(' ');
      Serial.print(right90);
      Serial.println(' ');
//      return;
//  if((analogRead(L)>threshhold[1] && analogRead(CL)>threshhold[2]) && analogRead(C)>threshhold[3] && (analogRead(CR)>threshhold[4] && analogRead(R)>threshhold[5])){
//    leftTurn();
//  }
 
       if(currentError == -7) {                      //line loss
                      if(currentError == -7 && outOfLine==notOut) wheel(THROTTLE, THROTTLE);
     		      else {
                          hardbreak();
                          delay(30);
                          Stop();
                          while(!errorCalculation());
                          if(currentError == -7) {
                              wheel(200,200);
                              delay(30);
                              Stop();
                              delay(10);
                              if(left90 && right90) {
                                  if(leftPrefer) LeftTurn();
                                  else RightTurn();
                              }
                              else if(outOfLine == leftOut) LeftTurn();
          	              else if(outOfLine == rightOut) RightTurn();
                              
                              outOfLine = notOut;
                              left90=right90=false;
                          }
                      }
                        //wall code
      }
      else if(currentError == 0 && sum==7){ //end condition
         hardbreak();
         delay(10);
       
         bluef();   
         ender();
      }
      else if(abs(currentError)<=2 && sum<=2 && (left90 || right90)) {                  // turn on intersection
        hardbreak();
        delay(30);
        if(left90 && right90) {
          if(leftPrefer) intersection_LeftTurn();
          else RightTurn();
        }
        else if(leftPrefer && left90) intersection_LeftTurn();
        else if(!leftPrefer && right90) intersection_RightTurn();
        else wheel(200,200);
        
        outOfLine = notOut;
        left90=right90=false;
      }
          
      //
    //lineout trackers  
    if(blackline) {
      if(analogRead(MR) < threshhold[6]) {
        delay(5);
        if(analogRead(MR) < threshhold[6]) 
          outOfLine = rightOut;
      }
      if(analogRead(ML) < threshhold[0]) {
        delay(5);
        if(analogRead(ML) < threshhold[0]) 
          outOfLine = leftOut;
      }
    }
    else {
      if(analogRead(MR) > threshhold[6]) {
        delay(5);
        if(analogRead(MR) > threshhold[6]) 
          outOfLine = rightOut;
      }
      if(analogRead(ML) > threshhold[0]) {
        delay(5);
        if(analogRead(ML) > threshhold[0]) 
          outOfLine = leftOut;
      }
    }
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

int switchread()
{
  int s = 0;
  s = analogRead(Switch);
  Serial.println(s);
  delay(100);
  if(s >=920 && s < 950) return 1;
  else if(s >= 860 && s < 920) return 2;
  else if(s < 860) return 3;
  else return 0;
}

//int switchread()
//{
//  int s = 0;
//  s = analogRead(Switch);
//  Serial.println(s);
//  delay(100);
//  if(s >=0 && s < 25) return 1;
//  else if(s >= 100 && s < 500 ) return 2;
//  else if(s>=25 && s < 50 ) return 3;
//  else return 0;
//}
//







