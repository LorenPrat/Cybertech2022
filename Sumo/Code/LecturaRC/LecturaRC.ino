#define MAX 2000  // Máxima lectura del receptor.
#define MID 1500  // Lectura media del receptor.
#define MIN 1000  // Lectura mínima del receptor.

                  // Canal 1: servo (conectado al receptor).
#define CH2 4     // Canal 2: velocidad del motor.
#define MOT_1 6   // Pin 1 de dirección.
#define MOT_2 5   // Pin 2 de dirección.

#define VMIN 50   // Mínimo PWM (sensibilidad).
#define VMAX 100  // Máximo PWM (límite de velocidad).
int ch = 0;

// Función de control del motor:
void mover(int vel)
{
  // Adelante:
  if(vel >= VMIN){
    vel = (vel > VMAX) ? VMAX : vel;
    analogWrite(MOT_1, vel);
    analogWrite(MOT_2, 0);
  }
  // Atrás:
  else if(vel <= -VMIN){
    vel = (vel < -VMAX) ? -VMAX : vel;
    analogWrite(MOT_1, 0);
    analogWrite(MOT_2, -vel);    
  }
  // Parado
  else{
    analogWrite(MOT_1, 0);
    analogWrite(MOT_2, 0);  
  }
}

void setup(){
  pinMode(CH2, INPUT);
  pinMode(MOT_1, OUTPUT);
  pinMode(MOT_2, OUTPUT);
}

void loop(){  
  ch = pulseIn(CH2, HIGH);  // Lectura del receptor.
  
  // Lectura correcta:
  if(ch >= MIN && ch <= MAX){
    ch = map(ch, MIN, MAX, -255, 255);
  }
  // Lectura incorrecta:
  else{
    ch = 0;  
  }
  
  mover(ch);
}
