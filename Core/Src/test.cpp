#include "led.hpp"
#include "main.h"
#include "switch.hpp"

#define TEST_SPEED 20
#define TEST_WAIT 1500

extern Switch switch_r;
extern Switch switch_g;
extern Switch switch_b;
extern Switch switch_l;

extern LED led_r;
extern LED led_g;
extern LED led_b;

void update_brightnesses(uint8_t r, uint8_t g, uint8_t b);

static bool switch_pushed() {
  switch_r.poll();
  switch_g.poll();
  switch_b.poll();
  switch_l.poll();

  return switch_r.pushed() || switch_g.pushed() || switch_b.pushed() || switch_l.pushed();
}

void update_test_pattern() {
  while (1) {
    // R
    while (led_r.brightness < BRIGHTNESS_MAX || led_g.brightness > 0 || led_b.brightness > 0) {
      if (led_r.brightness < BRIGHTNESS_MAX) {
        led_r.brightness++;
      }

      if (led_g.brightness > 0) {
        led_g.brightness--;
      }

      if (led_b.brightness > 0) {
        led_b.brightness--;
      }

      update_brightnesses(led_r.brightness, led_g.brightness, led_b.brightness);
      HAL_Delay(TEST_SPEED);

      if (switch_pushed())
        return;
    }

    for (size_t i = 0; i < TEST_WAIT; i++) {
      HAL_Delay(1);

      if (switch_pushed())
        return;
    }

    // R -> RG
    while (led_g.brightness < BRIGHTNESS_MAX) {
      if (led_g.brightness < BRIGHTNESS_MAX) {
        led_g.brightness++;
      }

      update_brightnesses(led_r.brightness, led_g.brightness, led_b.brightness);
      HAL_Delay(TEST_SPEED);

      if (switch_pushed())
        return;
    }

    for (size_t i = 0; i < TEST_WAIT; i++) {
      HAL_Delay(1);

      if (switch_pushed())
        return;
    }

    // RG -> G
    while (led_r.brightness > 0) {
      if (led_r.brightness > 0) {
        led_r.brightness--;
      }

      update_brightnesses(led_r.brightness, led_g.brightness, led_b.brightness);
      HAL_Delay(TEST_SPEED);

      if (switch_pushed())
        return;
    }

    for (size_t i = 0; i < TEST_WAIT; i++) {
      HAL_Delay(1);

      if (switch_pushed())
        return;
    }

    // G -> GB
    while (led_b.brightness < BRIGHTNESS_MAX) {
      if (led_b.brightness < BRIGHTNESS_MAX) {
        led_b.brightness++;
      }

      update_brightnesses(led_r.brightness, led_g.brightness, led_b.brightness);
      HAL_Delay(TEST_SPEED);

      if (switch_pushed())
        return;
    }

    for (size_t i = 0; i < TEST_WAIT; i++) {
      HAL_Delay(1);

      if (switch_pushed())
        return;
    }

    // GB -> B
    while (led_g.brightness > 0) {
      if (led_g.brightness > 0) {
        led_g.brightness--;
      }

      update_brightnesses(led_r.brightness, led_g.brightness, led_b.brightness);
      HAL_Delay(TEST_SPEED);

      if (switch_pushed())
        return;
    }

    for (size_t i = 0; i < TEST_WAIT; i++) {
      HAL_Delay(1);

      if (switch_pushed())
        return;
    }

    // B -> BR
    while (led_r.brightness < BRIGHTNESS_MAX) {
      if (led_r.brightness < BRIGHTNESS_MAX) {
        led_r.brightness++;
      }

      update_brightnesses(led_r.brightness, led_g.brightness, led_b.brightness);
      HAL_Delay(TEST_SPEED);

      if (switch_pushed())
        return;
    }

    for (size_t i = 0; i < TEST_WAIT; i++) {
      HAL_Delay(1);

      if (switch_pushed())
        return;
    }
  }
}
