#pragma once

#include "Controller.hxx"
#include "interfaces/IRumbleController.hxx"
#include "interfaces/IImpulseTriggerController.hxx"
#include <winrt/Windows.Gaming.Input.h>

namespace Gaming = winrt::Windows::Gaming::Input;
namespace Foundation = winrt::Windows::Foundation::Collections;

namespace BrokenBytes::ControllerKit::Internal {
	class GamingInputController : Controller, IRumbleController, IImpulseTriggerController {
	public:
		GamingInputController();
		auto Equals(void* data) -> bool override;

		auto SetRumble(Rumble motor, uint8_t strength) -> void override;
	private:
		static inline Foundation::IVectorView<Gaming::Gamepad> _gamepads;
		Gaming::Gamepad* _addr;
	};
}
