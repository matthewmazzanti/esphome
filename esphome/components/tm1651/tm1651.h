#pragma once

#include <memory>

#include "esphome/core/component.h"
#include "esphome/core/esphal.h"
#include "esphome/core/automation.h"

#include <TM1651.h>

namespace esphome {
namespace tm1651 {

class TM1651Display : public Component {
 public:
  void set_clk_pin(GPIOPin *pin) { clk_pin_ = pin; }
  void set_dio_pin(GPIOPin *pin) { dio_pin_ = pin; }

  void setup() override;
  void dump_config() override;

  void set_level_percent(uint8_t);
  void set_level(uint8_t);
  void set_brightness(uint8_t);

  void turn_on();
  void turn_off();

 protected:
  std::unique_ptr<TM1651> battery_display_;
  GPIOPin *clk_pin_;
  GPIOPin *dio_pin_;
  bool is_on_ = true;

  uint8_t brightness_;
  uint8_t level_;

  void repaint_();

  uint8_t calculate_level_(uint8_t);
  uint8_t calculate_brightness_(uint8_t);
};

template<typename... Ts> class SetLevelPercentAction : public Action<Ts...>, public Parented<TM1651Display> {
 public:
  TEMPLATABLE_VALUE(uint8_t, level_percent)

  void play(Ts... x) override {
    auto level_percent = this->level_percent_.value(x...);
    this->parent_->set_level_percent(level_percent);
  }
};

template<typename... Ts> class SetLevelAction : public Action<Ts...>, public Parented<TM1651Display> {
 public:
  TEMPLATABLE_VALUE(uint8_t, level)

  void play(Ts... x) override {
    auto level = this->level_.value(x...);
    this->parent_->set_level(level);
  }
};

template<typename... Ts> class SetBrightnessAction : public Action<Ts...>, public Parented<TM1651Display> {
 public:
  TEMPLATABLE_VALUE(uint8_t, brightness)

  void play(Ts... x) override {
    auto brightness = this->brightness_.value(x...);
    this->parent_->set_brightness(brightness);
  }
};

template<typename... Ts> class TurnOnAction : public Action<Ts...>, public Parented<TM1651Display> {
 public:
  void play(Ts... x) override { this->parent_->turn_on(); }
};

template<typename... Ts> class TurnOffAction : public Action<Ts...>, public Parented<TM1651Display> {
 public:
  void play(Ts... x) override { this->parent_->turn_off(); }
};

}  // namespace tm1651
}  // namespace esphome
