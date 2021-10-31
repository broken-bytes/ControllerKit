#pragma once

#include <array>
#include <bitset>

#include "HIDController.hxx"
#include "interfaces/IGyroscopeController.hxx"
#include "interfaces/ILightbarController.hxx"
#include "interfaces/IRumbleController.hxx"
#include "interfaces/ITouchpadController.hxx"


namespace BrokenBytes::ControllerKit::Internal {
	constexpr uint8_t DUALSHOCK4_READ_REPORT_SIZE = 64;
	constexpr uint8_t DUALSHOCK4_WRITE_REPORT_SIZE = 32;
	class DualShock4 :
		public HIDController,
		public IRumbleController,
		public IGyroscopeController,
		public ILightbarController,
		public ITouchpadController
	{
	public:
		DualShock4(DevicePath path);
		DualShock4(const DualShock4&) = delete;
		DualShock4(const DualShock4&&) = delete;
		~DualShock4();

		auto ReadGyroscope()->Math::Vector3<float> override;
		auto ReadAcceleration()->Math::Vector3<float> override;
		auto SetLightbarColor(ControllerKitColor c) -> void override;
		auto SetRumble(ControllerKitRumble motor, uint8_t strength) -> void override;
		auto GetTouches()->std::vector<Math::Vector2<float>> override;
		
	private:
		unsigned char* _report;
		const uint8_t _pollRateMs = 3;
		unsigned char _readIndex = 0x01;

		std::thread _thread;

		auto Routine() -> void;
		auto SetDirty() -> void override;
		auto SetClear() -> void override;
	};
}
