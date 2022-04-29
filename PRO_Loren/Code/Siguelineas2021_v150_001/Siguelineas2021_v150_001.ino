#include <QTRSensors.h>

#define pinPWMA  11
#define pinAIN2  12
#define pinAIN1  10
#define pinBIN1  8
#define pinBIN2  7
#define pinPWMB  6
#define pinSTBY  9

//MOTORS//
const int pinMotorA[3] = { pinPWMA, pinAIN2, pinAIN1 };
const int pinMotorB[3] = { pinPWMB, pinBIN1, pinBIN2 };
//QTR SENSORS//
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
uint16_t sensorValuesm[SensorCount];
//QTR LATERAL//
const uint8_t SensorCountl = 1;
uint16_t sensorValuesl[SensorCountl];
//PID VALUES//
float kp=5.5, kd=0.55, ki=0; //35
//PROGRAM VARIABLES//
float vn=150;
float cor=0, e=0, eprev=0, sum=0;
float pos=0;
float reg=0;
float va=0, vb=0;
int startstop = 0;
int flag=0;
int flagm=0;
float timer=0;


//Funciones que controlan el vehiculo
void Forward(float speedA, float speedB){
    moveMotorForward(pinMotorA, speedA);
    moveMotorForward(pinMotorB, speedB);
}

void Back(float speedA, float speedB){
    moveMotorBackward(pinMotorA, speedA);
    moveMotorBackward(pinMotorB, speedB);
}

void turnRigth(float rotspeed){
    moveMotorForward(pinMotorA, rotspeed);
    moveMotorBackward(pinMotorB, rotspeed);
}

void turnLeft(float rotspeed){
    moveMotorBackward(pinMotorA, rotspeed);
    moveMotorForward(pinMotorB, rotspeed);
}

void Stop(){
  disableMotors();
  stopMotor(pinMotorA);
  stopMotor(pinMotorB);
  enableMotors();
}

//Funciones que controlan los motores
void moveMotorForward(const int pinMotor[3], int Speed){
  digitalWrite(pinMotor[1], HIGH);
  digitalWrite(pinMotor[2], LOW);
  analogWrite(pinMotor[0], Speed);
}
void moveMotorBackward(const int pinMotor[3], int Speed){
  digitalWrite(pinMotor[1], LOW);
  digitalWrite(pinMotor[2], HIGH);
  analogWrite(pinMotor[0], Speed);
}
void stopMotor(const int pinMotor[3]){
  digitalWrite(pinMotor[1], LOW);
  digitalWrite(pinMotor[2], LOW);
  analogWrite(pinMotor[0], 0);
}
void enableMotors() {digitalWrite(pinSTBY, HIGH);}
void disableMotors(){digitalWrite(pinSTBY, LOW);}

float regulator(float e){
  cor = (kp*e) + (kd*(e- eprev)) + (ki*sum);
  eprev = e;
  sum = sum + e;
  return cor;
  }

QTRSensors qtr;
QTRSensors qtrl;

void setup()
{
  Serial.begin(9600);
  //motors
  pinMode(pinAIN2, OUTPUT);
  pinMode(pinAIN1, OUTPUT);
  pinMode(pinPWMA, OUTPUT);
  pinMode(pinBIN1, OUTPUT);
  pinMode(pinBIN2, OUTPUT);
  pinMode(pinPWMB, OUTPUT);
  //qtr sensors
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  pinMode(13, OUTPUT);


  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);
  
  // calibration
  for (uint8_t i = 0; i < 200; i++)
  {
    qtr.calibrate();
  }
  
  digitalWrite(13, HIGH);
  
  enableMotors();
}

void loop()
{ 

    pos = qtr.readLineBlack(sensorValues); 
    e= (pos-3500)/100;
    reg = regulator (e);
    va = vn + reg;
    vb = vn - reg;
    if (va>=255){
      va=255;
    }
    if (va<0){
      va=0;
    }
    if (vb>=255){
      vb=255;
    }
    if (vb<0){
      vb=0;
    }
    Forward(va,vb);
}
