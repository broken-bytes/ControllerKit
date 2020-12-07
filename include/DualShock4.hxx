#pragma once

#include <array>
#include <bitset>

#include "HIDController.hxx"
#include "Types.hxx"
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
		auto SetLightbarColor(Types::Color c) -> void override;
		auto SetRumble(Types::Rumble motor, uint8_t strength) -> void override;
		auto GetTouches()->std::vector<Math::Vector2<float>> override;
		
	private:
		struct ButtonState {
			std::bitset<14> buttons;
		};

		const std::array<Types::DPadDirection, 9> _dPadDirections = {
			Types::DPadDirection::Up,
			Types::DPadDirection::RightUp,
			Types::DPadDirection::Right,
			Types::DPadDirection::RightDown,
			Types::DPadDirection::Down,
			Types::DPadDirection::LeftDown,
			Types::DPadDirection::Left,
			Types::DPadDirection::LeftUp,
			Types::DPadDirection::None
		};

		unsigned char* _report;
		const uint8_t _pollRateMs = 3;
		unsigned char _readIndex = 0x01;
		int _readSize = 128;

		ButtonState _buttons;

		// Sticks
		Math::Vector2<float> _leftStick{ 0, 0 };
		Math::Vector2<float> _rightStick{ 0, 0 };

		// DPad
		Types::DPadDirection _dPad = Types::DPadDirection::None;
		// L2, R2
		std::array<uint8_t, 2> _triggers{ 0, 0 };

		Types::Color _ledColor{ 0,0,0 };
		std::array<uint8_t, 2> _rumble { 0, 0 };
		std::array<uint8_t, 2> _flashing = { 0,0 };
		
		std::thread _thread;

		auto Routine() -> void;
		auto SetDirty() -> void override;
		auto SetClear() -> void override;
	};
}
