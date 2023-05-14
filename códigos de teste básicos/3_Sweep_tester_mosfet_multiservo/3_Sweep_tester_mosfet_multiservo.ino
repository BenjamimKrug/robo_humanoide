/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald

 modified for the ESP32 on March 2017
 by John Bennett

 see http://www.arduino.cc/en/Tutorial/Sweep for a description of the original code

 * Different servos require different pulse widths to vary servo angle, but the range is 
 * an approximately 500-2500 microsecond pulse every 20ms (50Hz). In general, hobbyist servos
 * sweep 180 degrees, so the lowest number in the published range for a particular servo
 * represents an angle of 0 degrees, the middle of the range represents 90 degrees, and the top
 * of the range represents 180 degrees. So for example, if the range is 1000us to 2000us,
 * 1000us would equal an angle of 0, 1500us would equal 90 degrees, and 2000us would equal 1800
 * degrees.
 * 
 * Circuit: (using an ESP32 Thing from Sparkfun)
 * Servo motors have three wires: power, ground, and signal. The power wire is typically red,
 * the ground wire is typically black or brown, and the signal wire is typically yellow,
 * orange or white. Since the ESP32 can supply limited current at only 3.3V, and servos draw
 * considerable power, we will connect servo power to the VBat pin of the ESP32 (located
 * near the USB connector). THIS IS ONLY APPROPRIATE FOR SMALL SERVOS. 
 * 
 * We could also connect servo power to a separate external
 * power source (as long as we connect all of the grounds (ESP32, servo, and external power).
 * In this example, we just connect ESP32 ground to servo ground. The servo signal pins
 * connect to any available GPIO pins on the ESP32 (in this example, we use pin 18.
 * 
 * In this example, we assume a Tower Pro MG995 large servo connected to an external power source.
 * The published min and max for this servo is 1000 and 2000, respectively, so the defaults are fine.
 * These values actually drive the servos a little past 0 and 180, so
 * if you are particular, adjust the min and max values to match your needs.
 */

#include <ESP32Servo.h>

Servo myservo;   // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo
// 16 servo objects can be created on the ESP32

// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33
// Possible PWM GPIO pins on the ESP32-S2: 0(used by on-board button),1-17,18(used by on-board LED),19-21,26,33-42

#define CONTROLE_MOSFET 14
#define SERVO_PWM 27
#define SERVO_PWM2 26

int pos = 0;  // variable to store the servo position
int direction = 1;

void setup() {
  Serial.begin(115200);
  pinMode(CONTROLE_MOSFET, OUTPUT);
  digitalWrite(CONTROLE_MOSFET, LOW);
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);
  myservo.attach(SERVO_PWM, 500, 2500);
  myservo2.setPeriodHertz(60);
  myservo2.attach(SERVO_PWM2);
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
  myservo.write(pos);   // tell servo to go to position in variable 'pos'
  myservo2.write(pos);  // tell servo to go to position in variable 'pos'
  delay(20);            // waits 15ms for the servo to reach the position
  if (pos == 180) {     // goes from 0 degrees to 180 degrees
    direction = -1;
    delay(15);
  }
  if (pos == 5) {  // goes from 180 degrees to 0 degrees
    direction = 1;
    delay(15);
  }
}
