#ifndef LED_H
#define LED_H

#include "switch.hpp"

#define BRIGHTNESS_MAX 200
#define MOVING_SPEED 8
#define MOVING_SLOW_SPEED 20

class LED {
 public:
  Switch *switch_button;
  uint8_t brightness;
  bool brightness_direction;  // false: up, true: down
  bool brightness_moving;
  bool brightness_slow_moving;

  uint8_t brightness_count;

  LED(Switch *switch_button);

  void check_click();
  void update_brightness();
};

#endif
