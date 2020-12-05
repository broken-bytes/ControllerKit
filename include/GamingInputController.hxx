#pragma once

#include "Controller.hxx"
#include "interfaces/IRumbleController.hxx"
#include "interfaces/IImpulseTriggerController.hxx"
#include <winrt/Windows.Gaming.Input.h>

namespace Input = winrt::Windows::Gaming::Input;

namespace BrokenBytes::ControllerKit::Internal {
	class GamingInputController : Controller, IRumbleController, IImpulseTriggerController {
	public:
		GamingInputController(Input::Gamepad* addr);
		auto Equals(void* data) -> bool override;

		auto SetRumble(Rumble motor, uint8_t strength) -> void override;
	private:
		Input::Gamepad* _addr;
		std::thread _t;
	};
}
