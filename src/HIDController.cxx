#include "HIDController.hxx"


namespace BrokenBytes::ControllerKit::Internal {
	HIDController::HIDController(char* path, Types::ControllerType type)
	: Controller(type) {
		this->_path = path;
		HID::OpenDevice(path, _report);
	}

	HIDController::~HIDController() {
		delete this->_path;
		HID::CloseDevice(_device);
	}

	bool HIDController::operator==(char* path) const {
		return _path == path;
	}

	bool HIDController::operator==(const char* path) const {
		return _path == path;
	}

	auto HIDController::Equals(void* data) -> bool {
		return this->Path() == data;
	}

	auto HIDController::SendReport(HID::byte* data, size_t length) const -> void {
		HID::WriteToDevice(_device, _report, sizeof(_report));
	}

	auto HIDController::ReadReport(HID::byte* data, size_t& length) const -> void {
		if(HID::ReadFromDevice(_device, data, length <= 0)) {
			// The device is not connected anymore, delete it
			delete this;
		}
	}

	auto HIDController::Device() const -> HID::HIDDevice {
		return _device;
	}

	auto HIDController::Path() const -> char* {
		return _path;
	}
}
