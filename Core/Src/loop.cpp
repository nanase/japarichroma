#include "led.hpp"
#include "main.h"
#include "switch.hpp"

extern TIM_HandleTypeDef htim2;

Switch switch_r = Switch(BUTTON_R_GPIO_Port, BUTTON_R_Pin);
Switch switch_g = Switch(BUTTON_G_GPIO_Port, BUTTON_G_Pin);
Switch switch_b = Switch(BUTTON_B_GPIO_Port, BUTTON_B_Pin);
Switch switch_l = Switch(BUTTON_L_GPIO_Port, BUTTON_L_Pin);

LED led_r = LED(&switch_r);
LED led_g = LED(&switch_g);
LED led_b = LED(&switch_b);

uint8_t led_pattern_index    = 0;
const uint8_t led_patterns[] = {
  0b11111, 0b11011, 0b10101, 0b01110, 0b10001, 0b01010, 0b00001, 0b00010, 0b00100, 0b01000, 0b10000,
};

void update_test_pattern();

void update_pattern() {
  uint8_t pattern = led_patterns[led_pattern_index];

  HAL_GPIO_WritePin(LED_0_GPIO_Port, LED_0_Pin, pattern & 0b00001 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, pattern & 0b00010 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, pattern & 0b00100 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, pattern & 0b01000 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LED_4_GPIO_Port, LED_4_Pin, pattern & 0b10000 ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void update_brightnesses(uint8_t r, uint8_t g, uint8_t b) {
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, r / 4);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, g / 4);
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, b / 4);
}

void loop() {
  led_r.check_click();
  led_g.check_click();
  led_b.check_click();

  led_r.update_brightness();
  led_g.update_brightness();
  led_b.update_brightness();

  update_brightnesses(led_r.brightness, led_g.brightness, led_b.brightness);

  switch_l.poll();

  if (switch_l.singleClick()) {
    // next pattern
    led_pattern_index++;

    if (sizeof(led_patterns) - 1 < led_pattern_index)
      led_pattern_index = 0;

    update_pattern();
  }

  if (switch_l.doubleClick()) {
    // first pattern
    led_pattern_index = 0;
    update_pattern();
  }

  if (switch_l.longPress()) {
    // test pattern
    update_test_pattern();
  }

  HAL_Delay(1);
}
