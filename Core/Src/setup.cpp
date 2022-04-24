#include "led.hpp"
#include "main.h"

extern TIM_HandleTypeDef htim2;

extern LED led_r;
extern LED led_g;
extern LED led_b;

void update_pattern();

void setup() {
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

  led_r.brightness_moving = true;
  led_g.brightness_moving = true;
  led_b.brightness_moving = true;
  update_pattern();
}
