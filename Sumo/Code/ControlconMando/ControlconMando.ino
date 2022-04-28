/*
###Código diseñado de manera que:###

      MOTOR A = MOTOR IZQUIERDO
      MOTOR B = MOTOR DERECHO
*/

// JOSI WAS HERE

#define N 5
#define CH1 A0
#define CH2 A5
#define CH3 A2
#define CH4 A3
#define CH5 A4

#define pinPWMA  11
#define pinAIN2  12
#define pinAIN1  10
#define pinBIN1  8
#define pinBIN2  7
#define pinPWMB  6
//#define pinENA  9

#define u 125

int vel = 0;
int veld = 0;
int reg = 0;
int veli = 0;
int ch[N];

const int pinMotorA[3] = { pinPWMA, pinAIN2, pinAIN1 };
const int pinMotorB[3] = { pinPWMB, pinBIN1, pinBIN2 };

//Funciones que controlan el vehiculo
void Forward(int speedA, int speedB)
{
    moveMotorForward(pinMotorA, speedA);
    moveMotorForward(pinMotorB, speedB);
}

void Back(int speedA, int speedB)
{
    moveMotorBackward(pinMotorA, speedA);
    moveMotorBackward(pinMotorB, speedB);
}

void turnRight(int rotspeed)
{
    moveMotorForward(pinMotorA, rotspeed);
    moveMotorBackward(pinMotorB, rotspeed);
}

void turnLeft(int rotspeed)
{
    moveMotorBackward(pinMotorA, rotspeed);
    moveMotorForward(pinMotorB, rotspeed);
}

void Stop()
{
  //disableMotors();
  stopMotor(pinMotorA);
  stopMotor(pinMotorB);
  //enableMotors();
}

//Funciones que controlan los motores
void moveMotorForward(const int pinMotor[3], int Speed)
{
  digitalWrite(pinMotor[1], HIGH);
  digitalWrite(pinMotor[2], LOW);
  analogWrite(pinMotor[0], Speed);
}
void moveMotorBackward(const int pinMotor[3], int Speed)
{
  digitalWrite(pinMotor[1], LOW);
  digitalWrite(pinMotor[2], HIGH);
  analogWrite(pinMotor[0], Speed);
}
void stopMotor(const int pinMotor[3])
{
  digitalWrite(pinMotor[1], LOW);
  digitalWrite(pinMotor[2], LOW);
  analogWrite(pinMotor[0], 0);
}
//void enableMotors() {digitalWrite(pinENA, HIGH);}
//void disableMotors(){digitalWrite(pinENA, LOW);}


void setup()
{
  Serial.begin(9600);
  pinMode(pinAIN2, OUTPUT);
  pinMode(pinAIN1, OUTPUT);
  pinMode(pinPWMA, OUTPUT);
  pinMode(pinBIN1, OUTPUT);
  pinMode(pinBIN2, OUTPUT);
  pinMode(pinPWMB, OUTPUT);
  //pinMode(pinENA, OUTPUT);
  //digitalWrite (pinENA,HIGH);
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH5, INPUT);

  //enableMotors();
}

void loop()
{
  //leer canales
  ch[0] = pulseIn(CH1, HIGH); //(joystick dcho) giro dcha 1100 izq 1800 medio 1450
  ch[1] = pulseIn(CH3, HIGH); //(joystick dcho) sentido arriba 1100 abajo 1900 medio 1500
  ch[2] = pulseIn(CH2, HIGH); //(joystick izq) pwm arriba 1900/ abajo 1100/ centro 1500-1600
  ch[3] = pulseIn(CH4, HIGH); //(joystick izq) pwm derecha 1100 izquierda 1800 medio 1450
  ch[4] = pulseIn(CH5, HIGH); // nada es in canal que te cambia un valor de 100 a 1500 y está en un gatillo
  
  //Cálculo de pwm nominal al que mover robot en función de lo leído
  vel = map (ch[2],1850,1150,0,250);
  if (vel>250) vel=250;
  else if (vel<0) vel=0;
  //Cálculo de los pwm a cada motor distinto
  reg = map (ch[0],1800,1150,vel*0.5,-vel*0.5);
  veli = vel - reg;
  veld = vel + reg;
  
  if (veld>250) veld=250;
  else if (veld<0) veld=0;
  
  if (veli>250) veli=250;
  else if (veli<0) veli=0;

  Serial.println(veld);

  // giros y sentido
  if (ch[3]>=1650) {
    turnRight(vel); //giro en eje con joy izq
    //Serial.println("giro izquierda");
    }
  else if (ch[3]<=1250) {
    turnLeft(vel); //giro en eje con joy izq
    //Serial.println("giro derecha");
    }
    else if (ch[1]<=1350) {
      Forward (veli, veld);
      //Serial.println("marcha atrás");
      }
      else if (ch[1]>=1650){
        Back (veli,veld);
        //Serial.println("marcha alante");
        }
        else Stop();
  
  
}
