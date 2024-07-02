#include "servo_compensated_control.h"

#if defined(ARDUINO_ARCH_ESP32)
uint8_t servo_compensated::begin(ledc_timer_config_t* _timer_config,
                                 ledc_channel_config_t* _pwm_channel,
                                 calculate_duty_t calc_duty) {
  timer_config = _timer_config;
  if (timer_config == NULL) return EINVAL;
  ledc_timer_config(timer_config);

  pwm_channel = _pwm_channel;
  if (pwm_channel == NULL) return EINVAL;
  ledc_channel_config(pwm_channel);

  calculate_duty = calc_duty;
  update_duty();
  return ESP_OK;
}
#endif

float servo_compensated::set_target_angle(float new_val) {
  if (new_val > max_angle)
    target_angle = max_angle;
  else if (new_val < min_angle)
    target_angle = min_angle;
  else
    target_angle = new_val;
  return target_angle;
}

float servo_compensated::get_target_angle() { return target_angle; }

float servo_compensated::set_cur_angle(float new_val) {
  if (new_val > max_angle)
    cur_angle = max_angle;
  else if (new_val < min_angle)
    cur_angle = min_angle;
  else
    cur_angle = new_val;
  return cur_angle;
}

float servo_compensated::get_cur_angle() { return cur_angle; }

float servo_compensated::set_speed(float _max_step_per_ms) {
  max_step_per_ms = _max_step_per_ms;
  return max_step_per_ms;
}

float servo_compensated::get_speed() { return max_step_per_ms; }

void servo_compensated::handle() {
  uint32_t current_millis = millis();
  if (target_angle == cur_angle) {
    last_handle_time = current_millis;
    return;
  }
  float step = target_angle - cur_angle;
  float max_step_in_interval =
      (current_millis - last_handle_time) * max_step_per_ms;
  if (step > max_step_in_interval) step = max_step_in_interval;
  cur_angle += step;
  last_handle_time = current_millis;
  update_duty();
}

void servo_compensated::update_duty() {
#if defined(ARDUINO_ARCH_ESP32)
  ledc_set_duty(pwm_channel->speed_mode, pwm_channel->channel,
                calculate_duty(this));
  ledc_update_duty(pwm_channel->speed_mode, pwm_channel->channel);
#endif
}