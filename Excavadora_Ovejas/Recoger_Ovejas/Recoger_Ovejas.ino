#define pinPWMA  11
#define pinAIN2  12
#define pinAIN1  10
#define pinBIN1  8
#define pinBIN2  7
#define pinPWMB  6
#define pinSTBY  9

#define SIZ A0
#define SAL A1
#define SDE A2

//MOTORS//
const int pinMotorA[3] = { pinPWMA, pinAIN2, pinAIN1 };
const int pinMotorB[3] = { pinPWMB, pinBIN1, pinBIN2 };

//PID VALUES//
float kp=0.25, kd=0, ki=0; //Voy a asumir que no habra que tocar mucho esto, pero no se que valores nos dara el sensor nuevo
//PROGRAM VARIABLES//
float vn=50; //vn=100, despacito y con buena letra
float cor=0, e=0, eprev=0, sum=0;
float pos=0;
float reg=0;
float va=0, vb=0;
int Lectura[3];
int Centro = 320; //Toca definir la distancia a la pared otra vez, sheit
int Aumento = 320; //Lo que aumenta, pero tambien la distancia a la pared en el centro, por no meter mas variables
int Limite = 280; //Distancia maxim que miden los sensores? La pista es grande
int Limite2 = 280; //Distancia para que de marcha atras
int LIM = 800; //Distancia maxima al exterior, para que no se coma el centro o deje de leer
int T = 0;
int flag = 1; //Para el barrido exterior, que siempre se cumpla una vuelta o lo que sea
int flag2 = 0; //Para los limites
float Timer = 0;
int Vuelta = 20000; //Tiempo que tarda en dar una vuelta y media, mas o menos, toca medirlo a ojo
int aux = 0;
int aux2 = 0; //Me odio por tener que hacer esto, 2 auxiliares, fack. Se podria quitar uno?
int aux3 = 0; //Sin comentarios, era necesario
int Rumbo = 0; //Para saber si esta barriendo en perimetro o de camino a casa


//Funciones que controlan el vehiculo
void Forward(float speedA, float speedB){
    moveMotorForward(pinMotorA, speedA);
    moveMotorForward(pinMotorB, speedB);
}

void Back(float speedA, float speedB){
    moveMotorBackward(pinMotorA, speedA);
    moveMotorBackward(pinMotorB, speedB);
}

void turnRight(float rotspeed){
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

int LEER(){
  //Leer el sensor de la linea, no me acuerdo de la funcion
  //Vamos a tope, confiamos
  aux = SensorRead o algo asi
  if (aux > 1500){return 0;}
  else {return 1;}
}

void Leer(int Lectura[3]){
  //Leer los sensores de distancia, espero que marta sepa...

  
}

void VolverCentro () {
  //Giro a 90, recto hasta pared y rezar que no se pierda
  Back(vn,vn);
  delay(1);
  turnRight(vn);
  delay(50);
  Forward(vn,vn);
  delay(100); //Ajustar el tiempo para que llegue a la pared bien
}

void DejarCarga () {
  delay (200); //Como para que acabe el giro y entre, a ajustar
  Back(vn,vn);
  delay(1);
  turnRight(vn);
  delay(100);
  Forward(vn,vn);
  delay(200); //Y salimos de la zona  
}

void SeguirPared (int aux3){
  Leer(Lectura);
  e = aux3 - Lectura[0];
  reg = regulator(e);
  va = vn + reg;
  vb = vn - reg;
  if(va>255){va=255;}
  if(vb>255){vb=255;}
  if(va<25){va=25;}
  if(vb<25){vb=25;}
  Forward (va,vb);

 //No se porque hay dos limites, =(, podriamos eliminar uno, solo nos interesa el de delante no?
  if (Lectura[1] >= Limite){
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
  }
}

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
  //Sharps
  pinMode(A0, INPUT); //Los de distancia
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(D5, INPUT); //El de la linea
  
  Timer= millis();
  enableMotors();
  Forward (vn,vn);
  delay(2000); //Para que vaya recto y salga al comienzo del centro, a ajustar como todo =(
}

void loop() {
    T = millis();
        
    if ((T-Timer) > (flag*Vuelta)){ //Ajustar tiempos para que sea como vuelta y media, para que el regreso sea por la parte de atras
      flag++;                       // De esta manera nos aseguramos que no llegue al centro por delante, eso seria malo supongo
      Rumbo = 1;
      Centro = Centro + Aumento; //Para que cada vuelta sea mas lejos de la pared externa
      if (Centro > LIM) { // Para que vaya se acerque al interior y luego se aleje, hasta que se acabe el tiempo
        Centro = LIM;
        flag2 = 1;
      }
      if (Centro < Aumento) {
        Centro = -Aumento;
        flag2 = 1;
      }
      if (flag2 = 1) {
        Aumento = -Aumento;
        flag = 0
    }
    
    if (Rumbo == 0){SeguirPared(Centro);}

    if (Rumbo == 1){
      if (aux2 == 0) {
      VolverCentro(); //Solo una vez claro
      aux2 = 1;
      }
      Rumbo = LEER();
      SeguirPared(Aumento);
      if (Rumbo == 0){
        DejarCarga();
        aux2 = 0;
        Timer = millis();
      }
    }   

}
