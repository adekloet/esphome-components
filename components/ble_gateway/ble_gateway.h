#pragma once

#include "esphome/core/component.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"

#ifdef USE_ESP32

#include "esphome/core/automation.h"

namespace esphome {
namespace ble_gateway {

std::string scan_rst_to_hci_packet_hex(const esp_ble_gap_cb_param_t::ble_scan_result_evt_param &param);

class BLEGatewayDevice {
 public:
  BLEGatewayDevice(uint64_t address) { this->address_ = address; }
  uint64_t address_;
};

class BLEGateway : public Component, public esp32_ble_tracker::ESPBTDeviceListener {
 public:
  void register_device(BLEGatewayDevice *device) { this->devices_.push_back(device); }

  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
  void add_callback(std::function<void(const esp32_ble_tracker::ESPBTDevice &, std::string)> &&callback) { this->callback_.add(std::move(callback)); }
  bool parse_device(const esp32_ble_tracker::ESPBTDevice &device) override;
 protected:
  std::vector<BLEGatewayDevice *> devices_;
  CallbackManager<void(const esp32_ble_tracker::ESPBTDevice &, std::string)> callback_;
};

class BLEGatewayBLEAdvertiseTrigger : public Trigger<const esp32_ble_tracker::ESPBTDevice &, std::string> {
 public:
  explicit BLEGatewayBLEAdvertiseTrigger(BLEGateway *parent) {
   parent->add_callback([this](const esp32_ble_tracker::ESPBTDevice &value, std::string packet) {
    this->trigger(value, packet); });
  }
};

}  // namespace ble_gateway
}  // namespace esphome

#endif
