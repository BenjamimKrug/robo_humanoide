#ifndef SERVO_COMPENSATED_CONTROL_H_
#define SERVO_COMPENSATED_CONTROL_H_

#include <stdint.h>

#ifndef USE_SMOOTHING
#define USE_SMOOTHING 1
#endif

#if defined(ARDUINO_ARCH_ESP32)
#include "Arduino.h"
#include "driver/ledc.h"
typedef uint16_t pwm_val_t;
#else
#error "platform not supported"
#endif

class servo_compensated;

typedef pwm_val_t (*calculate_duty_t)(servo_compensated*);

class servo_compensated {
 public:
#if defined(ARDUINO_ARCH_ESP32)
  uint8_t begin(ledc_timer_config_t* _timer_config,
                ledc_channel_config_t* _pwm_channel,
                pwm_val_t* _comp_array,
                calculate_duty_t calc_duty);
#endif
  float set_target_angle(float new_val);
  float get_target_angle();
  float set_cur_angle(float new_val);
  float get_cur_angle();
  float set_speed(float _max_step_per_ms);
  float get_speed();
  void update_duty();
  void handle();

  pwm_val_t* comp_array;
  float min_angle = 0.0f;
  float max_angle = 180.0f;
  
 private:
  calculate_duty_t calculate_duty;

  float cur_angle = 90.0f;
  float target_angle = 90.0f;
  float max_step_per_ms =
      180.0f;  // allows instant jump from one side to another
  uint32_t last_handle_time = 0;

#if defined(ARDUINO_ARCH_ESP32)
  ledc_timer_config_t* timer_config;
  ledc_channel_config_t* pwm_channel;
#endif
};

#endif
