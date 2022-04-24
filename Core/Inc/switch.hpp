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

#ifndef SWITCH_H
#define SWITCH_H

#include "main.h"

class Switch {
 public:
  Switch(GPIO_TypeDef* port, uint16_t pin, bool polarity = false, uint32_t debouncePeriod = 30,
         uint32_t longPressPeriod = 1000, uint32_t doubleClickPeriod = 300, uint32_t deglitchPeriod = 10);

  bool poll();      // Returns 1 if switched
  bool switched();  // will be refreshed by poll()
  bool on();
  bool pushed();       // will be refreshed by poll()
  bool released();     // will be refreshed by poll()
  bool longPress();    // will be refreshed by poll()
  bool doubleClick();  // will be refreshed by poll()
  bool singleClick();  // will be refreshed by poll()

  uint32_t deglitchPeriod, debouncePeriod, longPressPeriod, doubleClickPeriod;

 protected:
  bool process();
  void deglitch();
  void debounce();
  void calcLongPress();
  void calcDoubleClick();
  void calcSingleClick();

  const bool polarity;
  GPIO_TypeDef* port;
  uint16_t pin;

  uint32_t deglitchTime, switchedTime, pushedTime, releasedTime, ms;
  bool input, lastInput, equal, deglitched, debounced, _switched, _longPress, longPressDisable, _doubleClick,
      _singleClick, singleClickDisable;
};

#endif
