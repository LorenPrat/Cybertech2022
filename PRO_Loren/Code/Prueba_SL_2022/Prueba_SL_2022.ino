#include <QTRSensors.h>
/*
###Código diseñado de manera que:###

      MOTOR A = MOTOR IZQUIERDO
      MOTOR B = MOTOR DERECHO
      
###Siguelineas laterales:###

      4 = SENSOR QTR IZQUIERDO
      5 = SENSOR QTR DERECHO

###Aclaraciones###

      Bandas de linea a la derecha = Inicio/Final
      Bandas de linea a la izquierda = Cambio de curvatura
      
*/
#define pinPWMA  11
#define pinAIN2  12
#define pinAIN1  10
#define pinBIN1  8
#define pinBIN2  7
#define pinPWMB  6
#define pinSTBY  9

#define vnomin 100

//MOTORS//
const int pinMotorA[3] = { pinPWMA, pinAIN2, pinAIN1 };
const int pinMotorB[3] = { pinPWMB, pinBIN1, pinBIN2 };
//QTR SENSORS//
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
//QTR LATERAL//
const uint8_t SensorCountl = 1;
uint16_t sensorValuesl[SensorCountl];
//PID VALUES//
float kp=2, kd=0, ki=0; //35
//PROGRAM VARIABLES//
float vn=70; //vn=100;
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
void printe(){
  Serial.print(startstop);
  Serial.print('\t');
  Serial.print(sensorValuesl[0]);
  Serial.print('\t');
  Serial.print(pos);
  Serial.print('\t');
  Serial.print("-----");
  Serial.print('\t');
  Serial.print(sensorValues[0]);
  Serial.print('\t');
  Serial.print(sensorValues[1]);
  Serial.print('\t');
  Serial.print(sensorValues[2]);
  Serial.print('\t');
  Serial.print(sensorValues[3]);
  Serial.print('\t');
  Serial.print(sensorValues[4]);
  Serial.print('\t');
  Serial.print(sensorValues[5]);
  Serial.print('\t');
  Serial.print(sensorValues[6]);
  Serial.print('\t');
  Serial.print(sensorValues[7]);
  Serial.print('\n');
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
  //qtr laterales
  pinMode(5, INPUT);    //RIGHT
  // led pin
  pinMode(13, OUTPUT);

  // configure the sensors
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5, A6, A7}, SensorCount);
  
  // lateral sensors
  qtrl.setTypeRC();
  qtrl.setSensorPins((const uint8_t[]){5}, SensorCountl);          //5 right sensor //
  
  // calibration
  for (uint16_t i = 0; i < 200; i++)
  {
    qtr.calibrate();
  }
  
  digitalWrite(13, HIGH);
  
  enableMotors();
}

void loop()
{ 
    qtrl.read(sensorValuesl);                // black = 2500 // white = 0
    pos = qtr.readLineBlack(sensorValues);   //centre=3500;  line to the left=0;  line to the right=7000//
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
    Forward (va,vb);   
  printe();

}
