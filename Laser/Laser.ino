#include <Adafruit_VL6180X.h>

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0x32

// set the pins to shutdown
#define SHT_LOX1 4
#define SHT_LOX2 3
#define SHT_LOX3 2


// objects for the VL6180X
Adafruit_VL6180X lox1 = Adafruit_VL6180X();
Adafruit_VL6180X lox2 = Adafruit_VL6180X();
Adafruit_VL6180X lox3 = Adafruit_VL6180X();

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

void setup() {
  Serial.begin(9600);
  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  pinMode(SHT_LOX3, OUTPUT);

  setID();
}

void loop() {
  uint8_t range;
  uint8_t status;
  range = lox1.readRange();
  status = lox1.readRangeStatus();
  Serial.print(range);
  Serial.print("  ");
  delay(10);
  range = lox2.readRange();
  status = lox2.readRangeStatus();
  Serial.print(range);
  Serial.print("  ");
  range = lox3.readRange();
  status = lox3.readRangeStatus();
  Serial.println(range);
}
