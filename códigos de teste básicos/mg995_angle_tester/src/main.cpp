#include <Arduino.h>

#include "driver/ledc.h"

#define CONTROLE_MOSFET 32
#define R_HP_IO 18

#define MG995_MIN 1638
#define MG995_MAX 8192

uint16_t pwm_val = 4915;

ledc_timer_config_t ledc_timer = {
    .speed_mode = LEDC_HIGH_SPEED_MODE,    // timer mode
    .duty_resolution = LEDC_TIMER_16_BIT,  // resolution of PWM duty
    .timer_num = LEDC_TIMER_0,             // timer index
    .freq_hz = 50,                         // frequency of PWM signal
    .clk_cfg = LEDC_AUTO_CLK,              // Auto select the source clock
};

ledc_channel_config_t servo_pwm_channel = {
    .gpio_num = R_HP_IO,
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .timer_sel = LEDC_TIMER_0,
    .duty = pwm_val,
    .hpoint = 0,
};

void setup() {
  Serial.begin(112500);
  pinMode(CONTROLE_MOSFET, OUTPUT);
  digitalWrite(CONTROLE_MOSFET, LOW);
  ledc_timer_config(&ledc_timer);
  ledc_channel_config(&servo_pwm_channel);
}

void loop() {
  if (Serial.available()) {
    char recebido[40];
    uint8_t i = 0;
    for (; i < sizeof(recebido) && Serial.available(); i++)
      recebido[i] = Serial.read();

    if (recebido[0] == 'l') {
      digitalWrite(CONTROLE_MOSFET, HIGH);
      Serial.println("ligou");
      return;
    }
    if (recebido[0] == 'd') {
      digitalWrite(CONTROLE_MOSFET, LOW);
      Serial.println("desligou");
      return;
    }

    recebido[i] = 0;
    pwm_val = atoi(recebido);
    pwm_val = constrain(pwm_val, MG995_MIN, MG995_MAX);

    ledc_set_duty(servo_pwm_channel.speed_mode, servo_pwm_channel.channel,
                  pwm_val);
    ledc_update_duty(servo_pwm_channel.speed_mode, servo_pwm_channel.channel);
  }
}