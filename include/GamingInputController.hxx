#pragma once

#include <winrt/Windows.Gaming.Input.h>

#include "Controller.hxx"
#include "interfaces/IRumbleController.hxx"
#include "interfaces/IImpulseTriggerController.hxx"

namespace Gaming = winrt::Windows::Gaming::Input;
namespace Foundation = winrt::Windows::Foundation::Collections;

namespace BrokenBytes::ControllerKit::Internal {
	class GamingInputController :
	public Controller,
	public IRumbleController,
	public IImpulseTriggerController {
	public:
		GamingInputController();
		auto Equals(void* data) -> bool override;

		auto SetRumble(Types::Rumble motor, uint8_t strength) -> void override;
	private:
		std::thread _worker;
		std::vector<Gaming::Gamepad> _gamepads;
		const Gaming::Gamepad* _gamepad = nullptr;
		Gaming::GamepadVibration _vibration;
		
		auto Routine() -> void;
	};
}
