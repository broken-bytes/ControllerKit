#pragma once

#include <array>
#include <bitset>

#include "HIDController.hxx"
#include "interfaces/IGyroscopeController.hxx"
#include "interfaces/ILightbarController.hxx"
#include "interfaces/IRumbleController.hxx"
#include "interfaces/ITouchpadController.hxx"


namespace BrokenBytes::ControllerKit::Internal
{
	class DualShock4 :
		public HIDController,
		public IRumbleController,
		public IGyroscopeController,
		public ILightbarController,
		public ITouchpadController
	{
	public:
		DualShock4(char* path);
		DualShock4(const DualShock4&) = delete;
		DualShock4(const DualShock4&&) = delete;
		DualShock4& operator=(const DualShock4&) = delete;
		DualShock4& operator=(DualShock4&&) = delete;
		~DualShock4();
		
	private:
		struct ButtonState {
			std::bitset<14> buttons;
		};

		const std::array<DPadDirection, 9> _dPadDirections = {
			DPadDirection::Up,
			DPadDirection::RightUp,
			DPadDirection::Right,
			DPadDirection::RightDown,
			DPadDirection::Down,
			DPadDirection::LeftDown,
			DPadDirection::Left,
			DPadDirection::LeftUp,
			DPadDirection::None
		};

		const uint8_t _pollRateMs = 3;
		unsigned char _readIndex = 0x01;
		int _readSize = 128;

		ButtonState _buttons;

		// Sticks
		Vector2<float> _leftStick{ 0, 0 };
		Vector2<float> _rightStick{ 0, 0 };

		// DPad
		DPadDirection _dPad = DPadDirection::None;
		// L2, R2
		std::array<uint8_t, 2> _triggers{ 0, 0 };

		Color _ledColor{ 0,0,0 };
		std::array<uint8_t, 2> _rumble { 0, 0 };
		std::array<uint8_t, 2> _flashing = { 0,0 };
		
		std::thread _thread;

		void Routine();

		void WriteReports();
		void ReadReports();
	public:
		Math::Vector3<float> ReadGyroscope() override;
		Math::Vector3<float> ReadAcceleration() override;
		void SetLightbarColor(Color c) override;
		void SetRumble(Rumble motor, uint8_t strength) override;
		std::vector<Math::Vector2<uint8_t>> GetTouches() override;
	};
}
