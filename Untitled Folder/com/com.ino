#include <SoftwareSerial.h>
#include <NewPing.h>

#define MAX 100                       //MAX Allowed pwm motor value
#define rMAX 0
#define rightSpeed 70
#define leftSpeed rightSpeed


#define trigL 4
#define echoL 5
#define max_distance 40
#define obstacle_distance 18
#define trigR 8
#define echoR 9
#define trigM 6
#define echoM 7
#define SP 14


NewPing sonarL(trigL, echoL, max_distance);
NewPing sonarR(trigR, echoR, max_distance);
NewPing sonarM(trigM, echoM, max_distance);

SoftwareSerial mySerial(2, 3); // RX, TX

int m1 = 6, m2 = 5;
int leftForward = 7, leftBack = 4, rightForward = 8, rightBack = 9;
int m1Speed = 100;   //right
int m2Speed = 100;   //left
int I = 0, D = 0, lasterror=0;

int SL, SM, SR;

////////////////////////////////////////////////////////////////////.065,.08
double Ki = 0.0;
double Kp = .025;
double Kd = .0;
////////////////////////////////////////////////////////////////////

enum {wall=125, obstacle, none};

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
}
void reading(int n) {
  SL = SR = SM = 0;
  for(int i=0; i<n; i++) {
    SL += sonarL.convert_cm(sonarL.ping());
    SR += sonarR.convert_cm(sonarR.ping());
    SM += sonarM.convert_cm(sonarM.ping());
  }
  SL/=n;
  SR/=n;
  SM/=n;
}
void loop() { // run over and over
  reading(4);
 Serial.print(SR);
 Serial.print(' ');
 Serial.println(SL);
  int error=0;
  
  if(SL>0 && SM>0 && SL<max_distance && SM<SP) { 
    int m1Speed = 0, m2Speed = 70;
    mySerial.write(wall);
    mySerial.write(m1Speed);
    mySerial.write(m2Speed);
    return;
    
  }
  else if(SR>0 && SM>0 && SR<max_distance && SM<SP) {
    int m1Speed = 70, m2Speed = 0;
    mySerial.write(wall);
    mySerial.write(m1Speed);
    mySerial.write(m2Speed);
    return;
  }
  /*else if(SL>0 && SR>0 && SL<max_distance && SR<max_distance) {
    int SS = (SL+SR)/2;
    if(SS > 20 || SS < 10) SS = 14;
    error = (SL - SS) % 4;
    f(error*1000);
  }*/
  if(SL>0 && SL<max_distance) {
    error = (SL - SP) % 4;
    f(error*1000);
  }
  else if(SR>0 && SR<max_distance) {
    error = (SP - SR) % 4;
    f(error*1000);
  }
  else if(SM>0 && SM<obstacle_distance) {
    reading(5);
    if(SM<obstacle_distance) {
      mySerial.write(obstacle);
      Serial.print(SM);
  Serial.print(" ");
      
    }
    
  }
  else  {
    mySerial.write(none);
  }
}

void f(int error) {
  I+=error;
  D=error-lasterror;
  double turn = Kp*error + Kd*D + Ki*I;
  
  int m1Speed = rightSpeed + (turn<0 ? turn:0);    //right motor speed
  int m2Speed = leftSpeed - (turn<0 ? 0:turn);     //left motor Speed
  
  
  //int m1Speed = rightSpeed + turn;
  //int m2Speed = leftSpeed - turn;
  
  if (m1Speed < -rMAX)
      m1Speed = -rMAX;
  if (m2Speed < -rMAX)
      m2Speed = -rMAX;
  if (m1Speed > MAX)
      m1Speed = MAX;
  if (m2Speed > MAX)
      m2Speed = MAX;

  lasterror = error;  
  
  
  /*Serial.print(m1Speed);
  Serial.print(" ");
  Serial.println(m2Speed);*/
  
  mySerial.write(wall);
  mySerial.write(m1Speed);
  mySerial.write(m2Speed);
  
}
