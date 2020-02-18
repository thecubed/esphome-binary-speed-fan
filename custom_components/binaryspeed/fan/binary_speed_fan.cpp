#include "binary_speed_fan.h"
#include "esphome/core/log.h"

namespace esphome {
namespace binaryspeed {

static const char *TAG = "binaryspeed.fan";

void BinarySpeedFan::dump_config() {
  ESP_LOGCONFIG(TAG, "Fan '%s':", this->fan_->get_name().c_str());
  if (this->fan_->get_traits().supports_oscillation()) {
    ESP_LOGCONFIG(TAG, "  Oscillation: YES");
  }
}
void BinarySpeedFan::setup() {
  auto traits = fan::FanTraits(this->oscillating_ != nullptr, true);
  this->fan_->set_traits(traits);
  this->fan_->add_on_state_callback([this]() { this->next_update_ = true; });
}

void BinarySpeedFan::all_outputs_off() {
  this->low_output_->turn_off();
  this->medium_output_->turn_off();
  this->high_output_->turn_off();
}
void BinarySpeedFan::change_speed(output::BinaryOutput *speed_output) {
  this->all_outputs_off();
  speed_output->turn_on();
}

void BinarySpeedFan::loop() {
  if (!this->next_update_) {
    return;
  }
  this->next_update_ = false;

  {
    bool enable = this->fan_->state;
    if (enable) {
	  // Fan is enabled and has a speed
      if (this->fan_->speed == fan::FAN_SPEED_LOW) {
        this->change_speed(this->low_output_);
      } else if (this->fan_->speed == fan::FAN_SPEED_MEDIUM) {
        this->change_speed(this->medium_output_);
      } else if (this->fan_->speed == fan::FAN_SPEED_HIGH) {
        this->change_speed(this->high_output_);
	  }
    } else {
	  // Fan is turned off
	  this->all_outputs_off();
    }
    ESP_LOGD(TAG, "Setting fan: %s, speed: %d", ONOFF(enable), this->fan_->speed);
  }

  if (this->oscillating_ != nullptr) {
    bool enable = this->fan_->oscillating;
    if (enable) {
      this->oscillating_->turn_on();
    } else {
      this->oscillating_->turn_off();
    }
    ESP_LOGD(TAG, "Setting oscillation: %s", ONOFF(enable));
  }
}
float BinarySpeedFan::get_setup_priority() const { return setup_priority::DATA; }

}  // namespace binaryspeed
}  // namespace esphome