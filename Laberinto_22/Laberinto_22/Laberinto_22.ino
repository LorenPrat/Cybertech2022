#include <Adafruit_VL6180X.h>

#define pinPWMA  11
#define pinAIN2  12
#define pinAIN1  10
#define pinBIN1  8
#define pinBIN2  7
#define pinPWMB  6
#define pinSTBY  9

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x32

// set the pins to shutdown
#define SHT_LOX1 3  //Izquierda, a ajustar, que no se en que orden los puse,
#define SHT_LOX2 2  //Centro
#define SHT_LOX3 4  //Derecha


// objects for the VL6180X
Adafruit_VL6180X lox1 = Adafruit_VL6180X();
Adafruit_VL6180X lox2 = Adafruit_VL6180X();
Adafruit_VL6180X lox3 = Adafruit_VL6180X();

//MOTORS//
const int pinMotorA[3] = { pinPWMA, pinAIN2, pinAIN1 };
const int pinMotorB[3] = { pinPWMB, pinBIN2, pinBIN1 };

//PID VALUES//
float kp=0.45, kd=0.1, ki=0; //80mm centro, 1mm minimo que lee, 100 de vel y maximo que regulamos - 80/100
//PROGRAM VARIABLES//
float vn=60; 
float cor=0, e=0, eprev=0, sum=0;
float pos=0;
float reg=0;
float va=0, vb=0, vcb=30;
int Lectura[3];
int Centro = 40;  //Medidas en mm
int Limite = 30;
int Limite2 = 30;
int T = 20000;
int flag = 0;
float Timer = 0;


//Funciones que controlan el vehiculo
void Forward(int speedA, int speedB){
    moveMotorForward(pinMotorA, speedA);
    moveMotorForward(pinMotorB, speedB);
}

void Back(int speedA, int speedB){
    moveMotorBackward(pinMotorA, speedA);
    moveMotorBackward(pinMotorB, speedB);
}

void turnRight(int rotspeed){
    moveMotorForward(pinMotorA, rotspeed);
    moveMotorBackward(pinMotorB, rotspeed);
}

void turnLeft(int rotspeed){
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

void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  digitalWrite(SHT_LOX3, HIGH);
  delay(10);
  // activating LOX1
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  lox1.begin();
  lox1.setAddress(LOX1_ADDRESS);
  delay(10);
  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);
  lox2.begin();
  lox2.setAddress(LOX2_ADDRESS);
  // activating LOX3
  digitalWrite(SHT_LOX3, HIGH);
  delay(10);
  lox3.begin();
  lox3.setAddress(LOX3_ADDRESS);
}

void setup()
{
  Serial.begin(9600);
  pinMode(pinAIN2, OUTPUT);
  pinMode(pinAIN1, OUTPUT);
  pinMode(pinPWMA, OUTPUT);
  pinMode(pinBIN1, OUTPUT);
  pinMode(pinBIN2, OUTPUT);
  pinMode(pinPWMB, OUTPUT);
  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  pinMode(SHT_LOX3, OUTPUT);

  setID();
  do {
    disableMotors ();
    Lectura[1] = lox2.readRange();
  }
  while(Lectura[1] < 40);
  enableMotors();
}

void loop() {
  Lectura[0] = lox1.readRange();
  Lectura[1] = lox2.readRange();
  Lectura[2] = lox3.readRange();
  
    e = Lectura[0] - Lectura[2];
    reg = regulator(e);
    va = vn - vcb + reg;
    vb = vn - reg;
  
      
  if(va>255){va=254;}
  if(vb>255){vb=254;}
  if(va<0){va=0;}
  if(vb<0){vb=0;}
  

 
/*  if (Lectura[1] >= Limite){
    Back(vn,vn);
    delay(1);
  if ((Lectura[1] >= Limite) && (Lectura[0] >= Limite2) && (Lectura[2] >= Limite2)) {
      turnRight(vn);
      delay (20);
    }
  if((Lectura[1] >= Limite) && (Lectura[0] <= Limite2)){
    turnRight(vn);
    delay (45);
   }
  } */
  else {  
  Forward (va,vb);
}
}
