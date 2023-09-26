#include <ESP32Servo.h>

#define CONTROLE_MOSFET 32
//pernas
#define L_FT_IO 26
#define R_FT_IO 23
#define L_ANK_IO 27
#define R_ANK_IO 22
#define L_KN_IO 14
#define R_KN_IO 21
#define L_TG_IO 12
#define R_TG_IO 19
#define L_HP_IO 13
#define R_HP_IO 18

//Braços
#define L_TX_IO 5
#define R_TX_IO 4
#define L_SH_IO 17
#define R_SH_IO 2
#define L_ELB_IO 16
#define R_ELB_IO 15

int pos = 0;  // variable to store the servo position
int direction = 1;
uint8_t pins[16] = { L_FT_IO, R_FT_IO, L_ANK_IO, R_ANK_IO,
                     L_KN_IO, R_KN_IO, L_TG_IO, R_TG_IO,
                     L_HP_IO, R_HP_IO, L_TX_IO, R_TX_IO,
                     L_SH_IO, R_SH_IO, L_ELB_IO, R_ELB_IO };
Servo servos[16];  // create servo object to control a servo

void instanciarServos() {
  for (uint8_t i = 0; i < 16; i++) {
    servos[i].setPeriodHertz(50);
    servos[i].attach(pins[i], 500, 2500);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(CONTROLE_MOSFET, OUTPUT);
  digitalWrite(CONTROLE_MOSFET, LOW);
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  instanciarServos();
}

void loop() {
  if (Serial.available()) {
    String recebido = Serial.readStringUntil('\n');
    if (recebido[0] == 'l')
      digitalWrite(CONTROLE_MOSFET, HIGH);
    if (recebido[0] == 'd')
      digitalWrite(CONTROLE_MOSFET, LOW);
  }

  pos += direction;
  for (uint8_t i = 0; i < 16; i++) 
    servos[i].write(pos);  // tell servo to go to position in variable 'pos'
  delay(20);           // waits 15ms for the servo to reach the position
  if (pos == 180) {    // goes from 0 degrees to 180 degrees
    direction = -1;
    delay(15);
  }
  if (pos == 5) {  // goes from 180 degrees to 0 degrees
    direction = 1;
    delay(15);
  }
}
