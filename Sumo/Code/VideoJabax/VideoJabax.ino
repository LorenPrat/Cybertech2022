#define N 5
#define CH1 A0
#define CH2 A5
#define CH3 A2
#define CH4 A3
#define CH5 A4

int ch[N];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH5, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  ch[0] = pulseIn(CH1, HIGH); //(joystick dcho) giro dcha 1100 izq 1800 medio 1450
  ch[1] = pulseIn(CH2, HIGH); //(joystick dcho) sentido arriba 1100 abajo 1900 medio 1500
  ch[2] = pulseIn(CH3, HIGH); //(joystick izq) pwm arriba 1900/ abajo 1100/ centro 1500-1600
  ch[3] = pulseIn(CH4, HIGH); //(joystick izq) pwm derecha 1100 izquierda 1800
  ch[4] = pulseIn(CH5, HIGH); // nada es in canal que te cambia un valor de 100 a 1500 y está en un gatillo

  Serial.print(ch[0]);
  Serial.print(" , ");
  Serial.print(ch[1]);
  Serial.print(" , ");
  Serial.print(ch[2]);
  Serial.print(" , ");
  Serial.print(ch[3]);
  Serial.print(" , ");
  Serial.println(ch[4]);
}
