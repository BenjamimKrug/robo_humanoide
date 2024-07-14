#include <Arduino.h>
#include <ESP32Servo.h>

#include "servo_arrays.h"
#include "servo_compensated_control.h"

#define CONTROLE_MOSFET 32

// pernas
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

// Braços
#define L_TX_IO 5
#define R_TX_IO 4
#define L_SH_IO 17
#define R_SH_IO 2
#define L_ELB_IO 16
#define R_ELB_IO 15

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

ledc_channel_config_t servo_index_pwm_channel = {
    .gpio_num = R_TG_IO,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .timer_sel = LEDC_TIMER_0,
    .duty = 4500,
    .hpoint = 0,
};

ledc_channel_config_t servo_polinomial_pwm_channel = {
    .gpio_num = R_HP_IO,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .timer_sel = LEDC_TIMER_0,
    .duty = 4500,
    .hpoint = 0,
};

Servo servo_lib;
servo_compensated servo_polinomial;
servo_compensated servo_index;

pwm_val_t calc_duty_polinomial(servo_compensated* servo_) {
  float angle = servo_->get_cur_angle();
  pwm_val_t duty = -7.74e-4 * angle * angle + 27.1 * angle + 2360;
  Serial.printf("%i, %f\n", duty, angle);
  return duty;
}

pwm_val_t calc_duty_index(servo_compensated* servo_) {
  float angle = servo_->get_cur_angle();
  pwm_val_t duty = servo_->comp_array[static_cast<uint8_t>(angle)];
  Serial.printf("%i, %f\n", duty, angle);
  return duty;
}

void setup() {
  Serial.begin(112500);
  pinMode(CONTROLE_MOSFET, OUTPUT);
  digitalWrite(CONTROLE_MOSFET, LOW);

  servo_polinomial.begin(&ledc_timer, &servo_polinomial_pwm_channel,
                         M5_comp_array, calc_duty_polinomial);
  servo_polinomial.set_target_angle(angle);
  servo_polinomial.handle();

  servo_index.begin(&ledc_timer, &servo_index_pwm_channel, M5_comp_array,
                    calc_duty_index);
  servo_index.set_target_angle(angle);
  servo_index.handle();

  servo_lib.setPeriodHertz(50);
  servo_lib.attach(R_KN_IO, 500, 2500);
  servo_lib.write(angle);
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
        servo_polinomial.set_target_angle(angle);
        servo_index.set_target_angle(angle);
        servo_lib.write(angle);
        break;
    }
  }
  servo_polinomial.handle();
  servo_index.handle();
}