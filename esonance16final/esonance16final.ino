#include <EEPROM.h>

#define Kp 55
#define Kd 5000
#define Ki 0.0

#define MAX 250
#define revMAX 50

#define THROTTLE 190
#define SAFE_THROTTLE 90

#define ML A7
#define L A6
#define CL A5
#define C A4
#define CR A3
#define R  A2
#define MR A1

#define blue 10
#define red  11


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
bool emergencyBrakeOn = true;
bool turnfed = true;

int ar[2][2][2][2][2][2][2];
int sw,sw2;
int integral=0;
int speedL = THROTTLE;
int speedR = THROTTLE;

int MLeft,MRight;
int sum=0;
int threshhold[7];
int s[7];
bool S[7];
int white[] = {
  1024 , 1024 , 1024 , 1024 , 1024 , 1024 , 1024};
int black[] = {
  -1, -1, -1, -1, -1, -1, -1};
double currentError = 0, diffError = 0, previousError = 0, sp=0.0;

double pwm;
int lineOut,backuplineOut = notOut;
int outOfLine = notOut;
double thresh_const = .65;

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

void loop() { //testjob();return;
  lineFollowing();
  watchdog();
  redf(emergencyBrakeOn);
//      for (int i = 0; i < 7; i++) {
//        Serial.print(S[i]);
//        Serial.print(' ');
//      }
//      Serial.print('\t');
//      Serial.print(currentError);
//      Serial.print(outOfLine);
//      Serial.print(' ');
//      Serial.print(backuplineOut);
//      Serial.print('\t');
//      Serial.print(blackline);
//      Serial.print(' ');
//      Serial.print(left90);
//      Serial.print(' ');
//      Serial.print(right90);
//      Serial.println(' ');
//     return; 

  if(currentError == 0 && sum==7) {                   //end condition
    emergencybrake();
    delay(30);    
    while(!errorCalculation());
    if(sum==7){
      wheel(200,200);
      delay(10);
      hardbrake();
      delay(10);
      while(!errorCalculation());
      if(sum==7){  //end condition
        wheel(200,200);
        delay(10);
        stop();
        delay(20);
        while(1){
          bluef();
        }
      }
    }
    //redf(1);  
    if(sum==0) nolineTurnSelect();
    else intersectionTurnSelect();
  }
  else if(sum>=5) {
    emergencybrake();
    delay(30);   
    while(!errorCalculation());
    //redf(1);
    if(sum==0) nolineTurnSelect();
    else intersectionTurnSelect();
  }
  else if(left90 || right90) {
    emergencybrake();
    delay(30);   
    while(!errorCalculation());
    if(sum==0) nolineTurnSelect();
    else intersectionTurnSelect();  
  }
  else 
  if(currentError == -7) {                      //line loss
    if(currentError == -7 && outOfLine==notOut && backuplineOut==notOut) wheel(THROTTLE, THROTTLE);
    else {
      emergencybrake();
      delay(30);
      while(!errorCalculation());
      if(currentError == -7) {
        nolineTurnSelect();
      }
    }
  }

}

void testjob() {
  int t;
  while(1) {
    t = micros();
    watchdog();
    t = micros()-t;
    Serial.println(t);
  }
}

void watchdog() {
  int ii=13;
  bool SS[7] = {0,0,0,0,0,0,0};
  while(ii--) {
    if(!errorCalculation()) {
      lineFollowing();
      return;
    }
    else {
      for(int i=0; i<7; i++) SS[i]|=S[i];
      if(SS[0]+SS[1]+SS[2]+SS[3]+SS[4]+SS[5]+SS[6]==7) right90=left90=true;
      else if(SS[6]+SS[5]+SS[4]+SS[3]==4) left90=!(right90=true);
      else if(SS[0]+SS[1]+SS[2]+SS[3]==4) right90=!(left90=true);
      if(right90 || left90) {
        emergencybrake();
        delay(10000);
        break;
      }
    }
  }
  
}

