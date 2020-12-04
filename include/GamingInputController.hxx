#pragma once

#include "Controller.hxx"
#include "XInput.hxx"
#include "interfaces/IRumbleController.hxx"
#include "interfaces/IImpulseTriggerController.hxx"


namespace Input = winrt::Windows::Gaming::Input;

namespace BrokenBytes::ControllerKit::Internal {
	class GamingInputController : Controller, IRumbleController, IImpulseTriggerController {
	public:
		GamingInputController(const Input::Gamepad* gamepad);

		static auto Create(const Input::Gamepad* gamepad)->GamingInputController*;
		static auto Remove(const Input::Gamepad* gamepad) ->void;
		
		auto SetRumble(Rumble motor, uint8_t strength) -> void override;

		auto Gamepad() const -> const Input::Gamepad*;
	private:
		const winrt::Windows::Gaming::Input::Gamepad* _gamepad;
	};
}
