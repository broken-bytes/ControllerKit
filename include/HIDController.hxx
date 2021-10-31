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
		HIDController(DevicePath path, ControllerKitControllerType type);
		virtual ~HIDController();
		auto Equals(void* data) -> bool override;

	protected:
		auto SendReport(byte* data, size_t& length) const -> void;
		auto ReadReport(byte* data, size_t& length) const -> void;
		virtual auto SetDirty() -> void;
		virtual auto SetClear() -> void;
		[[nodiscard]] auto Device() const -> HIDDevice;
		[[nodiscard]] auto Path() const -> DevicePath;
		[[nodiscard]] auto IsDirty() const -> bool;
		
		
	private:
		HIDDevice _device;
		DevicePath _path;
		bool _isDirty;
		std::thread _reportSender;
		uint8_t _reportRate = 100;
		unsigned char* _report = {};
		unsigned char* _readReport = {};
	};
}
