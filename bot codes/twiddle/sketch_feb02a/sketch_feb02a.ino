//Notes to self:
//
//
//
#define leftSpeed 150
#define rightSpeed leftSpeed
#define numSensors 5
#define MAX 180
#define sharpMAX 100

//runner bot//int m1 = 5, m2 = 6;
//runner bot//int  rightForward = 4, rightBack = 7,  leftForward = 8, leftBack = 9;
//maze
int m1 = 6, m2 = 5;
int  rightForward = 7, rightBack = 8,  leftForward = 3, leftBack = 4;
int lf, lb, rf, rb;
int Theshold = 500;
int reading[8];
int sensorpins[5]={A0, A1, A4, A6, A7};

#define leftsensor reading[1]
#define rightsensor reading[numSensors]
#define midsensor reading[numSensors/2 +1]
#define SP (numSensors/2)*1000

long sum;
long e = 0;
double output = 0;
////////////////////////
double Kp = .0;
double Ki = .00;
double Kd = 0;
////////////////////////best kp=.05 kd=.3
double lasterror = 0;
int lastVal = 3000;
int lastEdge = 0;
double integral = 0;
bool white_line = false;
bool sharpturn = false;
void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  pinMode(2, OUTPUT);
  pinMode(12, OUTPUT);
  for (int i = A1; i <= A7; i++) pinMode(i, INPUT);
  delay(5000);
  twiddle();
}

void loop()
{
  //follow_line();
  int m1Speed = 0;
  int m2Speed = 0;
  analogWrite(m1, 0);
  analogWrite(m2, 0);
  digitalWrite(rightForward, (m1Speed >= 0 ? HIGH:LOW));
  digitalWrite(rightBack, (m1Speed < 0 ? HIGH:LOW));
  digitalWrite(leftForward, (m2Speed >= 0 ? HIGH:LOW));
  digitalWrite(leftBack, (m2Speed < 0 ? HIGH:LOW));
}

void twiddle() {
  double param[3]={0.0,0.0,0.0};
  double dp[3]={1,1,1};
  double best_err = 1000;
  double err;
  double sum = (dp[0]+dp[1]+dp[2]);
  while(sum>.0000000001)
  {
        if(sum>.0000000001) digitalWrite(12,HIGH);
        else digitalWrite(12,LOW);
  	for (int i=0;i<3;i++)
  	{
  		param[i] += dp[i];
  		err = follow_line();
  		
  		if (err<best_err)
  		{
  			best_err = err;
  			dp[i] *= 1.4;
  		} 
  		else
  		{
  			param[i] -= 2*dp[i];
  			err = follow_line();
  			
  			if (err<best_err)
  			{
  				best_err = err;
  				dp[i] *= 1.4;
  			} 
  			else
  			{
  				param[i] += dp[i];
  				dp[i] *= 0.6;
  			}
  		}
  	}		
  
  	sum = (dp[0]+dp[1]+dp[2]);
        Kp = param[0];
        Ki = param[1];
        Kd = param[2];
        Serial.print(Kp);
        Serial.print(" ");
        Serial.print(Ki);
        Serial.print(" ");
        Serial.print(Kd);
        Serial.print("/t");
        Serial.print(sum);
        Serial.print(" ");
        Serial.println(err);
  }
  digitalWrite(12,LOW);
}

double follow_line() {
  double correction = controllerOutput();
  integral += (double)correction;
  double turn = Kp * correction + Kd * (correction - lasterror) + Ki * integral;
  //Serial.print(correction);
  //Serial.print(" ");
  //Serial.print(turn);
  //Serial.print(" ");
  //int m1Speed = rightSpeed + (midsensor > Theshold /*&& reading[2]<Theshold && reading[4]<Theshold*/ ? 0 : turn);
  //int m2Speed = leftSpeed - (midsensor > Theshold /*&& reading[2]<Theshold && reading[4]<Theshold*/ ? 0 : turn);
  int m1Speed = rightSpeed + turn;//(turn<0 ? turn:0);
  int m2Speed = leftSpeed - turn;//(turn<0 ? 0:turn);
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
  //Serial.print(m1Speed);
  //Serial.print(" ");
  //Serial.println(m2Speed);
  
  analogWrite(m1, abs(m1Speed));
  analogWrite(m2, abs(m2Speed));
  digitalWrite(rightForward, (m1Speed >= 0 ? HIGH:LOW));
  digitalWrite(rightBack, (m1Speed < 0 ? HIGH:LOW));
  digitalWrite(leftForward, (m2Speed >= 0 ? HIGH:LOW));
  digitalWrite(leftBack, (m2Speed < 0 ? HIGH:LOW));
  lasterror = correction;
 
  
  //if(m1Speed == m2Speed) Serial.println(" F");
  //else if(m1Speed > m2Speed) Serial.println(" L");
  //else Serial.println(" R");
  delay(50);
  
  return correction;
}
double controllerOutput()
{
  sum = 0;
  e = 0;
  bool on_line = false;
  for (int i = 1, w = -4; i <= numSensors; i++, w += 2)
  {
    reading[i] = (analogRead(sensorpins[i-1]) > Theshold ? 1000 : 0);
    if (!white_line) reading[i] = 1000 - reading[i];
    if (reading[i] > Theshold) on_line = true;
    if (reading[i] > 50) {
      sum += reading[i];
      e += (long)w * (long)reading[i];
    }
    //Serial.print(reading[i]);
    //Serial.print(" ");

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
    if (lastEdge == -1) return -6;//-28000;
    else return 6;//32000;
  }
  else sharpturn = false;
  lastVal = e / sum;
  
  return lastVal;
}



