#include "led.hpp"

LED::LED(Switch *switch_button) : switch_button(switch_button) {}

void LED::check_click() {
  this->switch_button->poll();

  if (this->switch_button->doubleClick()) {
    this->brightness             = BRIGHTNESS_MAX;
    this->brightness_moving      = false;
    this->brightness_slow_moving = false;
  } else if (this->switch_button->singleClick()) {
    this->brightness_moving = !this->brightness_moving;

    if (this->brightness == BRIGHTNESS_MAX)
      this->brightness_direction = true;
    else if (this->brightness == 0)
      this->brightness_direction = false;
  } else if (this->switch_button->longPress()) {
    this->brightness_slow_moving = true;
  } else if (this->switch_button->released()) {
    if (this->brightness_slow_moving)
      this->brightness_slow_moving = false;
  }
}

void LED::update_brightness() {
  if (this->brightness_moving) {
    if (this->brightness_direction) {
      if (this->brightness > 0) {
        if (brightness_count++ > MOVING_SPEED) {
          this->brightness--;
          brightness_count = 0;
        }
      } else {
        this->brightness_moving = false;
      }
    } else {
      if (this->brightness < BRIGHTNESS_MAX) {
        if (brightness_count++ > MOVING_SPEED) {
          this->brightness++;
          brightness_count = 0;
        }
      } else {
        this->brightness_moving = false;
      }
    }
  }

  if (this->brightness_slow_moving) {
    if (this->brightness_direction) {
      if (this->brightness > 0) {
        if (brightness_count++ > MOVING_SLOW_SPEED) {
          this->brightness--;
          brightness_count = 0;
        }
      } else {
        this->brightness_direction = !this->brightness_direction;
      }
    } else {
      if (this->brightness < BRIGHTNESS_MAX) {
        if (brightness_count++ > MOVING_SLOW_SPEED) {
          this->brightness++;
          brightness_count = 0;
        }
      } else {
        this->brightness_direction = !this->brightness_direction;
      }
    }
  }
}
