#include "HIDController.hxx"


namespace BrokenBytes::ControllerKit::Internal {
	HIDController::HIDController(DevicePath path, ControllerKitControllerType type)
	: Controller(type) {
		this->_path = path;
		this->_device = nullptr;
		this->_isDirty = false;
		HID::OpenDevice(path, &_device);
	}

	HIDController::~HIDController() {
		delete this->_path;
		HID::CloseDevice(_device);
	}

	auto HIDController::Equals(void* data) -> bool {
		return this->Path() == data;
	}

	auto HIDController::SendReport(byte* data, size_t& length) const -> void {
		length = HID::WriteToDevice(_device, data, length);
	}

	auto HIDController::ReadReport(byte* data, size_t& length) const -> void {
		length = HID::ReadFromDevice(_device, data, length);
	}

	auto HIDController::SetDirty() -> void {
		_isDirty = true;
	}
	auto HIDController::SetClear() -> void {
		_isDirty = false;
	}

	auto HIDController::Device() const -> HIDDevice {
		return _device;
	}

	auto HIDController::Path() const -> DevicePath {
		return _path;
	}

	auto HIDController::IsDirty() const -> bool {
		return _isDirty;
	}
}
