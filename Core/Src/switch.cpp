/*
Switch.cpp
Copyright (C) 2012  Albert van Dalen http://www.avdweb.nl
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License at
http://www.gnu.org/licenses . AUTHOR: Albert van Dalen WEBSITE:
http://www.avdweb.nl/arduino/hardware-interfacing/simple-switch-debouncer.html
*/

#include "switch.hpp"

Switch::Switch(GPIO_TypeDef* port, uint16_t pin, bool polarity, uint32_t debouncePeriod, uint32_t longPressPeriod,
               uint32_t doubleClickPeriod, uint32_t deglitchPeriod)
    : deglitchPeriod(deglitchPeriod),
      debouncePeriod(debouncePeriod),
      longPressPeriod(longPressPeriod),
      doubleClickPeriod(doubleClickPeriod),
      polarity(polarity),
      port(port),
      pin(pin) {
  this->switchedTime       = HAL_GetTick();
  this->debounced          = HAL_GPIO_ReadPin(this->port, this->pin);
  this->singleClickDisable = true;
  this->poll();
}

bool Switch::poll() {
  this->input = HAL_GPIO_ReadPin(this->port, this->pin);
  this->ms    = HAL_GetTick();
  return this->process();
}

bool Switch::process() {
  this->deglitch();
  this->debounce();
  this->calcSingleClick();
  this->calcDoubleClick();
  this->calcLongPress();

  if (this->switched()) {
    this->switchedTime = this->ms;  // stores last times for future rounds

    if (this->pushed()) {
      this->pushedTime = this->ms;
    } else {
      this->releasedTime = this->ms;
    }
  }

  // this->triggerCallbacks();
  return this->_switched;
}

void Switch::deglitch() {
  if (this->input == this->lastInput)
    this->equal = 1;
  else {
    this->equal        = 0;
    this->deglitchTime = this->ms;
  }

  if (this->equal && ((this->ms - this->deglitchTime) > this->deglitchPeriod))  // max 50ms, disable deglitch: 0ms
  {
    this->deglitched   = this->input;
    this->deglitchTime = this->ms;
  }

  this->lastInput = this->input;
}

void Switch::debounce() {
  this->_switched = 0;

  if ((this->deglitched != this->debounced) && ((this->ms - this->switchedTime) > this->debouncePeriod)) {
    this->debounced = this->deglitched;
    this->_switched = 1;
  }
}

void Switch::calcSingleClick() {
  this->_singleClick = false;

  if (this->pushed()) {
    if ((this->ms - this->pushedTime) >= this->doubleClickPeriod) {
      this->singleClickDisable = false;  // resets when pushed not in second click of doubleclick
    } else {
      this->singleClickDisable = true;  // silence single click in second cl. doublecl.
    }
  }

  if (!this->singleClickDisable) {
    this->_singleClick =
        !this->switched() && !this->on() && ((this->releasedTime - this->pushedTime) <= this->longPressPeriod) &&
        ((this->ms - this->pushedTime) >= this->doubleClickPeriod);  // true just one time between polls
    this->singleClickDisable = this->_singleClick;                   // will be reset at next push
  }
}

void Switch::calcDoubleClick() {
  this->_doubleClick = this->pushed() && ((this->ms - this->pushedTime) < this->doubleClickPeriod);
}

void Switch::calcLongPress() {
  this->_longPress = false;

  if (this->released())
    this->longPressDisable = false;  // resets when released
  if (!this->longPressDisable) {
    this->_longPress = !this->switched() && this->on() &&
                       ((this->ms - this->pushedTime) > this->longPressPeriod);  // true just one time between polls
    this->longPressDisable = this->_longPress;                                   // will be reset at next release
  }
}

bool Switch::switched() {
  return this->_switched;
}

bool Switch::on() {
  return !(this->debounced ^ this->polarity);
}

bool Switch::pushed() {
  return this->_switched && !(this->debounced ^ this->polarity);
}

bool Switch::released() {
  return this->_switched && (this->debounced ^ this->polarity);
}

bool Switch::longPress() {
  return this->_longPress;
}

bool Switch::doubleClick() {
  return this->_doubleClick;
}

bool Switch::singleClick() {
  return this->_singleClick;
}

// void Switch::triggerCallbacks() {
//   if (this->_pushedCallback && this->pushed()) {
//     this->_pushedCallback(this->_pushedCallbackParam);
//   } else if (this->_releasedCallback && this->released()) {
//     this->_releasedCallback(this->_releasedCallbackParam);
//   }

//   if (this->_longPressCallback && this->longPress()) {
//     this->_longPressCallback(this->_longPressCallbackParam);
//   }

//   if (this->_doubleClickCallback && this->doubleClick()) {
//     this->_doubleClickCallback(this->_doubleClickCallbackParam);
//   }

//   if (this->_singleClickCallback && this->singleClick()) {
//     this->_singleClickCallback(this->_singleClickCallbackParam);
//   }
// }

// void Switch::setPushedCallback(switchCallback_t cb, void* param) {
//   this->_pushedCallback      = cb;  // Store the "pushed" callback function
//   this->_pushedCallbackParam = param;
// }

// void Switch::setReleasedCallback(switchCallback_t cb, void* param) {
//   this->_releasedCallback      = cb;  // Store the "released" callback function
//   this->_releasedCallbackParam = param;
// }

// void Switch::setLongPressCallback(switchCallback_t cb, void* param) {
//   this->_longPressCallback      = cb;  // Store the "long press" callback function
//   this->_longPressCallbackParam = param;
// }

// void Switch::setDoubleClickCallback(switchCallback_t cb, void* param) {
//   this->_doubleClickCallback      = cb;  // Store the "double click" callback function
//   this->_doubleClickCallbackParam = param;
// }

// void Switch::setSingleClickCallback(switchCallback_t cb, void* param) {
//   this->_singleClickCallback      = cb;  // Store the "double click" callback function
//   this->_singleClickCallbackParam = param;
// }
