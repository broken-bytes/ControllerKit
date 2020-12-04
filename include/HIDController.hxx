#pragma once

#include "Controller.hxx"
#include "HID.hxx"

namespace BrokenBytes::ControllerKit::Internal {

	/// <summary>
	/// The delay before a device is considered disconnected
	/// </summary>
	constexpr uint8_t HID_DC_MS = 250;
	
	class HIDController : public Controller {
	public:
		HIDController(char* path, Types::ControllerType type);
		~HIDController();

	protected:
		auto SendReport(HID::byte* data, size_t length) const -> void;
		auto ReadReport(HID::byte* data, size_t& length) const -> void;
		[[nodiscard]] auto Device() const -> HID::HIDDevice;
		[[nodiscard]] auto Path() const -> char*;
		
	private:
		HID::HIDDevice _device;
		char* _path;
		std::thread _reportSender;
		uint8_t _reportRate = 100;
		unsigned char* _report = {};
		unsigned char* _readReport = {};
	};
}
