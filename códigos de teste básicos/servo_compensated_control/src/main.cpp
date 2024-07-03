#include <Arduino.h>

#include "servo_compensated_control.h"

#define CONTROLE_MOSFET 32
#define R_HP_IO 18

#define MG995_MIN 0
#define MG995_MAX 180

float angle = 90.0f;

ledc_timer_config_t ledc_timer = {
    .speed_mode = LEDC_LOW_SPEED_MODE,     // timer mode
    .duty_resolution = LEDC_TIMER_16_BIT,  // resolution of PWM duty
    .timer_num = LEDC_TIMER_0,             // timer index
    .freq_hz = 50,                         // frequency of PWM signal
    .clk_cfg = LEDC_AUTO_CLK,              // Auto select the source clock
};

ledc_channel_config_t servo_pwm_channel = {
    .gpio_num = R_HP_IO,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .timer_sel = LEDC_TIMER_0,
    .duty = 4500,
    .hpoint = 0,
};

servo_compensated servo;
pwm_val_t duty_offset = 0;

pwm_val_t calc_duty(servo_compensated* servo_) {
  float angle = servo_->get_cur_angle();
  pwm_val_t duty = servo_->comp_array[static_cast<uint16_t>(servo_->get_cur_angle())];
  Serial.printf("%i, %f\n", duty, angle);
  return duty;
}

void setup() {
  Serial.begin(112500);
  pinMode(CONTROLE_MOSFET, OUTPUT);
  digitalWrite(CONTROLE_MOSFET, LOW);
  servo.begin(&ledc_timer, &servo_pwm_channel, servo_comp_array, calc_duty);
  servo.set_target_angle(angle);
  servo.handle();
}

void loop() {
  if (Serial.available()) {
    char recebido[40];
    uint8_t i = 0;
    for (; i < sizeof(recebido) && Serial.available(); i++)
      recebido[i] = Serial.read();

    switch (recebido[0]) {
      case 'l':
        digitalWrite(CONTROLE_MOSFET, HIGH);
        Serial.println("ligou");
        return;

      case 'd':
        digitalWrite(CONTROLE_MOSFET, LOW);
        Serial.println("desligou");
        return;

      case 'a':
        recebido[i] = 0;
        angle = atof(&recebido[1]);
        servo.set_target_angle(angle);
        break;

      case 'o':
        duty_offset = atoi(&recebido[1]);
        servo.update_duty();
        break;
    }
  }
  servo.handle();
}