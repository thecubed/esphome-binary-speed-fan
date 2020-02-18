#pragma once

#include "esphome/core/component.h"
#include "esphome/components/output/binary_output.h"
#include "esphome/components/fan/fan_state.h"

namespace esphome {
namespace binaryspeed {

class BinarySpeedFan : public Component {
 public:
  //SpeedFan(fan::FanState *fan, output::FloatOutput *output) : fan_(fan), output_(output) {}
  BinarySpeedFan(fan::FanState *fan, output::BinaryOutput *low, output::BinaryOutput *medium, output::BinaryOutput *high) : fan_(fan), low_output_(low), medium_output_(medium), high_output_(high) {}
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void set_oscillating(output::BinaryOutput *oscillating) { this->oscillating_ = oscillating; }
  // void set_outputs(output::BinaryOutput *low, output::BinaryOutput *medium, output::BinaryOutput *high) {
  //   this->low_output_ = low;
  //   this->medium_output_ = medium;
  //   this->high_output_ = high;
  // }
  

 protected:
  fan::FanState *fan_;
  output::BinaryOutput *low_output_;
  output::BinaryOutput *medium_output_;
  output::BinaryOutput *high_output_;
  
  output::BinaryOutput *oscillating_{nullptr};

  bool next_update_{true};
  void all_outputs_off();
  void change_speed(output::BinaryOutput *speed_output);
};

}  // namespace binaryspeed
}  // namespace esphome