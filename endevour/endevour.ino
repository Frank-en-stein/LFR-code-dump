#include <QTRSensors.h>
#include <SoftwareSerial.h>

#include <EEPROM.h>

SoftwareSerial mySerial(3, 2);


#define NUM_SENSORS             7      // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4      // average 4 analog samples per sensor reading
#define EMITTER_PIN QTR_NO_EMITTER_PIN 
#define calSpeed 80
#define SP 3000                        //3500 for 8 sensors
#define MAX 150                       //MAX Allowed pwm motor value
#define rMAX MAX
#define rightSpeed 70
#define leftSpeed rightSpeed

// sensors 0 through 7 are connected to analog inputs 0 through 7, respectively
QTRSensorsAnalog qtra((unsigned char[]) {A1, A2, A3, A4, A5, A6, A7}, 
  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS], threshold[NUM_SENSORS];

int m1 = 5, m2 = 6;
int leftForward = 4, leftBack = 7, rightForward = 8, rightBack = 9;
//int m1 = 6, m2 = 5;
//int leftForward = 7, leftBack = 4, rightForward = 8, rightBack = 9;
int m1Speed = 100;   //right
int m2Speed = 100;   //left
int I = 0, D = 0, lasterror=0;

////////////////////////////////////////////////////////////////////.065,.08
double Ki = 0.0;
double Kp = .03;
double Kd = .0;
////////////////////////////////////////////////////////////////////300rpm: 0, .06, .1 @ 120

int leftMotorReading=0, rightMotorReading=0;
enum {wall=125, obstacle, none};
bool wall_found=false;
bool obj_found=false;

void setup()
{
  delay(500);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  
  drive(calSpeed, -calSpeed);
  for (int i = 0; i < 100; i++)  // make the calibration take about 10 seconds
  {
    qtra.calibrate();       // reads all sensors 10 times at 2.5 ms per six sensors (i.e. ~25 ms per call)
  }
  drive(0,0);
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtra.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  // print the calibration maximum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtra.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(3000);
  
  mySerial.begin(9600);
}

void loop() {
   //follow_line();
     int reading;
  if(mySerial.available()) {
    bluef();
     {
      reading = (char) mySerial.read();
      //Serial.print(reading);
      //Serial.print(" ");
      if(reading==wall) {
        wall_found = true;
        rightMotorReading =(char) mySerial.read();
        leftMotorReading =(char) mySerial.read();
      }
      else if(reading == obstacle) obj_found = true;
      else wall_found=obj_found=false;
    }
    //Serial.println();
  }
  
  Serial.print(rightMotorReading);
  Serial.print(" ");
  Serial.print(wall_found);
  Serial.print(' ');
  Serial.println(leftMotorReading);
  wheel(leftMotorReading, rightMotorReading);
}

void inch() {
  drive(90,90);
  delay(120);
  drive(0,0);
}

void follow_line()
{
  // read calibrated sensor values and obtain a measure of the line position from 0 to 5000
  // To get raw sensor values, call:
  //  qtra.read(sensorValues); instead of unsigned int position = qtra.readLine(sensorValues);
  unsigned int position = qtra.readLine(sensorValues,QTR_EMITTERS_ON,true);
  
  // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
  // 1000 means minimum reflectance, followed by the line position
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print("\t");
  }
  Serial.print("pos: "); // uncomment this line if you are using raw values
  Serial.print(position); // comment this line out if you are using raw values
  
  int error = position - SP;
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
  
  drive(m2Speed, m1Speed);
  lasterror = error;
  
  Serial.print(" ");
  Serial.print(m2Speed);
  Serial.print(" ");
  Serial.print(m1Speed); 
  if(m1Speed == m2Speed) Serial.println(" F");
  else if(m1Speed > m2Speed) Serial.println(" L");
  else Serial.println(" R");
  delay(30);
}

void drive(int m2Speed, int m1Speed) {
  analogWrite(m1, abs(m1Speed));
  analogWrite(m2, abs(m2Speed));
  digitalWrite(rightForward, (m1Speed >= 0 ? HIGH:LOW));
  digitalWrite(rightBack, (m1Speed < 0 ? HIGH:LOW));
  digitalWrite(leftForward, (m2Speed >= 0 ? HIGH:LOW));
  digitalWrite(leftBack, (m2Speed < 0 ? HIGH:LOW));
} 

void twiddle() {
  double param[3]={Kp,Ki,Kd};
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
                follow_line();
  		param[i] += dp[i];
  		err = lasterror;
  		
  		if (err<best_err)
  		{
  			best_err = err;
  			dp[i] *= 1.1;
  		} 
  		else
  		{
  			param[i] -= 2*dp[i];
  			err = lasterror;
  			
  			if (err<best_err)
  			{
  				best_err = err;
  				dp[i] *= 1.1;
  			} 
  			else
  			{
  				param[i] += dp[i];
  				dp[i] *= 0.9;
  			}
  		}
  	}		
  
  	sum = (dp[0]+dp[1]+dp[2]);
        Kp = param[0];
        Ki = 0;
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
