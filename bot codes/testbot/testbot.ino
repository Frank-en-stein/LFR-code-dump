//Notes to self:
//
//
//
#define leftSpeed 80
#define rightSpeed leftSpeed
#define numSensors 5
#define MAX 100
#define sharpMAX 100
#define samples 1

//runner bot//int m1 = 5, m2 = 6;
//runner bot//int  rightForward = 4, rightBack = 7,  leftForward = 8, leftBack = 9;
//maze
int m1 = 6, m2 = 5;
int  rightForward = 7, rightBack = 8,  leftForward = 3, leftBack = 4;
int lf, lb, rf, rb;
int Theshold = 100;
int reading[8], reading2[4];
int sensorpins[5]={A0, A1, A4, A6, A7};
int sensorpins2[3]={A2, A3, A5};

#define leftsensor reading[1]
#define rightsensor reading[numSensors]
#define midsensor reading[numSensors/2 +1]
#define SP (numSensors/2)*1000

long sum;
long e = 0;
double output = 0;
////////////////////////
double Kp = .8;
double Ki =  0.000;
double Kd = 1;
////////////////////////best kp=.05 kd=.3
double lasterror = 0;
int lastVal = 3000;
int lastEdge = 0;
double integral = 0;
bool white_line = false;
bool sharpturn = false;
bool array2active = true;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  pinMode(2, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  for (int i = A1; i <= A7; i++) pinMode(i, INPUT);
}

void loop() {
  double correction=0;
  for(int i=0; i<samples; i++) {
   correction += controllerOutput();
   //delay(1);
  }
  correction/=samples;
  correction-=SP;
  correction/=3000;
  integral += (int)correction;
  //if(integral > 999999) integral = 100000;
  //else if(integral < -999999) integral = -100000;
  double turn = Kp * correction + Kd * (correction - lasterror) + Ki * integral;
  Serial.print(correction);
  Serial.print(" ");
  Serial.print(integral);
  Serial.print(" ");
  Serial.print(correction - lasterror);
  Serial.print(" ");
  Serial.print(turn);
  Serial.print("\t");
  //if(reading[3] > Theshold && reading[2] < Theshold && reading[4] < Theshold) turn = 0;
  //int m1Speed = rightSpeed + (midsensor > Theshold /*&& reading[2]<Theshold && reading[4]<Theshold*/ ? 0 : turn);
  //int m2Speed = leftSpeed - (midsensor > Theshold /*&& reading[2]<Theshold && reading[4]<Theshold*/ ? 0 : turn);
  
  //int m1Speed = rightSpeed + (turn<0 ? turn:0);
  //int m2Speed = leftSpeed - (turn<0 ? 0:turn);
  
  int m1Speed = rightSpeed + (turn>0 ? (MAX-rightSpeed)*0 : (rightSpeed+MAX))*turn;
  int m2Speed = leftSpeed - (turn<0 ? (MAX-leftSpeed)*0 : (leftSpeed+MAX))*turn;
  //Serial.print(turn);
  //Serial.print(" ");
  if (sharpturn) {
    
   if (m1Speed < -sharpMAX)
      m1Speed = -sharpMAX;
    if (m2Speed < -sharpMAX)
      m2Speed = -sharpMAX;
    if (m1Speed > sharpMAX)
      m1Speed = sharpMAX;
    if (m2Speed > sharpMAX)
      m2Speed = sharpMAX;
  }
  else {
    if (m1Speed < -MAX)
      m1Speed = -MAX;
    if (m2Speed < -MAX)
      m2Speed = -MAX;
    if (m1Speed > MAX)
      m1Speed = MAX;
    if (m2Speed > MAX)
      m2Speed = MAX;
  }
  Serial.print(m1Speed);
  Serial.print(" ");
  Serial.println(m2Speed);
  
  analogWrite(m1, abs(m1Speed));
  analogWrite(m2, abs(m2Speed));
  digitalWrite(rightForward, (m1Speed >= 0 ? HIGH:LOW));
  digitalWrite(rightBack, (m1Speed < 0 ? HIGH:LOW));
  digitalWrite(leftForward, (m2Speed >= 0 ? HIGH:LOW));
  digitalWrite(leftBack, (m2Speed < 0 ? HIGH:LOW));
  lasterror = correction;
 
  
  if(m1Speed == m2Speed) Serial.println(" F");
  else if(m1Speed > m2Speed) Serial.println(" L");
  else Serial.println(" R");
  delay(25); 
}
double controllerOutput()
{
  sum = 0;
  e = 0;
  bool on_line = false;
  for (int i = 1, w = 0; i <= numSensors; i++, w += 1000)
  {
    reading[i] = (analogRead(sensorpins[i-1]) );//> Theshold ? 1000 : 0);
    if (!white_line) reading[i] = 1000 - reading[i];
    if (reading[i] > Theshold) on_line = true;
    if (reading[i] > 50) {
      sum += reading[i];
      e += (long)w * (long)reading[i];
    }
    Serial.print(reading[i]);
    Serial.print(" ");
  }
  Serial.print("\t");
  if(array2active) {
    for(int i=1, w=0; i<=3; i++, w+=1500) {
      reading2[i] = (analogRead(sensorpins2[i-1]) );//> Theshold ? 1000 : 0);
      if (!white_line) reading2[i] = 1000 - reading2[i];
      if (reading2[i] > Theshold) on_line = true;
      if (reading2[i] > 50) {
        sum += reading2[i];
        e += (long)w * (long)reading2[i];
      }
      Serial.print(reading2[i]);
      Serial.print(" ");
    }
  }
  
  if (leftsensor == 1000 && rightsensor == 0) lastEdge = -1;
  else if (leftsensor == 0 && rightsensor == 1000) lastEdge = 1;
  //detectPyramid();
  //if (midsensor == 1000 && leftsensor == 1000 && reading[2] == 0 && reading[3] == 0) reading[1] = 0;
  //if (midsensor == 1000 && rightsensor == 1000 && reading[6] == 0 && reading[5] == 0) reading[7] = 0;
  //Serial.print("\t");
  //Serial.print(lastEdge);
  //Serial.print("\t");
  if (!on_line) {
    sharpturn = true;
    if (lastEdge == -1) return -1000;//-28000;
    else return 5000;//32000;
  }
  else sharpturn = false;
  lastVal = e / sum;
  
  return lastVal;
}



