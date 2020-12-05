#pragma once

#include "Controller.hxx"
#include "interfaces/IRumbleController.hxx"
#include "interfaces/IImpulseTriggerController.hxx"
#include <winrt/Windows.Gaming.Input.h>

namespace Input = winrt::Windows::Gaming::Input;

namespace BrokenBytes::ControllerKit::Internal {
	class GamingInputController : Controller, IRumbleController, IImpulseTriggerController {
	public:
		GamingInputController(const Input::Gamepad* gamepad);
		bool operator==(const Input::Gamepad* gamepad) const;
		bool operator==(const Input::Gamepad& rhs) const;
		auto Equals(void* data) -> bool override;

		auto SetRumble(Rumble motor, uint8_t strength) -> void override;
		auto Gamepad() const -> const Input::Gamepad*;
	private:
		const winrt::Windows::Gaming::Input::Gamepad* _gamepad;
	};
}
